#!/bin/sh
# Copyright (C) 2008-2009  Wen-Yen Chuang <caleb AT calno DOT com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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

if [ "$EVILVTE" = "min" ]; then
  echo echo [1m[31mDisable everything.[m
  echo exit
fi

exit

if [ "$1" = "-v" ]; then
  SHOWVTE_VERSION=`grep ^echo $0 | cut -d : -f 1 | cut -d ' ' -f 5`
  echo showvte, version $SHOWVTE_VERSION
  exit
fi

grep ^#define $0 | sed -e 's/TRUE/[1m[32mTRUE[m/' -e 's/FALSE/[1m[31mFALSE[m/'
