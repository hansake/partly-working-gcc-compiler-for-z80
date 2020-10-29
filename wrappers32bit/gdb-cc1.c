/* Wrapper to start cc1 from gdb when debugging crashes in cc1.
 * This wrapper is copied after make to the gcc directory of the
 * build directory. First the original cc1 should be renamed
 * to cc1-org
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUILD_DIR change_to_build_directory_path

int debugflag = 1;

int main(int argc, char *argv[]) {
  char command[1024] = "";
  int argidx;
  int sysretval;

  strcat(command, "gdb --args BUILD_DIR/gcc/cc1-org ");
  for (argidx = 1; argidx < argc; argidx++) {
    strcat(command, argv[argidx]);
    strcat(command, " ");
  }
  if (debugflag)
    printf("wrapped command: %s\n", command);
  sysretval = system(command);
  exit(sysretval);
}

