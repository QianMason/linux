#include <stdio.h>
#include <stdint.h>

int main() {
	uint32_t reg[4];

	__asm__("mov $0x4ffffffe, %eax");
	__asm__("mov $0x72, %ecx");
	__asm__("cpuid;");
	__asm__("mov %%eax, %0;":"=r"(reg[0]));
	__asm__("mov %%ebx, %0;":"=r"(reg[1]));
	__asm__("mov %%ecx, %0;":"=r"(reg[2]));

	uint64_t cycles = (((uint64_t) reg[1]<<32) | (uint64_t) reg[2]);

	//printf("exit count = %d,  cycle count = %ld\n",reg[0],cycles);
	printf("eax = %d, ebx = %d, ecx = %d, edx = %d", reg[0], reg[1], reg[2], reg[3]);
	return 0;
}