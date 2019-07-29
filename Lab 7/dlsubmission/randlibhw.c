#include "randlib.h"
#include <immintrin.h>

/* Contains the hardware implementation of the random number generator*/

/* Return a random value, using hardware operations.  */
static unsigned long long
hardware_rand64 (void)
{
  unsigned long long int x;
  while (! _rdrand64_step (&x))
    continue;
  return x;
}

//implement the interface that is described by "randlib.h"
extern unsigned long long rand64 (void)
{
  return hardware_rand64();
}
