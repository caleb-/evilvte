#!/bin/sh
# Copyright (C) 2008-2011  Wen-Yen Chuang <caleb AT calno DOT com>
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

CONFFILE=src/install.sh

grep ^#define src/config.o > $CONFFILE
rm -f $1 src/evilvte.o

MENU_DEFAULT_ENCODING=`grep   MENU_ENCODING_LIST src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep 'Default Encoding'`
MENU_ENCODING_LIST_SIZE=`grep MENU_ENCODING_LIST src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | tr ',' '\n' | grep '"' | wc -l`
TOGGLE_BG_ORDER_SIZE=`grep    TOGGLE_BG_ORDER    src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | tr ',' '\n' | grep '"' | wc -l`
MENU_CUSTOM_SIZE=`grep        MENU_CUSTOM        src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | tr ',' '\n' | grep '"' | wc -l`
UPPER_PROGRAM_NAME=`grep      PROGRAM_NAME       src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | cut -d '"' -f 2 | sed 's/\b\(.\)/\u\1/'`
[ "$MENU_DEFAULT_ENCODING" != "" ]    && echo \#define MENU_DEFAULT_ENCODING   1                        >> $CONFFILE
[ "$MENU_ENCODING_LIST_SIZE" != "0" ] && echo \#define MENU_ENCODING_LIST_SIZE $MENU_ENCODING_LIST_SIZE >> $CONFFILE
[ "$TOGGLE_BG_ORDER_SIZE" != "0" ]    && echo \#define TOGGLE_BG_ORDER_SIZE    $TOGGLE_BG_ORDER_SIZE    >> $CONFFILE
[ "$MENU_CUSTOM_SIZE" != "0" ]        && echo \#define MENU_CUSTOM_SIZE        $MENU_CUSTOM_SIZE        >> $CONFFILE
[ "$UPPER_PROGRAM_NAME" != "" ]       && echo \#define UPPER_PROGRAM_NAME   '"'$UPPER_PROGRAM_NAME'"'   >> $CONFFILE

DEFAULT_TERMINAL_SIZE_DEFINE=`grep DEFAULT_TERMINAL_SIZE src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1`
if [ "$DEFAULT_TERMINAL_SIZE_DEFINE" != "" ]; then
  COLUMNS_DEFINE=`echo $DEFAULT_TERMINAL_SIZE_DEFINE | awk '{print $3}' | cut -d x -f 1`
  ROWS_DEFINE=`echo $DEFAULT_TERMINAL_SIZE_DEFINE | grep x | cut -d x -f 2`
  [ "$ROWS_DEFINE" = "" ] && ROWS_DEFINE=24
  echo \#define VTE_COLUMNS $COLUMNS_DEFINE >> $CONFFILE
  echo \#define VTE_ROWS $ROWS_DEFINE >> $CONFFILE
fi

TOGGLE_BG_ORDER_DEFINE=`grep   TOGGLE_BG_ORDER src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1`
TOGGLE_BG_IMAGE=`echo         $TOGGLE_BG_ORDER_DEFINE | grep  Image`
TOGGLE_BG_TRANSPARENT=`echo   $TOGGLE_BG_ORDER_DEFINE | grep  Transparent`
TOGGLE_BG_NO_BACKGROUND=`echo $TOGGLE_BG_ORDER_DEFINE | grep 'No background'`
TOGGLE_BG_OPACITY=`echo       $TOGGLE_BG_ORDER_DEFINE | grep  Opacity`
[ "$TOGGLE_BG_IMAGE" != "" ]         && echo \#define TOGGLE_BG_IMAGE         1 >> $CONFFILE
[ "$TOGGLE_BG_TRANSPARENT" != "" ]   && echo \#define TOGGLE_BG_TRANSPARENT   1 >> $CONFFILE
[ "$TOGGLE_BG_NO_BACKGROUND" != "" ] && echo \#define TOGGLE_BG_NO_BACKGROUND 1 >> $CONFFILE
[ "$TOGGLE_BG_OPACITY" != "" ]       && echo \#define TOGGLE_BG_OPACITY       1 >> $CONFFILE

MENU_CUSTOM_DEFINE=`grep MENU_CUSTOM src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1`

MENU_COPY=`echo $MENU_CUSTOM_DEFINE | grep Copy`
[ "$MENU_COPY" != "" ] && echo \#define MENU_COPY 1 >> $CONFFILE

MENU_PASTE=`echo $MENU_CUSTOM_DEFINE | grep Paste`
[ "$MENU_PASTE" != "" ] && echo \#define MENU_PASTE 1 >> $CONFFILE

MENU_SELECT_ALL=`echo $MENU_CUSTOM_DEFINE | grep 'Select all'`
[ "$MENU_SELECT_ALL" != "" ] && echo \#define MENU_SELECT_ALL 1 >> $CONFFILE

MENU_COLOR_BACKGROUND=`echo $MENU_CUSTOM_DEFINE | grep 'Background tint'`
[ "$MENU_COLOR_BACKGROUND" != "" ] && echo \#define MENU_COLOR_BACKGROUND 1 >> $CONFFILE

MENU_TAB_ADD=`echo $MENU_CUSTOM_DEFINE | grep 'Add tab'`
[ "$MENU_TAB_ADD" != "" ] && echo \#define MENU_TAB_ADD 1 >> $CONFFILE

MENU_TAB_REMOVE=`echo $MENU_CUSTOM_DEFINE | grep 'Remove tab'`
[ "$MENU_TAB_REMOVE" != "" ] && echo \#define MENU_TAB_REMOVE 1 >> $CONFFILE

MENU_TAB_EDIT_LABEL=`echo $MENU_CUSTOM_DEFINE | grep 'Edit label'`
[ "$MENU_TAB_EDIT_LABEL" != "" ] && echo \#define MENU_TAB_EDIT_LABEL 1 >> $CONFFILE

MENU_OPEN_NEW_WINDOW=`echo $MENU_CUSTOM_DEFINE | grep 'New window'`
[ "$MENU_OPEN_NEW_WINDOW" != "" ] && echo \#define MENU_OPEN_NEW_WINDOW 1 >> $CONFFILE

MENU_QUIT=`echo $MENU_CUSTOM_DEFINE | grep Quit`
[ "$MENU_QUIT" != "" ] && echo \#define MENU_QUIT 1 >> $CONFFILE

MENU_FONT_BIGGER=`echo $MENU_CUSTOM_DEFINE | grep 'Zoom in'`
[ "$MENU_FONT_BIGGER" != "" ] && echo \#define MENU_FONT_BIGGER 1 >> $CONFFILE

MENU_FONT_SMALLER=`echo $MENU_CUSTOM_DEFINE | grep 'Zoom out'`
[ "$MENU_FONT_SMALLER" != "" ] && echo \#define MENU_FONT_SMALLER 1 >> $CONFFILE

MENU_FONT_DEFAULT_SIZE=`echo $MENU_CUSTOM_DEFINE | grep 'Zoom default'`
[ "$MENU_FONT_DEFAULT_SIZE" != "" ] && echo \#define MENU_FONT_DEFAULT_SIZE 1 >> $CONFFILE

MENU_RESET_TERMINAL=`echo $MENU_CUSTOM_DEFINE | grep 'Reset terminal'`
[ "$MENU_RESET_TERMINAL" != "" ] && echo \#define MENU_RESET_TERMINAL 1 >> $CONFFILE

MENU_RESET_AND_CLEAR=`echo $MENU_CUSTOM_DEFINE | grep 'Reset and clear'`
[ "$MENU_RESET_AND_CLEAR" != "" ] && echo \#define MENU_RESET_AND_CLEAR 1 >> $CONFFILE

MENU_FONT_SELECT=`echo $MENU_CUSTOM_DEFINE | grep 'Select font'`
[ "$MENU_FONT_SELECT" != "" ] && echo \#define MENU_FONT_SELECT 1 >> $CONFFILE

MENU_SEPARATOR=`echo $MENU_CUSTOM_DEFINE | grep Separator`
[ "$MENU_SEPARATOR" != "" ] && echo \#define MENU_SEPARATOR 1 >> $CONFFILE

SUBMENU_ENCODING_LIST=`echo $MENU_CUSTOM_DEFINE | grep 'Submenu encoding list'`
[ "$SUBMENU_ENCODING_LIST" != "" ] && echo \#define SUBMENU_ENCODING_LIST 1 >> $CONFFILE

SUBMENU_INPUT_METHOD=`echo $MENU_CUSTOM_DEFINE | grep 'Submenu input method'`
[ "$SUBMENU_INPUT_METHOD" != "" ] && echo \#define SUBMENU_INPUT_METHOD 1 >> $CONFFILE

MENU_TOGGLE_BACKGROUND=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle background'`
[ "$MENU_TOGGLE_BACKGROUND" != "" ] && echo \#define MENU_TOGGLE_BACKGROUND 1 >> $CONFFILE

MENU_TOGGLE_HOTKEYS=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle hotkeys locking'`
[ "$MENU_TOGGLE_HOTKEYS" != "" ] && echo \#define MENU_TOGGLE_HOTKEYS 1 >> $CONFFILE

MENU_TOGGLE_ON_TOP=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle always on top'`
[ "$MENU_TOGGLE_ON_TOP" != "" ] && echo \#define MENU_TOGGLE_ON_TOP 1 >> $CONFFILE

MENU_TOGGLE_SCROLLBAR=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle scrollbar'`
[ "$MENU_TOGGLE_SCROLLBAR" != "" ] && echo \#define MENU_TOGGLE_SCROLLBAR 1 >> $CONFFILE

MENU_TOGGLE_STATUS_BAR=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle status bar'`
[ "$MENU_TOGGLE_STATUS_BAR" != "" ] && echo \#define MENU_TOGGLE_STATUS_BAR 1 >> $CONFFILE

MENU_TOGGLE_TABBAR=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle tabbar'`
[ "$MENU_TOGGLE_TABBAR" != "" ] && echo \#define MENU_TOGGLE_TABBAR 1 >> $CONFFILE

MENU_TOGGLE_DECORATED=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle window decorated'`
[ "$MENU_TOGGLE_DECORATED" != "" ] && echo \#define MENU_TOGGLE_DECORATED 1 >> $CONFFILE

MENU_TOGGLE_FULLSCREEN=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle fullscreen'`
[ "$MENU_TOGGLE_FULLSCREEN" != "" ] && echo \#define MENU_TOGGLE_FULLSCREEN 1 >> $CONFFILE

MENU_TOGGLE_ANTI_ALIAS=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle anti-alias'`
[ "$MENU_TOGGLE_ANTI_ALIAS" != "" ] && echo \#define MENU_TOGGLE_ANTI_ALIAS 1 >> $CONFFILE

MENU_CHANGE_SATURATION=`echo $MENU_CUSTOM_DEFINE | grep 'Adjust saturation'`
[ "$MENU_CHANGE_SATURATION" != "" ] && echo \#define MENU_CHANGE_SATURATION 1 >> $CONFFILE

ONLY_ONE_MENU_ITEM=`echo $MENU_CUSTOM_DEFINE | grep '"Auto-click"'`
[ "$ONLY_ONE_MENU_ITEM" != "" ] && echo \#define ONLY_ONE_MENU_ITEM >> $CONFFILE

mv $CONFFILE src/evilvte.h

head -n 1 $0 > src/install.sh
echo "set -x" >> src/install.sh
make -n install | grep 'install ' >> src/install.sh

head -n 1 $0 > src/showvte
tail -n 6 $0 | grep -v ^grep >> src/showvte
cat src/custom.h | tr '\t' ' ' | tr -s ' ' ' ' | sed -e 's/^ //' -e 's~/\*~\n~g' | grep ^\#define >> src/showvte

SHOWVTE_PROG_NAME=`grep PROGRAM_NAME    src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | cut -d '"' -f 2`
[ "$SHOWVTE_PROG_NAME" = "" ] && SHOWVTE_PROG_NAME="evilvte"

SHOWVTE_VERSION=`grep   PROGRAM_VERSION src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | cut -d '"' -f 2`
[ "$SHOWVTE_VERSION" = "" ] && SHOWVTE_VERSION=`head -n 1 ChangeLog`

echo echo Configuration of $SHOWVTE_PROG_NAME $SHOWVTE_VERSION: >> src/showvte

DEFINE_LINES=`grep ^\#define src/showvte | wc -l`

if [ "$DEFINE_LINES" = "0" ]; then
  echo echo [1m[31mDisable everything.[m >> src/showvte
  echo exit >> src/showvte
fi

tail -n 6 $0 | grep ^grep >> src/showvte

COMMAND_AT_ROOT_WINDOW=`grep COMMAND_AT_ROOT_WINDOW src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_DOCK_MODE=`grep      COMMAND_DOCK_MODE      src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_EXEC_PROGRAM=`grep   COMMAND_EXEC_PROGRAM   src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_FULLSCREEN=`grep     COMMAND_FULLSCREEN     src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_FONT=`grep           COMMAND_FONT           src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_GEOMETRY=`grep       COMMAND_GEOMETRY       src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_LOGIN_SHELL=`grep    COMMAND_LOGIN_SHELL    src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_SET_TITLE=`grep      COMMAND_SET_TITLE      src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_SHOW_HELP=`grep      COMMAND_SHOW_HELP      src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_SHOW_OPTIONS=`grep   COMMAND_SHOW_OPTIONS   src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_SHOW_VERSION=`grep   COMMAND_SHOW_VERSION   src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
COMMAND_TAB_NUMBERS=`grep    COMMAND_TAB_NUMBERS    src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`
PROGRAM_WM_CLASS=`grep       PROGRAM_WM_CLASS       src/custom.h | tr '\t' ' ' | sed 's/^\( \)*//g' | grep -v ^\/\/ | tail -n 1 | grep -v -i FALSE | grep -v 0`

head -n 10 misc/manpage.1 > misc/evilvte.1

[ "$COMMAND_DOCK_MODE" != "" ]      && grep '^.\\" D ' misc/manpage.1 | sed 's/^.\\" D //' >> misc/evilvte.1
[ "$COMMAND_EXEC_PROGRAM" != "" ]   && grep '^.\\" E ' misc/manpage.1 | sed 's/^.\\" E //' >> misc/evilvte.1
[ "$COMMAND_FULLSCREEN" != "" ]     && grep '^.\\" F ' misc/manpage.1 | sed 's/^.\\" F //' >> misc/evilvte.1
[ "$COMMAND_FONT" != "" ]           && grep '^.\\" 7 ' misc/manpage.1 | sed 's/^.\\" 7 //' >> misc/evilvte.1
[ "$COMMAND_GEOMETRY" != "" ]       && grep '^.\\" G ' misc/manpage.1 | sed 's/^.\\" G //' >> misc/evilvte.1
[ "$COMMAND_SHOW_HELP" != "" ]      && grep '^.\\" H ' misc/manpage.1 | sed 's/^.\\" H //' >> misc/evilvte.1
[ "$COMMAND_LOGIN_SHELL" != "" ]    && grep '^.\\" L ' misc/manpage.1 | sed 's/^.\\" L //' >> misc/evilvte.1
[ "$COMMAND_SHOW_OPTIONS" != "" ]   && grep '^.\\" O ' misc/manpage.1 | sed 's/^.\\" O //' >> misc/evilvte.1
[ "$COMMAND_AT_ROOT_WINDOW" != "" ] && grep '^.\\" R ' misc/manpage.1 | sed 's/^.\\" R //' >> misc/evilvte.1
[ "$COMMAND_SET_TITLE" != "" ]      && grep '^.\\" T ' misc/manpage.1 | sed 's/^.\\" T //' >> misc/evilvte.1
[ "$COMMAND_SHOW_VERSION" != "" ]   && grep '^.\\" V ' misc/manpage.1 | sed 's/^.\\" V //' >> misc/evilvte.1
[ "$COMMAND_TAB_NUMBERS" != "" ]    && grep '^.\\" 2 ' misc/manpage.1 | sed 's/^.\\" 2 //' >> misc/evilvte.1

COMMAND_NUMBERS=`grep '^.B..-' misc/evilvte.1 | wc -l`

grep '^.\\" 6 ' misc/manpage.1 | sed 's/^.\\" 6 //' >> misc/evilvte.1

tail -n 8 misc/manpage.1 >> misc/evilvte.1

[ "$COMMAND_NUMBERS" = "0" ] && grep -v OPTION$ misc/manpage.1 | sed 's/^.\\" 6 //' > misc/evilvte.1
if [ "$COMMAND_NUMBERS" != "0" ] && [ "$COMMAND_NUMBERS" != "1" ]; then
  sed 's/SH OPTION$/SH OPTIONS/' misc/evilvte.1 > misc/evilvte.2
  mv misc/evilvte.2 misc/evilvte.1
fi
if [ "$PROGRAM_WM_CLASS" != "" ]; then
  sed 's/^.\\" 8 //' misc/evilvte.1 > misc/evilvte.2
  mv misc/evilvte.2 misc/evilvte.1
fi
exit

if [ "$1" = "-v" ]; then
  SHOWVTE_VERSION=`grep ^echo $0 | cut -d : -f 1 | cut -d ' ' -f 5`
  echo showvte, version $SHOWVTE_VERSION
  exit
fi
grep ^#define $0 | sed -e 's/$/[m/' -e 's/"/[1m[32m"/' -e 's/ 0/[1m[32m 0/' -e 's/ 1/[1m[32m 1/' -e 's/ 2/[1m[32m 2/' -e 's/ 3/[1m[32m 3/' -e 's/ 4/[1m[32m 4/' -e 's/ 5/[1m[32m 5/' -e 's/ 6/[1m[32m 6/' -e 's/ 7/[1m[32m 7/' -e 's/ 8/[1m[32m 8/' -e 's/ 9/[1m[32m 9/' -e 's/TRUE/[1m[32mTRUE/' -e 's/FALSE/[1m[31mFALSE/' -e 's/ AUTO/[1m[32m AUTO/' -e 's/ ERASE_TTY/[1m[32m ERASE_TTY/' -e 's/ BLOCK/[1m[32m BLOCK/' -e 's/ IBEAM/[1m[32m IBEAM/' -e 's/ UNDERLINE/[1m[32m UNDERLINE/' -e 's/ BACKSPACE/[1m[32m BACKSPACE/g' -e 's/BACKSPACE_KEY/[mBACKSPACE_KEY/' -e 's/ DELETE/[1m[32m DELETE/g' -e 's/DELETE_KEY/[mDELETE_KEY/' -e 's/ VTE_FIXED/[1m[32m VTE_FIXED/' -e 's/ LINUX/[1m[32m LINUX/' -e 's/ RXVT/[1m[32m RXVT/' -e 's/ TANGO/[1m[32m TANGO/' -e 's/ XTERM/[1m[32m XTERM/' -e 's/ ZENBURN/[1m[32m ZENBURN/' -e 's/ g_/[1m[32m g_/' -e 's/ RIGHT/[1m[32m RIGHT/' -e 's/ LEFT/[1m[32m LEFT/' -e 's/ OFF_R/[1m[32m OFF_R/' -e 's/ OFF_L/[1m[32m OFF_L/' -e 's/ TOP/[1m[32m TOP/' -e 's/ BOTTOM/[1m[32m BOTTOM/' -e 's/ CTRL /[1m[32m CTRL /'
