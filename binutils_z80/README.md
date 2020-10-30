To create an assembler and other tools for z80 I used the binutils-2.35 distribution:
https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.xz

Uppack the distribution in a suitable directory.

To build it:
```
$ mkdir binutils-z80-build
$ cd binutils-z80-build
$ ../binutils-2.35/configure --prefix=$PWD/../binutils-z80-install --target=z80-elf
$ make all
$ make install
```
Now the tools are available under the binutils-z80-install directory.

The reason for specifying target=z80-elf is that the gcc for z80 compiler
generates assembler directives that the z80-coff assembler does not understand.
