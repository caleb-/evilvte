#!/bin/sh
# Copyright (C) 2008-2010  Wen-Yen Chuang <caleb AT calno DOT com>
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

NEWCONFFILE=src/install.sh

grep ^#define src/config.o > $NEWCONFFILE
rm -f $1 src/evilvte.o

MENU_DEFAULT_ENCODING=`grep MENU_ENCODING_LIST src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep 'Default Encoding'`
if [ "$MENU_DEFAULT_ENCODING" != "" ]; then
  echo \#define MENU_DEFAULT_ENCODING 1 >> $NEWCONFFILE
fi

MENU_ENCODING_LIST_SIZE=`grep MENU_ENCODING_LIST src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | sed 's/,/\n/g' | wc -l`
TOGGLE_BG_ORDER_SIZE=`grep    TOGGLE_BG_ORDER    src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | sed 's/,/\n/g' | wc -l`
MENU_CUSTOM_SIZE=`grep        MENU_CUSTOM        src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | sed 's/,/\n/g' | wc -l`
echo \#define MENU_ENCODING_LIST_SIZE $MENU_ENCODING_LIST_SIZE              >> $NEWCONFFILE
echo \#define TOGGLE_BG_ORDER_SIZE    $TOGGLE_BG_ORDER_SIZE | grep -v ' 0$' >> $NEWCONFFILE
echo \#define MENU_CUSTOM_SIZE        $MENU_CUSTOM_SIZE                     >> $NEWCONFFILE

DEFAULT_TERMINAL_SIZE_DEFINE=`grep DEFAULT_TERMINAL_SIZE src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`
if [ "$DEFAULT_TERMINAL_SIZE_DEFINE" != "" ]; then
  COLUMNS_DEFINE=`echo $DEFAULT_TERMINAL_SIZE_DEFINE | awk '{print $3}' | cut -d x -f 1`
  ROWS_DEFINE=`echo $DEFAULT_TERMINAL_SIZE_DEFINE | grep x - | cut -d x -f 2`
  if [ "$ROWS_DEFINE" = "" ]; then
    ROWS_DEFINE=24
  fi
  echo \#define VTE_COLUMNS $COLUMNS_DEFINE >> $NEWCONFFILE
  echo \#define VTE_ROWS $ROWS_DEFINE >> $NEWCONFFILE
fi

TOGGLE_BG_ORDER_DEFINE=`grep TOGGLE_BG_ORDER src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`

TOGGLE_BG_IMAGE=`echo $TOGGLE_BG_ORDER_DEFINE | grep Image`
if [ "$TOGGLE_BG_IMAGE" != "" ]; then
  echo \#define TOGGLE_BG_IMAGE 1 >> $NEWCONFFILE
fi

TOGGLE_BG_TRANSPARENT=`echo $TOGGLE_BG_ORDER_DEFINE | grep Transparent`
if [ "$TOGGLE_BG_TRANSPARENT" != "" ]; then
  echo \#define TOGGLE_BG_TRANSPARENT 1 >> $NEWCONFFILE
fi

TOGGLE_BG_NO_BACKGROUND=`echo $TOGGLE_BG_ORDER_DEFINE | grep 'No background'`
if [ "$TOGGLE_BG_NO_BACKGROUND" != "" ]; then
  echo \#define TOGGLE_BG_NO_BACKGROUND 1 >> $NEWCONFFILE
fi

TOGGLE_BG_OPACITY=`echo $TOGGLE_BG_ORDER_DEFINE | grep Opacity`
if [ "$TOGGLE_BG_OPACITY" != "" ]; then
  echo \#define TOGGLE_BG_OPACITY 1 >> $NEWCONFFILE
fi

MENU_CUSTOM_DEFINE=`grep MENU_CUSTOM src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`

MENU_COPY=`echo $MENU_CUSTOM_DEFINE | grep Copy`
if [ "$MENU_COPY" != "" ]; then
  echo \#define MENU_COPY 1 >> $NEWCONFFILE
fi

MENU_PASTE=`echo $MENU_CUSTOM_DEFINE | grep Paste`
if [ "$MENU_PASTE" != "" ]; then
  echo \#define MENU_PASTE 1 >> $NEWCONFFILE
fi

MENU_SELECT_ALL=`echo $MENU_CUSTOM_DEFINE | grep 'Select all'`
if [ "$MENU_SELECT_ALL" != "" ]; then
  echo \#define MENU_SELECT_ALL 1 >> $NEWCONFFILE
fi

MENU_COLOR_BACKGROUND=`echo $MENU_CUSTOM_DEFINE | grep 'Background tint'`
if [ "$MENU_COLOR_BACKGROUND" != "" ]; then
  echo \#define MENU_COLOR_BACKGROUND 1 >> $NEWCONFFILE
fi

MENU_TAB_ADD=`echo $MENU_CUSTOM_DEFINE | grep 'Add tab'`
if [ "$MENU_TAB_ADD" != "" ]; then
  echo \#define MENU_TAB_ADD 1 >> $NEWCONFFILE
fi

MENU_TAB_REMOVE=`echo $MENU_CUSTOM_DEFINE | grep 'Remove tab'`
if [ "$MENU_TAB_REMOVE" != "" ]; then
  echo \#define MENU_TAB_REMOVE 1 >> $NEWCONFFILE
fi

MENU_TAB_EDIT_LABEL=`echo $MENU_CUSTOM_DEFINE | grep 'Edit label'`
if [ "$MENU_TAB_EDIT_LABEL" != "" ]; then
  echo \#define MENU_TAB_EDIT_LABEL 1 >> $NEWCONFFILE
fi

MENU_OPEN_NEW_WINDOW=`echo $MENU_CUSTOM_DEFINE | grep 'New window'`
if [ "$MENU_OPEN_NEW_WINDOW" != "" ]; then
  echo \#define MENU_OPEN_NEW_WINDOW 1 >> $NEWCONFFILE
fi

MENU_QUIT=`echo $MENU_CUSTOM_DEFINE | grep Quit`
if [ "$MENU_QUIT" != "" ]; then
  echo \#define MENU_QUIT 1 >> $NEWCONFFILE
fi

MENU_FONT_BIGGER=`echo $MENU_CUSTOM_DEFINE | grep 'Zoom in'`
if [ "$MENU_FONT_BIGGER" != "" ]; then
  echo \#define MENU_FONT_BIGGER 1 >> $NEWCONFFILE
fi

MENU_FONT_SMALLER=`echo $MENU_CUSTOM_DEFINE | grep 'Zoom out'`
if [ "$MENU_FONT_SMALLER" != "" ]; then
  echo \#define MENU_FONT_SMALLER 1 >> $NEWCONFFILE
fi

MENU_FONT_DEFAULT_SIZE=`echo $MENU_CUSTOM_DEFINE | grep 'Zoom default'`
if [ "$MENU_FONT_DEFAULT_SIZE" != "" ]; then
  echo \#define MENU_FONT_DEFAULT_SIZE 1 >> $NEWCONFFILE
fi

MENU_RESET_TERMINAL=`echo $MENU_CUSTOM_DEFINE | grep 'Reset terminal'`
if [ "$MENU_RESET_TERMINAL" != "" ]; then
  echo \#define MENU_RESET_TERMINAL 1 >> $NEWCONFFILE
fi

MENU_RESET_AND_CLEAR=`echo $MENU_CUSTOM_DEFINE | grep 'Reset and clear'`
if [ "$MENU_RESET_AND_CLEAR" != "" ]; then
  echo \#define MENU_RESET_AND_CLEAR 1 >> $NEWCONFFILE
fi

MENU_FONT_SELECT=`echo $MENU_CUSTOM_DEFINE | grep 'Select font'`
if [ "$MENU_FONT_SELECT" != "" ]; then
  echo \#define MENU_FONT_SELECT 1 >> $NEWCONFFILE
fi

MENU_SEPARATOR=`echo $MENU_CUSTOM_DEFINE | grep Separator`
if [ "$MENU_SEPARATOR" != "" ]; then
  echo \#define MENU_SEPARATOR 1 >> $NEWCONFFILE
fi

SUBMENU_ENCODING_LIST=`echo $MENU_CUSTOM_DEFINE | grep 'Submenu encoding list'`
if [ "$SUBMENU_ENCODING_LIST" != "" ]; then
  echo \#define SUBMENU_ENCODING_LIST 1 >> $NEWCONFFILE
fi

SUBMENU_INPUT_METHOD=`echo $MENU_CUSTOM_DEFINE | grep 'Submenu input method'`
if [ "$SUBMENU_INPUT_METHOD" != "" ]; then
  echo \#define SUBMENU_INPUT_METHOD 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_BACKGROUND=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle background'`
if [ "$MENU_TOGGLE_BACKGROUND" != "" ]; then
  echo \#define MENU_TOGGLE_BACKGROUND 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_HOTKEYS=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle hotkeys locking'`
if [ "$MENU_TOGGLE_HOTKEYS" != "" ]; then
  echo \#define MENU_TOGGLE_HOTKEYS 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_ON_TOP=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle always on top'`
if [ "$MENU_TOGGLE_ON_TOP" != "" ]; then
  echo \#define MENU_TOGGLE_ON_TOP 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_SCROLLBAR=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle scrollbar'`
if [ "$MENU_TOGGLE_SCROLLBAR" != "" ]; then
  echo \#define MENU_TOGGLE_SCROLLBAR 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_STATUS_BAR=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle status bar'`
if [ "$MENU_TOGGLE_STATUS_BAR" != "" ]; then
  echo \#define MENU_TOGGLE_STATUS_BAR 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_TABBAR=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle tabbar'`
if [ "$MENU_TOGGLE_TABBAR" != "" ]; then
  echo \#define MENU_TOGGLE_TABBAR 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_DECORATED=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle window decorated'`
if [ "$MENU_TOGGLE_DECORATED" != "" ]; then
  echo \#define MENU_TOGGLE_DECORATED 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_FULLSCREEN=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle fullscreen'`
if [ "$MENU_TOGGLE_FULLSCREEN" != "" ]; then
  echo \#define MENU_TOGGLE_FULLSCREEN 1 >> $NEWCONFFILE
fi

MENU_TOGGLE_ANTI_ALIAS=`echo $MENU_CUSTOM_DEFINE | grep 'Toggle anti-alias'`
if [ "$MENU_TOGGLE_ANTI_ALIAS" != "" ]; then
  echo \#define MENU_TOGGLE_ANTI_ALIAS 1 >> $NEWCONFFILE
fi

MENU_CHANGE_SATURATION=`echo $MENU_CUSTOM_DEFINE | grep 'Adjust saturation'`
if [ "$MENU_CHANGE_SATURATION" != "" ]; then
  echo \#define MENU_CHANGE_SATURATION 1 >> $NEWCONFFILE
fi

mv $NEWCONFFILE src/evilvte.h

head -n 1 $0 > src/install.sh
make -n install | grep 'install ' >> src/install.sh

  head -n 1 $0 > src/showvte
  tail -n 6 $0 | grep -v ^grep >> src/showvte
  sed 's/\t/ /g' src/custom.h | tr -s ' ' ' ' | sed -e 's/^ //' -e 's~/\*~\n~g' | grep ^\#define >> src/showvte

SHOWVTE_PROG_NAME=`grep PROGRAM_NAME src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | awk '{print $3}' | sed 's/"//g'`
if [ "$SHOWVTE_PROG_NAME" = "" ]; then
  SHOWVTE_PROG_NAME="evilvte"
fi

SHOWVTE_VERSION=`grep PROGRAM_VERSION src/custom.h | grep -v ^\/\/ | awk '{print $3}'`
if [ "$SHOWVTE_VERSION" = "" ]; then
  SHOWVTE_VERSION=`head -n 1 ChangeLog`
fi

echo echo Configuration of $SHOWVTE_PROG_NAME $SHOWVTE_VERSION: >> src/showvte

DEFINE_LINES=`grep ^\#define src/showvte | wc -l`

if [ "$DEFINE_LINES" = "0" ]; then
  echo echo [1m[31mDisable everything.[m >> src/showvte
  echo exit >> src/showvte
fi

  tail -n 6 $0 | grep ^grep >> src/showvte

COMMAND_ENABLED=`grep COMMAND_ src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | grep -v FALSE | grep -v 0 | tail -n 1`
if [ "$COMMAND_ENABLED" = "" ]; then
  grep -v -i option misc/manpage.1 > misc/evilvte.1
  exit
fi

COMMAND_TAB_NUMBERS=`grep    COMMAND_TAB_NUMBERS    src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
TAB_ENABLE=`grep             ' TAB '                src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
if [ "$TAB_ENABLE" = "" ]; then
  COMMAND_TAB_NUMBERS=
fi

COMMAND_AT_ROOT_WINDOW=`grep COMMAND_AT_ROOT_WINDOW src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_DOCK_MODE=`grep      COMMAND_DOCK_MODE      src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_EXEC_PROGRAM=`grep   COMMAND_EXEC_PROGRAM   src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_FULLSCREEN=`grep     COMMAND_FULLSCREEN     src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_LOGIN_SHELL=`grep    COMMAND_LOGIN_SHELL    src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SET_TITLE=`grep      COMMAND_SET_TITLE      src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SHOW_HELP=`grep      COMMAND_SHOW_HELP      src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SHOW_OPTIONS=`grep   COMMAND_SHOW_OPTIONS   src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SHOW_VERSION=`grep   COMMAND_SHOW_VERSION   src/custom.h | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`

head -n 10 misc/manpage.1 > misc/evilvte.1

if [ "$COMMAND_DOCK_MODE" != "" ]; then
  grep '^.\\" D ' misc/manpage.1 | sed 's/^.\\" D //' >> misc/evilvte.1
fi

if [ "$COMMAND_EXEC_PROGRAM" != "" ]; then
  grep '^.\\" E ' misc/manpage.1 | sed 's/^.\\" E //' >> misc/evilvte.1
fi

if [ "$COMMAND_FULLSCREEN" != "" ]; then
  grep '^.\\" F ' misc/manpage.1 | sed 's/^.\\" F //' >> misc/evilvte.1
fi

if [ "$COMMAND_SHOW_HELP" != "" ]; then
  grep '^.\\" H ' misc/manpage.1 | sed 's/^.\\" H //' >> misc/evilvte.1
fi

if [ "$COMMAND_LOGIN_SHELL" != "" ]; then
  grep '^.\\" L ' misc/manpage.1 | sed 's/^.\\" L //' >> misc/evilvte.1
fi

if [ "$COMMAND_SHOW_OPTIONS" != "" ]; then
  grep '^.\\" O ' misc/manpage.1 | sed 's/^.\\" O //' >> misc/evilvte.1
fi

if [ "$COMMAND_AT_ROOT_WINDOW" != "" ]; then
  grep '^.\\" R ' misc/manpage.1 | sed 's/^.\\" R //' >> misc/evilvte.1
fi

if [ "$COMMAND_SET_TITLE" != "" ]; then
  grep '^.\\" T ' misc/manpage.1 | sed 's/^.\\" T //' >> misc/evilvte.1
fi

if [ "$COMMAND_SHOW_VERSION" != "" ]; then
  grep '^.\\" V ' misc/manpage.1 | sed 's/^.\\" V //' >> misc/evilvte.1
fi

if [ "$COMMAND_TAB_NUMBERS" != "" ]; then
  grep '^.\\" 2 ' misc/manpage.1 | sed 's/^.\\" 2 //' >> misc/evilvte.1
fi

tail -n 4 misc/manpage.1 >> misc/evilvte.1

COMMAND_NUMBERS=`grep '^.B..-' misc/evilvte.1 | wc -l`
if [ "$COMMAND_NUMBERS" = "0" ]; then
  grep -v -i option misc/manpage.1 > misc/evilvte.1
  exit
fi
if [ "$COMMAND_NUMBERS" != "1" ]; then
  sed -e 's/option/options/' -e 's/OPTION/OPTIONS/' misc/evilvte.1 > misc/evilvte.2
  mv misc/evilvte.2 misc/evilvte.1
fi
exit

if [ "$1" = "-v" ]; then
  SHOWVTE_VERSION=`grep ^echo $0 | cut -d : -f 1 | cut -d ' ' -f 5`
  echo showvte, version $SHOWVTE_VERSION
  exit
fi
grep ^#define $0 | sed -e 's/$/[m/' -e 's/"/[1m[32m"/' -e 's/ 0/[1m[32m 0/' -e 's/ 1/[1m[32m 1/' -e 's/ 2/[1m[32m 2/' -e 's/ 3/[1m[32m 3/' -e 's/ 4/[1m[32m 4/' -e 's/ 5/[1m[32m 5/' -e 's/ 6/[1m[32m 6/' -e 's/ 7/[1m[32m 7/' -e 's/ 8/[1m[32m 8/' -e 's/ 9/[1m[32m 9/' -e 's/TRUE/[1m[32mTRUE/' -e 's/FALSE/[1m[31mFALSE/' -e 's/ AUTO/[1m[32m AUTO/' -e 's/ ERASE_TTY/[1m[32m ERASE_TTY/' -e 's/ BLOCK/[1m[32m BLOCK/' -e 's/ IBEAM/[1m[32m IBEAM/' -e 's/ UNDERLINE/[1m[32m UNDERLINE/' -e 's/ BACKSPACE/[1m[32m BACKSPACE/g' -e 's/BACKSPACE_KEY/[mBACKSPACE_KEY/' -e 's/ DELETE/[1m[32m DELETE/g' -e 's/DELETE_KEY/[mDELETE_KEY/' -e 's/ VTE_FIXED/[1m[32m VTE_FIXED/' -e 's/ LINUX/[1m[32m LINUX/' -e 's/ RXVT/[1m[32m RXVT/' -e 's/ TANGO/[1m[32m TANGO/' -e 's/ XTERM/[1m[32m XTERM/' -e 's/ g_/[1m[32m g_/' -e 's/ RIGHT/[1m[32m RIGHT/' -e 's/ LEFT/[1m[32m LEFT/' -e 's/ TOP/[1m[32m TOP/' -e 's/ BOTTOM/[1m[32m BOTTOM/' -e 's/ CTRL /[1m[32m CTRL /'
