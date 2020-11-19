ifeq (,$(QUIET))
  QUIET:=@
endif

MVLC_DIR := /home/bloeher/opt/mesytec_mvme/mesytec-mvlc

CCNAME:=$(notdir $(CC))
BUILD_DIR := build_$(CCNAME)_$(shell $(CC) -dumpmachine)_$(shell $(CC) -dumpversion)

CXXFLAGS := -Wall -Wextra -Werror -ggdb -Wfatal-errors -Wshadow
CXXFLAGS += -march=native
CXXFLAGS += -fdiagnostics-color=auto
CXXFLAGS += -fPIC
CXXFLAGS += -Iinclude
CXXFLAGS += -std=c++17
CXXFLAGS += -I$(MVLC_DIR)/src/mesytec-mvlc
CXXFLAGS += -I$(MVLC_DIR)/build/src/mesytec-mvlc

ifeq (,$(MODE))
  CXXFLAGS += -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE
  CXXFLAGS += -ggdb -O
endif
ifeq (release,$(MODE))
  COMPILE_MODE := release
  BUILD_DIR := $(BUILD_DIR)_release
  CXXFLAGS += -O3
endif

TARGET := $(BUILD_DIR)/libmvlcc.a

mvlcc_LIBS := $(TARGET) -lstdc++
mvlcc_DEPS := $(TARGET)

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.d,$(SOURCES))

.PHONY: all
all: $(TARGET)

include $(DEPS)

.DEFAULT_GOAL :=

$(TARGET): $(OBJECTS)
	@echo " AR " $@
	$(QUIET)$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: src/%.cpp
	@echo "CXX " $@
	$(QUIET)$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.d: src/%.cpp | $(BUILD_DIR)
	@echo "DEP " $@; \
         set -e; rm -f $@; \
         $(CC) -MM $(CXXFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

$(BUILD_DIR):
	@test -d $@ || \
		echo "MKDR" $@ && \
		mkdir -p $(BUILD_DIR) && \
		mkdir -p $(BUILD_DIR)/bin

.PHONY: clean
clean:
	rm -rf ./$(BUILD_DIR)
