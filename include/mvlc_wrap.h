#pragma once
typedef void *mvlc_t;

#ifdef __cplusplus
extern "C" {
#endif

mvlc_t mvlc_make_mvlc_eth(const char *);
int mvlc_connect(mvlc_t);

#ifdef __cplusplus
}
#endif
