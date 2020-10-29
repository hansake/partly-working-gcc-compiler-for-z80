/* Wrapper function to add the flag --32 to the assembler
 * as gcc 2.95.2 does not understand the -m32 flag and as for x86-64
 * needs it to understand the 32 bit assembler output from gcc 2.95.2.
 * Should be copied to the gcc directiory in the build directory
 * after configuration but before make.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int debugflag = 1;

int main(int argc, char *argv[]) {
  char command[1024] = "";
  int argidx;
  int sysretval;

  strcat(command, "/usr/bin/as --32 ");
  for (argidx = 1; argidx < argc; argidx++) {
    strcat(command, argv[argidx]);
    strcat(command, " ");
  }
  if (debugflag)
    printf("wrapped command: %s\n", command);
  sysretval = system(command);
  exit(sysretval);
}

