#!/bin/sh

# 此脚本必须在工程根目录下执行

#find . -type f | xargs -n 5 touch

MY_PATH=$(cd `dirname $0`; pwd)
cd $MY_PATH

sh svn_version.sh $1

rm -rf autoscan.log aclocal.m4 config.* configure depcomp install-sh missing ltmain.sh m4 stamp-h1 autom4te.cache lib_log


if [ $# -gt 0 ];then
    if [ $1 == "clean" ]; then
        exit 0;
    fi;
fi;

if [ ! -d Debug ]; then
    mkdir Debug;
fi;

if [ ! -d m4 ]; then
    mkdir m4;
fi;

sh g_makefile.sh $1 > Debug/makefile.am

aclocal -I m4
autoheader
libtoolize --force
automake --add-missing
autoconf

./configure --prefix=`pwd`
if [ $? -ne 0 ]; then
    echo "confiture error;";
    exit -1;
fi;

cd Debug
#make clean;
make -j8 all;
if [ $? -ne 0 ]; then
    echo "make error;";
    exit -1;
fi;
