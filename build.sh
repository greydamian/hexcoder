#! /usr/bin/env bash

CC="cc";

SRC="src/hexcoder.c";
OUT="bin/hexcoder";

LIBSRC="./libsrc";
LIBDIR="./lib";

CFLAGS="";
IFLAGS="-I./include";
LFLAGS="-L./lib";

LIBS="-lwordspace -lgreyio";

ROOTDIR="$(pwd)";

# build each library
mkdir -p $LIBDIR;
for d in $(ls $LIBSRC);
do
    cd $LIBSRC/$d;
    bash ./build.sh;
    cp ./lib/* $ROOTDIR/$LIBDIR;
    rm -r ./obj; rm -r ./lib;
    cd $ROOTDIR;
done

mkdir -p $(dirname $OUT);
$CC $CFLAGS -o $OUT $SRC $IFLAGS $LFLAGS $LIBS;

