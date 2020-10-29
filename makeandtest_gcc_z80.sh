#!/bin/bash

# Configure and build gcc cross compiler 
#
# Paths where to put and find things
BASE_DIR=$PWD
BUILD_TYPE=gcc_z80
SOURCE_DIR=$BASE_DIR/gcc-2.95.2_z80
BUILD_DIR=$BASE_DIR/$BUILD_TYPE-build
INSTALL_DIR=$BASE_DIR/$BUILD_TYPE-install
CONFIGURE_LOG=$BASE_DIR/$BUILD_TYPE-configure.log
BUILD_LOG=$BASE_DIR/$BUILD_TYPE-build.log
INSTALL_LOG=$BASE_DIR/$BUILD_TYPE-install.log
TEST_LOG=$BASE_DIR/$BUILD_TYPE-test.log
# The path to the assembler is temporary for now
Z80_ASSEMBLER=/home/hal/sourcecode/binutils/binutils-z80-install/bin/z80-elf-as
# Test that all needed packages are installed
NEEDED_PACKAGES="autoconf gcc"
echo "Test if needad packages are installed"
let "pkg_missing=0"
for package in $NEEDED_PACKAGES; do
  dpkg-query -W $package
  let "pkg_missing=$pkg_missing+$?"
done
if [ $pkg_missing -ne 0 ]; then
  echo "Needed packages missing"
  exit 1
else
  echo "Needed packages installed"
fi
mkdir -p $BUILD_DIR
cd $BUILD_DIR
# Configure the build if file "$BUILD_DIR/is_configured" does not exist
#   Note: --host=i686... is really cheating but the configure script
#   does not understand x86_64.
echo "$BUILD_TYPE is built from: $SOURCE_DIR"
if [ ! -f $BUILD_DIR/is_configured ]; then
  echo "Confgure $BUILD_TYPE build"
  echo "  see $CONFIGURE_LOG for result"
  CFLAGS="-g -std=gnu89" \
  $SOURCE_DIR/configure --host=i686-pc-linux-gnu \
  --target=z80-zilog-linux --prefix=$INSTALL_DIR \
  --with-as=$Z80_ASSEMBLER \
  --enable-languages="c" > $CONFIGURE_LOG 2>&1
  if [ $? -ne 0 ]; then
    echo "  Configure reported errors"
  else
    echo "  Configure succeded without errors"
  fi
  touch $BUILD_DIR/is_configured
else
  echo "Already configured, to reconfigure remove file $BUILD_DIR/is_configured"
fi
# Build the z80 cross compiler
echo "Build $BUILD_TYPE with make"
echo "  see $BUILD_LOG for result"
make all > $BUILD_LOG 2>&1
if [ $? -ne 0 ]; then
  echo "  Build reported errors"
else
  echo "  Build succeded without errors"
fi
# Install the z80 cross compiler
echo "Install $BUILD_TYPE"
echo "  see $INSTALL_LOG for result"
make -i install > $INSTALL_LOG 2>&1
if [ $? -ne 0 ]; then
  echo "  Install reported errors"
else
  echo "  Install succeded without errors"
fi
# Test the z80 cross compiler with some simple programs
echo "Test to compile some simple programs to assembly code"
echo "  see $TEST_LOG for result"
cd ../test_programs
rm -f *.s *.o
TEST_PROGRAMS=( "$(echo simple*.c)" )
echo "Test the compiler" $INSTALL_DIR/bin/z80-zilog-linux-gcc > $TEST_LOG
for cprogram in $TEST_PROGRAMS; do
  echo "  compiling $cprogram"
  echo "Compiling $cprogram" >> $TEST_LOG
  $INSTALL_DIR/bin/z80-zilog-linux-gcc -S $cprogram  >> $TEST_LOG 2>&1
  if [ $? -ne 0 ]; then
    echo "    error while compiling $cprogram"
  else
    echo "    $cprogram compiled without errors"
  fi
done
