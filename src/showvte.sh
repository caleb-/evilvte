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

  head -n 1 $0 > src/showvte
  tail -n 6 $0 | grep -v ^grep >> src/showvte
  sed 's/\t/ /g' src/custom.h | tr -s ' ' ' ' | sed -e 's/^ //' -e 's~/\*~\n~g' | grep ^\#define >> src/showvte

SHOWVTE_PROG_NAME=`grep PROGRAM_NAME src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | awk '{print $3}' | sed 's/"//g'`
if [ "$SHOWVTE_PROG_NAME" = "" ]; then
  SHOWVTE_PROG_NAME="evilvte"
fi

SHOWVTE_VERSION=`grep PROGRAM_VERSION src/custom.h | grep -v ^\/\/ | awk '{print $3}'`
if [ "$SHOWVTE_VERSION" = "" ]; then
  SHOWVTE_VERSION=`head -n 1 Changelog`
fi

echo echo Configuration of $SHOWVTE_PROG_NAME $SHOWVTE_VERSION: >> src/showvte

DEFINE_LINES=`grep ^\#define src/showvte | wc | awk '{print $1}'`

if [ "$DEFINE_LINES" = "0" ]; then
  echo echo [1m[31mDisable everything.[m >> src/showvte
  echo exit >> src/showvte
fi

  grep ^grep $0 >> src/showvte
  chmod 755 src/showvte
  exit

if [ "$1" = "-v" ]; then
  SHOWVTE_VERSION=`grep ^echo $0 | cut -d : -f 1 | cut -d ' ' -f 5`
  echo showvte, version $SHOWVTE_VERSION
  exit
fi
grep ^#define $0 | sed -e 's/$/[m/' -e 's/"/[1m[32m"/' -e 's/ 0/[1m[32m 0/' -e 's/ 1/[1m[32m 1/' -e 's/ 2/[1m[32m 2/' -e 's/ 3/[1m[32m 3/' -e 's/ 4/[1m[32m 4/' -e 's/ 5/[1m[32m 5/' -e 's/ 6/[1m[32m 6/' -e 's/ 7/[1m[32m 7/' -e 's/ 8/[1m[32m 8/' -e 's/ 9/[1m[32m 9/' -e 's/TRUE/[1m[32mTRUE/' -e 's/FALSE/[1m[31mFALSE/' -e 's/ AUTO/[1m[32m AUTO/' -e 's/ BACKSPACE/[1m[32m BACKSPACE/g' -e 's/BACKSPACE_KEY/[mBACKSPACE_KEY/' -e 's/ DELETE/[1m[32m DELETE/g' -e 's/DELETE_KEY/[mDELETE_KEY/' -e 's/ VTE_FIXED/[1m[32m VTE_FIXED/' -e 's/ LINUX/[1m[32m LINUX/' -e 's/ RXVT/[1m[32m RXVT/' -e 's/ TANGO/[1m[32m TANGO/' -e 's/ XTERM/[1m[32m XTERM/' -e 's/ g_/[1m[32m g_/' -e 's/ RIGHT/[1m[32m RIGHT/' -e 's/ LEFT/[1m[32m LEFT/' -e 's/ TOP/[1m[32m TOP/' -e 's/ BOTTOM/[1m[32m BOTTOM/' -e 's/ CTRL /[1m[32m CTRL /'
