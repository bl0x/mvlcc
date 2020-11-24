#include <mesytec-mvlc.h>
#include <mvlc.h>
#include <mvlc_factory.h>
#include <mvlc_wrap.h>

using namespace mesytec::mvlc;

struct mvlcc
{
	CrateConfig config;
	MVLC mvlc;
};

mvlc_t
mvlc_make_mvlc_from_crate_config(const char *configname)
{
	/*TH1D *h = static_cast<TH1D *>(a_th1d);*/
	auto m = new mvlcc();
	std::ifstream config(configname);
	assert(config.is_open());
	m->config = crate_config_from_yaml(config);
	m->mvlc = make_mvlc(m->config);
	return m;
}

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

void
mvlc_disconnect(mvlc_t a_mvlc)
{
	auto m = static_cast<struct mvlcc *>(a_mvlc);
	m->mvlc.disconnect();
}
int
mvlc_init_readout(mvlc_t a_mvlc)
{
	int rc;
	auto m = static_cast<struct mvlcc *>(a_mvlc);
	auto result = init_readout(m->mvlc, m->config, {});
	rc = result.ec.value();
	if (rc != 0) {
		printf("'%s'\n", result.ec.message().c_str());
	}
	return rc;
}
