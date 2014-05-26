#! /usr/bin/env bash

# compiler command
CC="cc";

# archive command
AR="ar";

# source files directory
SRCDIR="./src/";

# source files
SRC="greyio.c";

# object files directory
OBJDIR="./obj/";

# object files - leave empty to auto generate
OBJ="";

# output file
OUT="./lib/libgreyio.a";

# compiler flags
CFLAGS="";

# include path flags
IFLAGS="-I./include/";

# library path flags
LFLAGS="";

# library flags
LIBS="";

function prepend {
    result="";
    for e in $1;
    do
        result="$result $2$e";
    done
    result="${result:1}"; # remove leading space
    echo "$result";
}

if [ "$OBJ" == "" ];
then
    # generate list of object files
    for f in $SRC;
    do
        OBJ="$OBJ $(basename $f .c).o"; # append file name, .c replaced with .o
    done
    OBJ="${OBJ:1}";
fi

# prepend directories to file names
SRC=$(prepend "$SRC" "$SRCDIR");
OBJ=$(prepend "$OBJ" "$OBJDIR");

SRCARR=($SRC);
OBJARR=($OBJ);

if [ ${#SRCARR[@]} -ne ${#OBJARR[@]} ];
then
    echo "error: mismatched number of source files (${#SRCARR[@]})"\
         "to object files (${#OBJARR[@]})" 1>&2;
    exit 1; # exit failure
fi

# build each object file
mkdir -p $OBJDIR;
for i in ${!SRCARR[@]};
do
    $CC $CFLAGS $IFLAGS -c -o ${OBJARR[$i]} ${SRCARR[$i]};
done

# build static library archive
mkdir -p $(dirname $OUT);
$AR rcs $OUT $OBJ;

