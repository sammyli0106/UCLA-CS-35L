#include "randcpuid.h"
#include <cpuid.h>

/*Should contain the code that determine whether the current
  CPU has the RDRAND instruction.*/

/* Hardware implementation.  */

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

//Does not need to export the cpuid function, so the function should be static
/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
         "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}

//Since the main function is determining whether it supports RDRAND instruction
//So function should be extern
/* Return true if the CPU supports the RDRAND instruction.  */
extern _Bool
rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}



