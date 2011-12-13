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

OLDCONFFILE=$1
NEWCONFFILE=src/evilvte.h

cat /dev/null > $NEWCONFFILE
rm -f $2 src/sakura.o

MENU_DEFAULT_ENCODING=`grep MENU_ENCODING_LIST $OLDCONFFILE | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep 'Default Encoding'`
if [ "$MENU_DEFAULT_ENCODING" != "" ]; then
  echo \#define MENU_DEFAULT_ENCODING 1 >> $NEWCONFFILE
fi

COLOR_STYLE_DEFINE=`grep COLOR_STYLE $OLDCONFFILE | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | awk '{print $3}'`
if [ "$COLOR_STYLE_DEFINE" = "VTE_FIXED" ]; then
  echo \#define COLOR_VTE_FIXED 1 >> $NEWCONFFILE
fi
if [ "$COLOR_STYLE_DEFINE" = "LINUX" ]; then
  echo \#define COLOR_LINUX 1 >> $NEWCONFFILE
fi
if [ "$COLOR_STYLE_DEFINE" = "RXVT" ]; then
  echo \#define COLOR_RXVT 1 >> $NEWCONFFILE
fi
if [ "$COLOR_STYLE_DEFINE" = "TANGO" ]; then
  echo \#define COLOR_TANGO 1 >> $NEWCONFFILE
fi
if [ "$COLOR_STYLE_DEFINE" = "XTERM" ]; then
  echo \#define COLOR_XTERM 1 >> $NEWCONFFILE
fi

DEFAULT_TERMINAL_SIZE_DEFINE=`grep DEFAULT_TERMINAL_SIZE $OLDCONFFILE | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`
if [ "$DEFAULT_TERMINAL_SIZE_DEFINE" != "" ]; then
  COLUMNS_DEFINE=`echo $DEFAULT_TERMINAL_SIZE_DEFINE | awk '{print $3}' | cut -d x -f 1`
  ROWS_DEFINE=`echo $DEFAULT_TERMINAL_SIZE_DEFINE | grep x - | cut -d x -f 2`
  if [ "$ROWS_DEFINE" = "" ]; then
    ROWS_DEFINE=24
  fi
  echo \#define VTE_COLUMNS $COLUMNS_DEFINE >> $NEWCONFFILE
  echo \#define VTE_ROWS $ROWS_DEFINE >> $NEWCONFFILE
fi

TOGGLE_BG_ORDER_DEFINE=`grep TOGGLE_BG_ORDER $OLDCONFFILE | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`
if [ "$TOGGLE_BG_ORDER_DEFINE" = "" ]; then
  echo \#define TOGGLE_BG_IMAGE 1 >> $NEWCONFFILE
  echo \#define TOGGLE_BG_TRANSPARENT 1 >> $NEWCONFFILE
  echo \#define TOGGLE_BG_NO_BACKGROUND 1 >> $NEWCONFFILE
  echo \#define TOGGLE_BG_OPACITY 1 >> $NEWCONFFILE
fi

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

HOTKEY_TOGGLE_BACKGROUND_DEFINE=`grep HOTKEY_TOGGLE_BACKGROUND $OLDCONFFILE | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`
if [ "$HOTKEY_TOGGLE_BACKGROUND_DEFINE" != "" ]; then
  if [ "$TOGGLE_BG_ORDER_DEFINE" = "" ]; then
    echo \#undef INIT_OPACITY >> $NEWCONFFILE
    echo \#define INIT_OPACITY 1 >> $NEWCONFFILE
  fi
  if [ "$TOGGLE_BG_OPACITY" != "" ]; then
    echo \#undef INIT_OPACITY >> $NEWCONFFILE
    echo \#define INIT_OPACITY 1 >> $NEWCONFFILE
  fi
fi

MENU_CUSTOM_DEFINE=`grep MENU_CUSTOM $OLDCONFFILE | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1`

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
  if [ "$TOGGLE_BG_ORDER_DEFINE" = "" ]; then
    echo \#undef INIT_OPACITY >> $NEWCONFFILE
    echo \#define INIT_OPACITY 1 >> $NEWCONFFILE
  fi
  if [ "$TOGGLE_BG_OPACITY" != "" ]; then
    echo \#undef INIT_OPACITY >> $NEWCONFFILE
    echo \#define INIT_OPACITY 1 >> $NEWCONFFILE
  fi
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
