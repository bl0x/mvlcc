#pragma once
typedef void *mvlc_t;

#ifdef __cplusplus
extern "C" {
#endif

mvlc_t mvlc_make_mvlc_from_crate_config(const char *);
mvlc_t mvlc_make_mvlc_eth(const char *);
int mvlc_connect(mvlc_t);
void mvlc_disconnect(mvlc_t);
int mvlc_init_readout(mvlc_t);

#ifdef __cplusplus
}
#endif
