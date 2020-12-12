/*  
 *  cmpe283-1-amd.c - Kernel module for CMPE283 assignment 1
 *  (AMD version)
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

#define MAX_MSG 80

/*
 * struct capability_info
 *
 * Represents a single capability (bit number and description).
 * Used by report_capability to output SVM capabilities.
 */
struct capability_info {
	uint8_t bit;
	const char *name;
};

/*
 * See AMD Programmer Reference Guide Vol 3 Appendix E
 */
struct capability_info svm0a_edx[17] =
{
	{ 0, "SVM Nested Paging" },
	{ 1, "LBR Virtualization" },
	{ 2, "SVM Lock" },
	{ 3, "NRIP save support" },
	{ 4, "TSC Rate Control" },
	{ 5, "VMCB Clean Bits" },
	{ 6, "Flush by ASID" },
	{ 7, "Decode Assists" },
	{ 10, "Pause Intercept Filter" },
	{ 12, "Pause Filter Threshold" },
	{ 13, "Advanced Virtual Interrupt Controller" },
	{ 15, "VMSAVE/VMLOAD Virtualization" },
	{ 16, "Virtualize Global Interrupt Flag" },
	{ 17, "Guest Mode Execution Trap" },
	{ 19, "SVM Supervisor Shadow Stack Check" },
	{ 20, "SPEC_CTRL Virtualization" },
	{ 24, "Suppport for INVLPGB/TLBYSYNC Hypervisor" }
	/*  ... add the rest here ... */
};

struct capability_info svm0a_eax[1] =
{
	{0, "SVM Revision Number"}
};

static inline void
__cpuid(unsigned int *eax, unsigned int *ebx, unsigned int *ecx,
    unsigned int *edx)
{
      asm volatile("cpuid"
            : "=a" (*eax),
              "=b" (*ebx),
              "=c" (*ecx),
              "=d" (*edx)
            : "0" (*eax), "1" (*ebx), "2" (*ecx), "3" (*edx));
}

/*
 * report_capability
 *
 * Reports capabilities present in 'cap'.
 *
 * Parameters:
 *  cap: capability_info structure for this feature
 *  len: number of entries in 'cap'
 *  function: CPUID function number
 *  reg: register to use (0 = eax, 1 = ebx, 2 = ecx, 3 = edx)
 */
void
report_capability(struct capability_info *cap, uint8_t len,
    uint32_t function, uint8_t reg)
{
	uint8_t i;
	uint32_t eax, ebx, ecx, edx, r;
	struct capability_info *c;
	char msg[MAX_MSG];

	memset(msg, 0, sizeof(msg));

	cpuid(function, &eax, &ebx, &ecx, &edx);

	switch (reg) {
	case 0: r = eax; break;
	case 1: r = ebx; break;
	case 2: r = ecx; break;
	case 3: r = edx; break;
	default: return;
	}

	for (i = 0; i < len; i++) {
		c = &cap[i];
		snprintf(msg, 79, "  %s: Can set=%s\n",
		    c->name,
		    (r & (1) << (c->bit)) ? "Yes" : "No");
		printk(msg);
	}
}

void
report_revision_number(
    uint32_t function, uint8_t reg)
{
	uint32_t eax, ebx, ecx, edx, r;
	char msg[MAX_MSG];

	memset(msg, 0, sizeof(msg));

	cpuid(function, &eax, &ebx, &ecx, &edx);

	switch (reg) {
	case 0: r = eax; break;
	case 1: r = ebx; break;
	case 2: r = ecx; break;
	case 3: r = edx; break;
	default: return;
	}
	r = r & 0xff;

	printk("  SVM Revision Number: %x", r);
}

/*
 * detect_svm_features
 *
 * Detects and prints SVM capabilities of this host's CPU.
 */
void
detect_svm_features(void)
{
	printk("SVM Capabilities:\n");
	// report_capability(svm0a_eax,
	// 	1,
	// 	0x8000000A,
	// 	0);
	report_revision_number(0x8000000A, 0);
	report_capability(svm0a_edx,
	    17 /* CHANGE THIS */, 
	    0x8000000A,
	    3); /* 3 == %edx */
}

/*
 * init_module
 *
 * Module entry point
 *
 * Return Values:
 *  Always 0
 */
int
init_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Start\n");

	detect_svm_features();

	unsigned int eax, ebx, ecx, edx;

	eax = 0x4FFFFFFF;
	__cpuid(&eax, &ebx, &ecx, &edx);
	ebx = ebx & lower = some_var & 0xffffffff;
	printk("CPUID(0x4FFFFFFF), exits=%x", ebx);	
	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

/*
 * cleanup_module
 *
 * Function called on module unload
 */
void
cleanup_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Exits\n");
}

