#pragma once

#include <stddef.h>
#include <stdint.h>

typedef void *mvlc_t;

#ifdef __cplusplus
extern "C" {
#endif

mvlc_t mvlc_make_mvlc_from_crate_config(const char *);
mvlc_t mvlc_make_mvlc_eth(const char *);
int mvlc_connect(mvlc_t);
int mvlc_stop(mvlc_t);
void mvlc_disconnect(mvlc_t);
int mvlc_init_readout(mvlc_t);
int mvlc_readout_eth(mvlc_t, uint8_t **, size_t);

#ifdef __cplusplus
}
#endif
