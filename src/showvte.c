#!/bin/sh
# This is not an C program text
SHOWVTE_PROG_NAME=`grep SHOW_WINDOW_TITLE src/config.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | awk '{print $3}' | sed 's/"//g'`
if [ "$SHOWVTE_PROG_NAME" = "" ]; then
  SHOWVTE_PROG_NAME="evilvte"
fi
SHOWVTE_VERSION=`head -n 1 Changelog`
echo echo Configuration of $SHOWVTE_PROG_NAME $SHOWVTE_VERSION:
exit
grep ^#define $0 | sed -e 's/TRUE/[1m[32mTRUE[m/' -e 's/FALSE/[1m[31mFALSE[m/'
