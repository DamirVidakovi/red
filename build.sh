#!/bin/zsh

VERBOSE=0
if [[ "$1" == "-v" || "$1" == "--verbose" ]]; then
  VERBOSE=1
  shift
fi

if [[ "$1" == "-h" || "$1" == "--help" ]]; then
  printf "Usage: $0 [-v|--verbose] {build|clean|distclean}\n"
  printf "  -v, --verbose   Print additional information about what the script is doing.\n"
  printf "  build           Build the project.\n"
  printf "  clean           Clean the build directory.\n"
  printf "  distclean       Remove the entire build directory.\n"
  exit 0
fi

ROOT_DIR=$PWD
WORK_DIR=$ROOT_DIR/build

build() {
  if [ ! -d $WORK_DIR ]
  then
    mkdir $WORK_DIR
    if [ $VERBOSE -eq 1 ]; then printf "Created directory $WORK_DIR\n"; fi
  fi

  pushd $WORK_DIR > /dev/null
    if [ $VERBOSE -eq 1 ]; then printf "Changed directory to $WORK_DIR\n"; fi
    cmake $ROOT_DIR
    if [ $VERBOSE -eq 1 ]; then printf "Ran cmake with root directory $ROOT_DIR\n"; fi
    make -j4 -l4
    if [ $VERBOSE -eq 1 ]; then printf "Ran make\n"; fi
  popd > /dev/null

  printf "Done\n"
}

clean() {
  pushd $WORK_DIR > /dev/null
    make clean
    if [ $VERBOSE -eq 1 ]; then printf "Ran make clean\n"; fi
  popd > /dev/null

  printf "Clean\n"
}

distclean() {
  rm -rf $WORK_DIR
  if [ $VERBOSE -eq 1 ]; then printf "Removed directory $WORK_DIR\n"; fi

  printf "DistClean\n"
}

"$@"

unset ROOT_DIR
unset WORK_DIR