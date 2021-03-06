/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/urandom and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "randcpuid.h"

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  char *error;
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
  unsigned long long (*rand64) (void);
  //void (*initialize) (void);
  //void (*finalize) (void);
  
  void *dl_handle;

  if (rdrand_supported ())
  {
    printf("HW\n");
    dl_handle = dlopen("randlibhw.so", RTLD_LAZY);
    if(!dl_handle){
      printf("dlopen() error - %s\n", dlerror());
      return 1;
    }
    rand64 = dlsym(dl_handle, "hardware_rand64");
    error = dlerror();
    if(error != NULL){
      printf("dlsym rand64 error - %s\n", error);
      return 1;
    }
  } 
  else
  {
    printf("SW\n");
    dl_handle = dlopen("randlibsw.so", RTLD_LAZY);
    if(!dl_handle){
      printf("dlopen() error - %s\n", dlerror());
      return 1;
    }
    rand64 = dlsym(dl_handle, "software_rand64");
    error = dlerror();
    if(error != NULL){
      printf("dlsym rand64 error - %s\n", error);
      return 1;
    }
  }

  //initialize ();
  //int wordsize = sizeof rand64 ();
  int output_errno = 0;

  int temp = nbytes;
  if(nbytes%8 != 0){         // what's the purpose
    temp = (temp/8)+1;
  }
  else{
    temp = temp/8;
  }

  int counter, ret;
  for(counter = 0; counter < temp; counter++){
    unsigned long long x = rand64 ();
    ret = printf("%llu\n", x);
    if(ret < 0){
      output_errno = errno;
      break;
    }
  }

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
  dlclose(dl_handle);
  return 0;
}
