#! /usr/bin/env bash

# Copyright (c) 2014 Damian Jason Lapidge
#
# The contents of this file are subject to the terms and conditions defined 
# within the file LICENSE.txt, located within this project's root directory.

CC="cc";

SRC="src/hexcoder.c";
OUT="bin/hexcoder";

CFLAGS="";
IFLAGS="-I./include";
LFLAGS="-L./lib";

LIBS="-lwordspace -lgreyio";

LIBSRC="./libsrc";
LIBDIR="./lib";

ROOTDIR="$(pwd)";

# build each library
mkdir -p $LIBDIR;
for d in $(ls $LIBSRC); do
    cd $LIBSRC/$d;
    bash ./build.sh;
    cp ./lib/* $ROOTDIR/$LIBDIR;
    rm -r ./obj; rm -r ./lib;
    cd $ROOTDIR;
done

mkdir -p $(dirname $OUT);
$CC $CFLAGS -o $OUT $SRC $IFLAGS $LFLAGS $LIBS;

