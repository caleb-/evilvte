#!/bin/sh
# This is not an C program text
SHOWVTE_PROG_NAME=`grep PROGRAM_NAME src/config.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | awk '{print $3}' | sed 's/"//g'`
if [ "$SHOWVTE_PROG_NAME" = "" ]; then
  SHOWVTE_PROG_NAME="evilvte"
fi
SHOWVTE_PROG_VERSION=`grep PROGRAM_VERSION src/config.h | grep -v ^\/\/ | awk '{print $3}'`
if [ "$SHOWVTE_PROG_VERSION" = "" ]; then
  SHOWVTE_VERSION=`head -n 1 Changelog`
else
  SHOWVTE_VERSION=$SHOWVTE_PROG_VERSION
fi
echo echo Configuration of $SHOWVTE_PROG_NAME $SHOWVTE_VERSION:
exit
grep ^#define $0 | sed -e 's/TRUE/[1m[32mTRUE[m/' -e 's/FALSE/[1m[31mFALSE[m/'
