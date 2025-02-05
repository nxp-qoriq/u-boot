#include <common.h>
#include <stdint.h>

#define NOP1() asm volatile("nop");
#define NOP2()   NOP1()  NOP1()
#define NOP4()   NOP2()  NOP2()
#define NOP8()   NOP4()  NOP4()
#define NOP16()  NOP8()  NOP8()
#define NOP32() NOP16() NOP16()
#define NOP64() NOP32() NOP32()

void rfnm_wsled_spl(uint32_t reg, uint32_t led1, uint32_t led2, uint32_t led3) {

	volatile unsigned int *addr;
	addr = 0x30220000;

	uint32_t initial = *addr;
	uint32_t cond_high = initial | reg;
	uint32_t cond_low = initial & ~(reg);

	// 1000 = 1.06us
	// 200 = 160ns

	int z;

	uint32_t send[4];
	send[0] = 0;
	send[1] = led1;
	send[2] = led2;
	send[3] = led3;

	*addr = cond_low; for(z = 0; z < 5000; z++) asm volatile ("nop");

	int8_t current_bit = 0;
	uint8_t current_led = 0;

	uint8_t bit = (send[current_led] & (1 << current_bit)) >> current_bit;

	for(current_led = 0; current_led < 4; current_led++) {
		for(current_bit = 23; current_bit >= 0; current_bit--) {

			if(current_led != 0) {
				*addr = cond_high; // reset condition while prefetching the loop
			}

			bit = (send[current_led] & (1 << current_bit)) >> current_bit;

			// datasheet is all lies...

			// reset time -> 280us+
			// send one  -> high for 620 ns; then low for 930 ns
			// send zero -> high for 300 ns; then low for 930 ns


			if(bit) {
				NOP64() NOP16() NOP8()
			} else {
				NOP8() NOP4() NOP4() 
			}

			*addr = cond_low;

			NOP64() NOP32() NOP8() NOP8() NOP8() NOP8()

		}
	}

	*addr = initial;
}


void rfnm_wsled(uint32_t reg, uint32_t led1, uint32_t led2, uint32_t led3) {

	volatile unsigned int *addr;
	addr = 0x30220000;

	uint32_t initial = *addr;
	uint32_t cond_high = initial | reg;
	uint32_t cond_low = initial & ~(reg);

	// 1000 = 1.06us
	// 200 = 160ns

	int z;

	uint32_t send[4];
	send[0] = 0;
	send[1] = led1;
	send[2] = led2;
	send[3] = led3;

	*addr = cond_low; for(z = 0; z < 5000; z++) asm volatile ("nop");

	int8_t current_bit = 0;
	uint8_t current_led = 0;

	uint8_t bit = (send[current_led] & (1 << current_bit)) >> current_bit;

	for(current_led = 0; current_led < 4; current_led++) {
		for(current_bit = 23; current_bit >= 0; current_bit--) {

			if(current_led != 0) {
				*addr = cond_high; // reset condition while prefetching the loop
			}

			bit = (send[current_led] & (1 << current_bit)) >> current_bit;

			// datasheet is all lies...

			// reset time -> 280us+
			// send one  -> high for 620 ns; then low for 930 ns
			// send zero -> high for 300 ns; then low for 930 ns


			if(bit) {
				for(z = 0; z < 440; z++) asm volatile ("nop");
			} else {
				for(z = 0; z < 208; z++) asm volatile ("nop");
			}

			*addr = cond_low;

			if(bit) {
				for(z = 0; z < 648; z++) asm volatile ("nop");
			} else {
				for(z = 0; z < 648; z++) asm volatile ("nop");
			}
		}
	}

	*addr = initial;
}