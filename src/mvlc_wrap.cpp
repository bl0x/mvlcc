#include <mesytec-mvlc.h>
#include <mvlc.h>
#include <mvlc_factory.h>
#include <mvlc_wrap.h>

using namespace mesytec::mvlc;

struct mvlcc
{
	mesytec::mvlc::CrateConfig config;
	mesytec::mvlc::MVLC mvlc;
	mesytec::mvlc::eth::MVLC_ETH_Interface *ethernet;
};

int readout_eth(eth::MVLC_ETH_Interface *a_eth, uint8_t *a_buffer,
    size_t *bytes_transferred);
int send_empty_request(MVLC *a_mvlc);

mvlc_t
mvlc_make_mvlc_from_crate_config(const char *configname)
{
	auto m = new mvlcc();
	std::ifstream config(configname);
	if(!config.is_open()) {
		printf("Could not open file '%s'\n", configname);
	}
	m->config = crate_config_from_yaml(config);
	m->mvlc = make_mvlc(m->config);
	m->ethernet = dynamic_cast<eth::MVLC_ETH_Interface *>(
	    m->mvlc.getImpl());
	return m;
}

mvlc_t
mvlc_make_mvlc_eth(const char *hostname)
{
	auto m = new mvlcc();
	m->mvlc = make_mvlc_eth(hostname);
	return m;
}

int
mvlc_connect(mvlc_t a_mvlc)
{
	int rc;
	auto m = static_cast<struct mvlcc *>(a_mvlc);

	/* cancel ongoing readout when connecting */
	m->mvlc.setDisableTriggersOnConnect(true);

	auto ec = m->mvlc.connect();
	rc = ec.value();
	if (rc != 0) {
		printf("'%s'\n", ec.message().c_str());
		abort();
	}
	return rc;
}

int
mvlc_stop(mvlc_t a_mvlc)
{
	auto m = static_cast<struct mvlcc *>(a_mvlc);

	/* perhaps try this a couple of times */
	auto ec = disable_all_triggers_and_daq_mode(m->mvlc);
	if (ec) {
		printf("'%s'\n", ec.message().c_str());
		abort();
	}

	return 0;
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

	assert(m->ethernet);

	auto result = init_readout(m->mvlc, m->config, {});

	printf("mvlc_init_readout\n");
	std::cout << "init_readout result = " << result.init << std::endl;

	rc = result.ec.value();
	if (rc != 0) {
		printf("init_readout: '%s'\n", result.ec.message().c_str());
		abort();
	}

	m->ethernet->resetPipeAndChannelStats();

	send_empty_request(&m->mvlc);

	auto ec = setup_readout_triggers(m->mvlc, m->config.triggers);
	if (ec) {
		printf("setup_readout_triggers: '%s'\n", ec.message().c_str());
		abort();
	}

	return rc;
}

int
send_empty_request(MVLC *a_mvlc)
{
	size_t bytesTransferred = 0;

	static const uint32_t empty_request[2] = {
		0xf1000000, 0xf2000000
	};

	auto ec = a_mvlc->write(Pipe::Data,
	    reinterpret_cast<const uint8_t *>(empty_request),
	    2 * sizeof(uint32_t), bytesTransferred);

	printf("send_empty_request: bytesTransferred = %lu\n", bytesTransferred);

	if (ec) {
		printf("Failure writing empty request.\n");
		abort();
	}

	return 0;
}

int
readout_eth(eth::MVLC_ETH_Interface *a_eth, uint8_t *a_buffer,
    size_t a_buffer_len, size_t *a_bytes_transferred)
{
	int rc = 0;
	size_t total_bytes = 0;
	uint8_t *buffer = a_buffer;
	size_t bytes_free = a_buffer_len;
	int cycle = 0;

	printf("  readout_eth: start, bytes_free = %lu\n",
	    bytes_free);
	while (bytes_free >= eth::JumboFrameMaxSize)
	{
		auto result = a_eth->read_packet(Pipe::Data, buffer,
		    bytes_free);
		auto ec = result.ec;
		if (ec == ErrorType::ConnectionError) {
			printf("Connection error.\n");
			abort();
		}
		if (ec == MVLCErrorCode::ShortRead) {
			printf("Short read.\n");
			abort();
		}
		rc = ec.value();
		if (rc != 0) {
			printf("'%s'\n", result.ec.message().c_str());
			abort();
		}
		if (result.leftoverBytes()) {
			printf("Leftover bytes. Bailing out!\n");
			abort();
		}
		buffer += result.bytesTransferred;
		bytes_free -= result.bytesTransferred;
		total_bytes += result.bytesTransferred;

		printf("  readout_eth: cycle = %d, bytes_free = %lu, bytes_tranferred = %d, total_bytes = %lu\n",
		    cycle, bytes_free, result.bytesTransferred, total_bytes);

		++cycle;
	}

	*a_bytes_transferred = total_bytes;
	return rc;
}

int
mvlc_readout_eth(mvlc_t a_mvlc, uint8_t **a_buffer, size_t bytes_free)
{
	int rc;
	size_t bytes_transferred;
	uint8_t *buffer;
	auto m = static_cast<struct mvlcc *>(a_mvlc);

	buffer = *a_buffer;

	printf("mvlc_readout_eth: a_buffer@%p, bytes_free = %lu\n", (void *)*a_buffer, bytes_free);

	rc = readout_eth(m->ethernet, buffer, bytes_free, &bytes_transferred);
	if (rc != 0) {
		printf("Failure in readout_eth %d\n", rc);
		abort();
	}

	printf("Transferred %lu bytes\n", bytes_transferred);

	*a_buffer += bytes_transferred;

	return rc;
}
