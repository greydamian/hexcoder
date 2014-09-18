#! /usr/bin/env bash

# Copyright (c) 2014 Damian Jason Lapidge
#
# The contents of this file are subject to the terms and conditions defined 
# within the file LICENSE.txt, located within this project's root directory.

# compiler command
CC="cc";

# archive command
AR="ar";

# source files
SRC="./src/wordspace.c";

# object files - leave empty to auto-generate
OBJ="";

# output file
OUT="./lib/libwordspace.a";

# compiler flags
CFLAGS="";

# include path flags
IFLAGS="-I./include/";

# library path flags
LFLAGS="";

# library flags
LIBS="";

OBJDIR="./obj"; # object file directory - for auto-generation
if [ "$OBJ" == "" ]; then
    # generate list of object files
    for f in $SRC; do
        OBJ="$OBJ $OBJDIR/$(basename $f .c).o"; # append object file
    done
    OBJ="${OBJ:1}"; # remove leading space
fi

if [ $(echo $SRC | wc -w) -ne $(echo $OBJ | wc -w) ]; then
    echo "error: mismatched number of"\
         "source files ($(echo $SRC | wc -w | tr -d ' ')) to"\
         "object files ($(echo $OBJ | wc -w | tr -d ' '))" 1>&2;
    exit 1; # exit failure
fi

# build each object file
for i in $(seq $(echo $SRC | wc -w)); do
    srcfile="$(echo $SRC | cut -d' ' -f$i)";
    objfile="$(echo $OBJ | cut -d' ' -f$i)";

    mkdir -p $(dirname $objfile);
    $CC $CFLAGS $IFLAGS -c -o $objfile $srcfile;
done

# build static library archive
mkdir -p $(dirname $OUT);
$AR rcs $OUT $OBJ;

