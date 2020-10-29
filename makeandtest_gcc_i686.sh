#!/bin/bash

# Configure and build gcc 32 bit compiler 
#
# Paths where to put and find things
BASE_DIR=$PWD
BUILD_TYPE=gcc_i686
SOURCE_DIR=$BASE_DIR/gcc-2.95.2_z80
BUILD_DIR=$BASE_DIR/$BUILD_TYPE-build
INSTALL_DIR=$BASE_DIR/$BUILD_TYPE-install
CONFIGURE_LOG=$BASE_DIR/$BUILD_TYPE-configure.log
BUILD_LOG=$BASE_DIR/$BUILD_TYPE-build.log
INSTALL_LOG=$BASE_DIR/$BUILD_TYPE-install.log
TEST_LOG=$BASE_DIR/$BUILD_TYPE-test.log
# Test that all needed packages are installed
NEEDED_PACKAGES="autoconf gcc linux-libc-dev-i386-cross gcc-multilib"
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
# Build the "wrappers" needed to build the compiler as a 32 bit program
cd wrappers32bit
make
# copy the gcc32 wrapper to ~/bin if this directory exists
if [ -d $HOME/bin ]; then
  cp gcc32 $HOME/bin
else
  echo "The directory $HOME/bin must be created and included in \$PATH"
  echo "before this script is run"
  exit 1
fi
cd $BASE_DIR
# Configure the build if file "$BUILD_DIR/is_configured" does not exist
#   Note: --host=i686... is really cheating but the configure script
#   does not understand x86_64.
mkdir -p $BUILD_DIR
cd $BUILD_DIR
echo "$BUILD_TYPE is built from: $SOURCE_DIR"
if [ ! -f $BUILD_DIR/is_configured ]; then
  echo "Confgure $BUILD_TYPE build"
  echo "  see $CONFIGURE_LOG for result"
  CC="gcc32" \
  CFLAGS="-g -std=gnu89" \
  LDFLAGS="-m32" \
  $SOURCE_DIR/configure --host=i686-pc-linux-gnu \
  --target=i686-pc-linux-gnu --prefix=$INSTALL_DIR \
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
# Copy the as wrapper to the gcc build directory
cp $BASE_DIR/wrappers32bit/as $BUILD_DIR/gcc
# Fake that documentation is already built
touch gcc/cpp.info gcc/gcc.info gcc/lang.info
# Build the z80 cross compiler
echo "Build $BUILD_TYPE with make"
echo "  see $BUILD_LOG for result"
LIBRARY_PATH=/usr/lib32:$LIBRARY_PATH \
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
# Test the compiler with some simple programs
echo "Test to compile some simple programs to object code"
echo "  with $INSTALL_DIR/bin/i686-pc-linux-gnu-gcc"
echo "  see $TEST_LOG for result"
cd ../test_programs
rm -f *.s *.o
TEST_PROGRAMS=( "$(echo simple*.c hello.c)" )
echo "Test the compiler" $INSTALL_DIR/bin/i686-pc-linux-gnu-gcc > $TEST_LOG
for cprogram in $TEST_PROGRAMS; do
  echo "  compiling $cprogram"
  echo "Compiling $cprogram" >> $TEST_LOG
  $INSTALL_DIR/bin/i686-pc-linux-gnu-gcc -Wa,"--32" -c $cprogram  >> $TEST_LOG 2>&1
  if [ $? -ne 0 ]; then
    echo "    error while compiling $cprogram"
  else
    echo "    $cprogram compiled without errors"
  fi
done
