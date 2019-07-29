#include "randlib.h"
#include <stdio.h>
#include <stdlib.h>

//include stdio.h because of fopen functions
//include stdlib.h because of abort functions

/* contain the software implementation of the random number generator.
Software also needs initialization and finalization, should define an initializer
and a finalizer function, using the gcc constructor and destructor */

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */
//define the initializer function
__attribute__((constructor))

static void
software_rand64_init (void)
{
  urandstream = fopen ("/dev/urandom", "r");
  if (! urandstream)
    abort ();
}

/* Return a random value, using software operations.  */
static unsigned long long
software_rand64 (void)
{
  unsigned long long int x;
  if (fread (&x, sizeof x, 1, urandstream) != 1)
    abort ();
  return x;
}

/* Finalize the software rand64 implementation.  */
//define the finalizer function
__attribute__((destructor))

static void
software_rand64_fini (void)
{
  fclose (urandstream);
}

extern unsigned long long rand64 (void)
{
  return software_rand64();
}
