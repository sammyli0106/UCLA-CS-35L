#include "randcpuid.h"
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

/* The module is bring linked statically */
/* If the randcpuid reports the hardware supports the RDRAND instruction,
then the main program should link the hardware-oriented implementation of 
randlib. If not, then link the software-oriented implementation. 
The dynamic linking is done through dlopen and dlsym and use dlclose to clean 
up before exiting. Error need to be reported if the main program fails and 
exit with nonzero status (-1) */

static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
        return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
        perror (argv[1]);
      else
        valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the                                
     appropriate library.  */

  /*remove all the initialize and finalize pointers,
    since split into other header files. 
   */
  
  //void (*initialize) (void);
  void *dlhandle;
  char *errstr;
  unsigned long long (*rand64) (void);
  //void (*finalize) (void);
  if (rdrand_supported ())
    {
      /*
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
      */

      //dynamically link the hardware-oriented implementation
      dlhandle = dlopen("./randlibhw.so", RTLD_LAZY);
      if (!dlhandle)
	{
	  /* fail to load the library */
	  fprintf(stderr, "dlopen error : %s.", dlerror());
	  return 1;
	}
      //use dlsym here
      rand64 = dlsym(dlhandle, "rand64");
      //check error for dlsym
      errstr = dlerror();
      if (errstr != NULL)
	{
	  fprintf(stderr, "A dynamic linking error occured with randlibhw : (%s)\n", errstr);
	  return 1;
	}
    }
  else
    {
      /*
      initialize = software_rand64_init;
      rand64 = software_rand64;
      finalize = software_rand64_fini;
      */

      //dynamically link the soft-ware-oriented implementation
      dlhandle = dlopen("./randlibsw.so", RTLD_LAZY);
      if (!dlhandle)
	{
	  fprintf(stderr, "dlopen error : %s.", dlerror());
	  return 1;
	}
      rand64 = dlsym(dlhandle, "rand64");
      /* Correct way to check for error: call dlerror to clear any old error
	 conditions, then call dlsym, and then call dlerror again, save the return
	 value into a variable, check whether the saved value is not NULL */
      
      errstr = dlerror();
      if (errstr != NULL)
	{
	  fprintf(stderr, "A dynamic linking error occured with randlibsw : (%s)\n", errstr);
	  return 1;
	}
    }

  //initialize ();
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
        {
          output_errno = errno;
          break;
        }
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      //finalize ();
      return 1;
    }

  //finalize ();

  //Clean up before exiting
  dlclose(dlhandle);
  
  return 0;
}



