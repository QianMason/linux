/*
 * Assignment 2 test program
 *
 * cc -Wall -Wextra -o test_assignment2 test_assignment2.c
 */

#include <stdio.h>

#include <sys/types.h>

static inline void
__cpuid(u_int64_t *eax, u_int64_t *ebx, u_int64_t *ecx,
    u_int64_t *edx)
{
      asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "1" (*ebx), "2" (*ecx), "3" (*edx));
}

int
main(int argc, char **argv)
{
	u_int64_t eax, ebx, ecx, edx;

	eax = 0x4FFFFFFF;
	__cpuid(&eax, &ebx, &ecx, &edx);
	ebx = ebx & 0xffffffff;
	ecx = ecx >> 32;
	printf("CPUID(0x4FFFFFFF), exits=%lu, exits spent in process=%lu\n", ebx, ecx);	
}