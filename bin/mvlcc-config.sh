#!/bin/sh

# The purpose of this script is to provide necessary compile and link
# flags to _user_ programs.  I.e. not when compiling mvlcc itself.

if [ -z "$CC" ]; then
    CC=cc
fi

CC_MACHINE=`$CC -dumpmachine`
CC_VERSION=`$CC -dumpversion`
ARCH_SUFFIX=${CC_MACHINE}_${CC_VERSION}

usage()
{
    echo "Usage: mvlcc-config [options]"
    echo ""
    echo "  --cflags       echos C compiler flags needed"
    echo "  --ldflags      echos linker flags needed"
    echo "  --libs         echos library flags needed"
    echo "  --arch-prefix  echos the architecture string (used for directory names)"
    echo "  --help         prints this message"
    exit $1
}

if [ $# -eq 0 ]; then
    usage 1 1>&2
fi

SCRIPTDIR="$( cd "$( dirname "$0" )" && pwd )"

# echo directory $SCRIPTDIR

MVLCC_DIR=$SCRIPTDIR/..

LIB_DIR=build_${CC}_${ARCH_SUFFIX}

if [ -e ${MVLCC_DIR}/${LIB_DIR}/mvlcc.config ]
then
    . ${MVLCC_DIR}/${LIB_DIR}/mvlcc.config
else
    echo "Missing: ${MVLCC_DIR}/${LIB_DIR}/mvlcc.config" 1>&2
    exit 1
fi

CFLAGS="$CFLAGS -I ${MVLCC_DIR}/include/"
LDFLAGS="$LDFLAGS"
# The platform support libs ($LIBS from config) must be last
LIBSDIR="-L${MVLCC_DIR}/${LIB_DIR} -L${MVLC_DIR}/build/"
LIBS="$LIBS -lmvlcc -lmesytec-mvlc \
    -Wl,-rpath=${MVLC_DIR}/build/"

while [ $# -gt 0 ]; do
    case $1 in
	--arch-prefix)
	    OUT="$OUT ${ARCH_SUFFIX}"
	    ;;
	--cflags)
	    OUT="$OUT ${CFLAGS}"
	    ;;
	--ldflags)
	    OUT="$OUT ${LDFLAGS}"
	    ;;
	--libs)
	    OUT="$OUT ${LIBSDIR}"
	    OUT="$OUT ${LIBS}"
	    ;;
	--help|-h)
	    usage 0
	    ;;
	*)
	    echo "Unknown argument: $1" 1>&2
	    usage 1 1>&2
	    ;;
    esac
    shift
done

echo $OUT
