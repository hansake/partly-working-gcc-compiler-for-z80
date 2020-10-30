# partly-working-gcc-compiler-for-z80
A not yet really working compiler for z80 built on gcc 2.95.2

This gcc 2.95.2 compiler is patched to generate assembler code for z80.
The patch is made by V.Brahmaiah and send in a mail to the gcc-patches@gcc.gnu.org
list at 25 Jun 2001: http://gcc.gnu.org/ml/gcc-patches/2001-06/msg01546/gcc-2.95.2-z80-patch.gz.

Just out of curiosity I have compiled this code with gcc 9.3.0 on a Linux Mint 20 x86_64 laptop
to have a look at the z80 assembler code that is generated.

If the patched gcc compiler is built as a 32 bit i686 program it does at least not crash.
However the z80 assembler code generates namy instructions that are not valid for z80.
(The code is more valid for Zilog eZ80 but z80-elf-as with the -ez80 flag still gives errors.)

## Scripts for building the compiler

As the building of the compiler is not quite straight forward I created some scripts
for configuration, build (make), install to a temporary directory and simple tests.

The script: ./makeandtest_gcc_z80_i686.sh will build and test the compiler for a 32 bit i686
host that generates code for the z80. The code is however not correct as indicated above.

The script: ./makeandtest_gcc_z80.sh will build and test the z80 compiler for a 64 bit x86-64 host.
It can generate z80 code for some very simple test examples but will crash for almost
every other C code.

The script: ./makeandtest_gcc_i686.sh will build and test the compiler for a 32 bit i686
host that generates code for a 32 bit i686 target. This was created to be able to find
out which workarounds that is needed to be able to build the compiler for a 32 bit i686
host that generates code for the z80.

The other scripts: ./clearall_gcc_z80_i686.sh, ./clearall_gcc_z80.sh and ./clearall_gcc_i686.sh
will remove the created build and install directories and log files.

## Patch of patch

When first trying to build the cross compiler it would always crash in cc1 with signal 11.
The problem seems to be that the variable "base" is not initialized in ../../gcc-2.95.2_z80/gcc/config/z80/z80.c:

      
    case REG:
        return REGNO_OK_FOR_BASE_P2 (REGNO (base), strict);


should most probably be:

      
    case REG:
        return REGNO_OK_FOR_BASE_P2 (REGNO (operand), strict);


copy-paste error from previous line? 

## Future plans maybe...

I will not try to make this version of the z80 compiler generate correct assembler code.
If I ever will make a gcc compiler for z80 I will base it on a version of gcc that is the most recent at that time.
