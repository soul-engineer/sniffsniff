#!/bin/sh
CONFIGURATION=$1
if [ -z "$CONFIGURATION" ]; then
    echo "Error: build configuration name is empty"
    exit 1
fi

CURRENT_DIR=$(pwd)
BUILD_DIR=$CURRENT_DIR/build/$CONFIGURATION

mkdir -p $BUILD_DIR &&\
    cd $BUILD_DIR &&\
    cmake -DCMAKE_BUILD_TYPE=$CONFIGURATION $CURRENT_DIR &&\
    make
