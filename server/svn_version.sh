#!/bin/sh

# 此脚本必须在工程根目录下执行

MY_PATH=$(cd `dirname $0`; pwd)
cd $MY_PATH

export LC_ALL=en_US.UTF-8

TMP_FILE="svn_info.h.tmp"
OUT_FILE="svn_info.h"
cd ..
REV=`svn info | grep -i "Last Changed Rev"`
URL=`svn info | grep -i "^URL"`
if [ -z "$REV" ];then
  REV=`svn info | grep -i "最后修改的版本"`
fi;
  
cd $MY_PATH

if [ -z "$REV" ];then
  #local_ip="`ifconfig eth0 |grep "inet addr"| cut -f 2 -d ":"|cut -f 1 -d " "`"
  REV=`svn info | grep -i "Last Changed Rev"`
  URL=`svn info | grep -i "^URL"`
  if [ -z "$REV" ];then
    REV=`svn info | grep -i "最后修改的版本"`
  fi;
  if [ -z "$REV" ];then
    REV="no version."
  fi;
  BUILD=`echo "Builder Info:"$1 `
#  BUILD=$BUILD`echo ", user:"`
else
  BUILD=`echo "Builder Info:"$1 `
#  BUILD=$BUILD`echo ", user:"`
fi;

REV=${REV/Last Changed /}
URL=${URL/https:\/\/jd02\/svn\/JD02\//}
URL=${URL/https:\/\/jd02\/svn\/JD02\//}
URL=${URL/\/project/}
URL=${URL/\/src/}

local_ip="`ifconfig eth0 |grep "inet addr"| cut -f 2 -d ":"|cut -f 1 -d " "`"
if [ -z "$local_ip" ];then
	local_ip="`ifconfig em1 |grep "inet addr"| cut -f 2 -d ":"|cut -f 1 -d " "`"
fi

#BUILD=$BUILD`echo ~`
#BUILD=$BUILD`echo "  hostname:"``hostname`
BUILD=$BUILD`echo "  ip:"`$local_ip

echo "
#ifndef SVN_INFO_H_
#define SVN_INFO_H_

#define URL \"$URL\"
#define REV \"$REV\"
#define BUILD \"$BUILD\" 

#endif

" > $TMP_FILE

if [ -f "$OUT_FILE" ]
then
	cmp_res=`cmp $OUT_FILE $TMP_FILE`
	if [ -n cmp_res ]
	then
		\mv "$TMP_FILE" "$OUT_FILE"	
	else
		rm -f "$TMP_FILE"
	fi
else
	\mv "$TMP_FILE" "$OUT_FILE"
fi


