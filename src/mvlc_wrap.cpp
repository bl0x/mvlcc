#include <mvlc.h>
#include <mvlc_factory.h>
#include <mvlc_wrap.h>

using namespace mesytec::mvlc;

struct mvlcc
{
	MVLC mvlc;
};

mvlc_t
mvlc_make_mvlc_eth(const char *hostname)
{
	/*TH1D *h = static_cast<TH1D *>(a_th1d);*/
	auto m = new mvlcc();
	m->mvlc = make_mvlc_eth(hostname);
	return m;
}

int
mvlc_connect(mvlc_t a_mvlc)
{
	int rc;
	auto m = static_cast<struct mvlcc *>(a_mvlc);
	auto ec = m->mvlc.connect();
	rc = ec.value();
	if (rc != 0) {
		printf("'%s'\n", ec.message().c_str());
	}
	return rc;
}
