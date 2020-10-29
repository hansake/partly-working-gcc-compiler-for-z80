# partly-working-gcc-compiler-for-z80
A not yet really working compiler for z80 built on gcc 2.95.2

This gcc 2.95.2 compiler is patched to generate assembler code for z80.
The patch is made by V.Brahmaiah and send in a mail to the gcc-patches@gcc.gnu.org
mail list at 25 Jun 2001.

Just out of curiosity I have compiled this code with gcc 9.3.0
on a Linux Mint x86_64 laptop.

If the patched gcc compiler is built as a 32 bit i386 program it does at least not crash.
However the z80 assembler code generates namy instructions that are not valid for z80.
(The code is more valid for Zilog eZ80 but z80-elf-as with the -ez80 flag still gives errors.)
