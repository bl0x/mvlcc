#include <mvlc_readout_config.h>
#include <mvlc_wrap.h>
#include <stdio.h>

int main(){
  mvlc_t mvlc = mvlc_make_mvlc_eth("192.168.1.103");
  int ec;

  if (ec = mvlc_connect(mvlc))
    {
      printf("Could not connect\n");
      return 1;
    }
  
  uint32_t vmeBase = 0x21000000u;
  uint16_t regAddr = 0x6008u;
  uint32_t readValue = 0u;

  ec = mvlc_single_vme_read(mvlc, vmeBase + regAddr, &readValue, 178, 0);

  regAddr = 0x6004u;
  
  ec = mvlc_single_vme_write(mvlc, vmeBase + regAddr, 9, 178, 0);
  ec = mvlc_single_vme_read(mvlc, vmeBase + regAddr, &readValue, 178, 0);

  mvlc_disconnect(mvlc);

  return 0;
}
