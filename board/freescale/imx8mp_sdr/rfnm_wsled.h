#ifndef RFNM_UBOOT_WSLED
#define RFNM_UBOOT_WSLED

void rfnm_wsled(uint32_t reg, uint32_t led1, uint32_t led2, uint32_t led3);
void rfnm_wsled_spl(uint32_t reg, uint32_t led1, uint32_t led2, uint32_t led3);

#endif