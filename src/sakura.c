/***************************************************************************** *
 *  Filename: sakura.c
 *  Description: VTE-based terminal emulator
 *
 *           Copyright (C) 2006-2008  David Gómez <david@pleyades.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****************************************************************************/

/* Copyright (C) 2008  Wen-Yen Chuang <caleb AT calno DOT com>
 *
 * Forked from sakura 2.0.1, http://www.pleyades.net/david/sakura.php
 */

#include <gdk/gdkkeysyms.h>
#include <string.h>
#include <vte/vte.h>

#define AUTO            VTE_ERASE_AUTO
#define BACKSPACE       VTE_ERASE_ASCII_BACKSPACE
#define DELETE          VTE_ERASE_ASCII_DELETE
#define DELETE_SEQUENCE VTE_ERASE_DELETE_SEQUENCE

#define VTE_FIXED color_vte_fixed
#define LINUX     color_linux
#define RXVT      color_rxvt
#define TANGO     color_tango
#define XTERM     color_xterm

#define LEFT   0
#define RIGHT  1
#define TOP    2
#define BOTTOM 3

#include "config.h"
#include "evilvte.h"

#ifndef DEFAULT_COMMAND
#define DEFAULT_COMMAND g_getenv("SHELL")
#endif

#ifndef DEFAULT_ARGV
#define DEFAULT_ARGV NULL
#endif

#ifndef DEFAULT_ENVV
#define DEFAULT_ENVV NULL
#endif

#ifndef DEFAULT_DIRECTORY
#define DEFAULT_DIRECTORY g_get_current_dir()
#endif

#ifndef RECORD_LASTLOG
#define RECORD_LASTLOG 1
#endif

#ifndef RECORD_UTMP
#define RECORD_UTMP 1
#endif

#ifndef RECORD_WTMP
#define RECORD_WTMP 1
#endif

#ifndef LABEL_DEFAULT_ENCODING
#define LABEL_DEFAULT_ENCODING "Default Encoding"
#endif

#ifndef LABEL_DIALOG_BACKGROUND_TINT
#define LABEL_DIALOG_BACKGROUND_TINT "Background tint color"
#endif

#ifndef LABEL_DIALOG_CLOSE
#define LABEL_DIALOG_CLOSE "Do you really want to close it?"
#endif

#ifndef LABEL_DIALOG_SATURATION
#define LABEL_DIALOG_SATURATION "Saturation level"
#endif

#ifndef LABEL_MENU_SATURATION
#define LABEL_MENU_SATURATION "Adjust saturation"
#endif

#ifndef LABEL_MENU_TOGGLE_ANTI_ALIAS
#define LABEL_MENU_TOGGLE_ANTI_ALIAS "Toggle anti-alias"
#endif

#ifndef LABEL_MENU_TOGGLE_BG
#define LABEL_MENU_TOGGLE_BG "Toggle background"
#endif

#ifndef LABEL_MENU_TOGGLE_DECORATED
#define LABEL_MENU_TOGGLE_DECORATED "Toggle window decorated"
#endif

#ifndef LABEL_MENU_TOGGLE_FULLSCREEN
#define LABEL_MENU_TOGGLE_FULLSCREEN "Toggle fullscreen"
#endif

#ifndef LABEL_MENU_TOGGLE_HOTKEYS
#define LABEL_MENU_TOGGLE_HOTKEYS "Toggle hotkeys locking"
#endif

#ifndef LABEL_MENU_TOGGLE_ON_TOP
#define LABEL_MENU_TOGGLE_ON_TOP "Toggle always on top"
#endif

#ifndef LABEL_MENU_TOGGLE_SCROLLBAR
#define LABEL_MENU_TOGGLE_SCROLLBAR "Toggle scrollbar"
#endif

#ifndef LABEL_MENU_TOGGLE_STATUS_BAR
#define LABEL_MENU_TOGGLE_STATUS_BAR "Toggle status bar"
#endif

#ifndef LABEL_MENU_TOGGLE_TABBAR
#define LABEL_MENU_TOGGLE_TABBAR "Toggle tabbar"
#endif

#ifndef LABEL_SUBMENU_ENCODING
#define LABEL_SUBMENU_ENCODING "Character Encoding"
#endif

#ifndef LABEL_SUBMENU_IME
#define LABEL_SUBMENU_IME "Input Methods"
#endif

#if SCROLLBAR_LEFT
#undef SCROLLBAR_RIGHT
#endif

#if CLOSE_DIALOG
#define GET_VTE_CHILD_PID 1
#define DO_CLOSE_DIALOG 1
#endif

#if !CLOSE_DIALOG
#define DO_CLOSE_DIALOG 0
#endif

#if CLOSE_SAFELY
#undef GET_VTE_CHILD_PID
#define GET_VTE_CHILD_PID 1
#endif

#if !TAB
#undef CTRL_TAB_ADD
#undef CTRL_TAB_REMOVE
#undef CTRL_TAB_PREVIOUS
#undef CTRL_TAB_NEXT
#undef CTRL_TAB_FIRST
#undef CTRL_TAB_LAST
#undef CTRL_TAB_GO_TO_NUMBER
#undef CTRL_TAB_EDIT_LABEL
#undef CTRL_TOGGLE_TABBAR
#undef MENU_TAB_ADD
#undef MENU_TAB_REMOVE
#undef MENU_TAB_EDIT_LABEL
#undef MENU_TOGGLE_TABBAR
#undef SHOW_WINDOW_BORDER
#undef TAB_BORDER
#undef TAB_BORDER_HORIZONTAL
#undef TAB_BORDER_VERTICAL
#undef TAB_EXPANDED_WIDTH
#undef COMMAND_TAB_NUMBERS
#undef TAB_LABEL
#undef TAB_LABEL_POEM
#undef TAB_LABEL_CUSTOM
#undef TAB_NEW_PATH_EQUAL_OLD
#undef TAB_REORDERABLE
#undef TAB_SHOW_INFO_AT_TITLE
#undef TABBAR
#undef TABBAR_PLACE
#undef TABBAR_AUTOHIDE
#undef TABBAR_SCROLLABLE
#undef TABBAR_MENU_SELECT_TAB
#define SHOW_WINDOW_BORDER 0
#endif

#if TAB_NEW_PATH_EQUAL_OLD
#undef GET_VTE_CHILD_PID
#define GET_VTE_CHILD_PID 1
#define VTE_DEFAULT_DIRECTORY default_directory
#endif

#if !TAB_NEW_PATH_EQUAL_OLD
#define VTE_DEFAULT_DIRECTORY DEFAULT_DIRECTORY
#endif

#if !CTRL
#undef CTRL_COPY
#undef CTRL_PASTE
#undef CTRL_SELECT_ALL
#undef CTRL_COLOR_BACKGROUND
#undef CTRL_EDIT_ENCODING
#undef CTRL_FONT_BIGGER
#undef CTRL_FONT_SMALLER
#undef CTRL_FONT_DEFAULT_SIZE
#undef CTRL_FONT_SELECT
#undef CTRL_OPEN_NEW_WINDOW
#undef CTRL_RESET_TERMINAL
#undef CTRL_RESET_AND_CLEAR
#undef CTRL_SATURATION_DIALOG
#undef CTRL_SATURATION_MORE
#undef CTRL_SATURATION_LESS
#undef CTRL_TAB_ADD
#undef CTRL_TAB_REMOVE
#undef CTRL_TAB_PREVIOUS
#undef CTRL_TAB_NEXT
#undef CTRL_TAB_FIRST
#undef CTRL_TAB_LAST
#undef CTRL_TAB_GO_TO_NUMBER
#undef CTRL_TAB_EDIT_LABEL
#undef CTRL_TOGGLE_ANTI_ALIAS
#undef CTRL_TOGGLE_DECORATED
#undef CTRL_TOGGLE_FULLSCREEN
#undef CTRL_TOGGLE_HOTKEYS
#undef CTRL_TOGGLE_ON_TOP
#undef CTRL_TOGGLE_SCROLLBAR
#undef CTRL_TOGGLE_STATUS_BAR
#undef CTRL_TOGGLE_TABBAR
#undef CTRL_TOGGLE_BACKGROUND
#endif

#ifdef CTRL_TOGGLE_BACKGROUND
#ifndef BACKGROUND_IMAGE
#define BACKGROUND_IMAGE ".config/evilvte/background.png"
#endif
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#ifndef TOGGLE_BG_ORDER
#define TOGGLE_BG_ORDER "Image", "Transparent", "No background", "Opacity"
#endif
#define DO_TOGGLE_BACKGROUND 1
static char *background_order[] = { TOGGLE_BG_ORDER };
static int background_order_size = sizeof(background_order) / sizeof(background_order[0]);
int background_status = 0;
#endif

#if BUTTON_ORDER_BY_RCFILE
int button_order = 0;
#endif

#if MOUSE_CTRL_SATURATION
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#endif

#ifdef CTRL_SATURATION_DIALOG
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#define DO_CHANGE_SATURATION 1
GtkWidget *adjustment;
#endif

#if BACKGROUND_OPACITY
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#define INIT_OPACITY 1
#endif

#define MENU_INPUT_METHOD 1

#if !MENU
#undef MENU_INPUT_METHOD
#undef MENU_ENCODING_DEFAULT
#undef MENU_ENCODING_LIST
#undef MENU_MATCH_STRING_EXEC
#undef MATCH_STRING
#undef MENU_CUSTOM
#endif

#ifndef MENU_CUSTOM
#undef MENU_COPY
#undef MENU_PASTE
#undef MENU_SELECT_ALL
#undef MENU_COLOR_BACKGROUND
#undef MENU_TAB_ADD
#undef MENU_TAB_REMOVE
#undef MENU_TAB_EDIT_LABEL
#undef MENU_OPEN_NEW_WINDOW
#undef MENU_QUIT
#undef MENU_FONT_BIGGER
#undef MENU_FONT_SMALLER
#undef MENU_FONT_DEFAULT_SIZE
#undef MENU_RESET_TERMINAL
#undef MENU_RESET_AND_CLEAR
#undef MENU_FONT_SELECT
#undef MENU_SEPARATOR
#undef SUBMENU_ENCODING_LIST
#undef SUBMENU_INPUT_METHOD
#undef MENU_TOGGLE_BACKGROUND
#undef MENU_TOGGLE_HOTKEYS
#undef MENU_TOGGLE_ON_TOP
#undef MENU_TOGGLE_SCROLLBAR
#undef MENU_TOGGLE_STATUS_BAR
#undef MENU_TOGGLE_TABBAR
#undef MENU_TOGGLE_DECORATED
#undef MENU_TOGGLE_FULLSCREEN
#undef MENU_TOGGLE_ANTI_ALIAS
#undef MENU_CHANGE_SATURATION
#endif

#if MENU_TAB_EDIT_LABEL
#define DO_TAB_EDIT_LABEL 1
#endif

#ifdef CTRL_TAB_EDIT_LABEL
#ifndef DO_TAB_EDIT_LABEL
#define DO_TAB_EDIT_LABEL 1
#endif
#endif

#ifdef CTRL_TOGGLE_ON_TOP
#ifndef PROGRAM_ALWAYS_ON_TOP
#define PROGRAM_ALWAYS_ON_TOP 0
#endif
#endif

#if MENU_TOGGLE_ON_TOP
#ifndef PROGRAM_ALWAYS_ON_TOP
#define PROGRAM_ALWAYS_ON_TOP 0
#endif
#endif

#if MENU_CHANGE_SATURATION
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#ifndef CTRL_SATURATION_DIALOG
GtkWidget *adjustment;
#endif
#define DO_CHANGE_SATURATION 1
#endif

#ifdef CTRL_TOGGLE_HOTKEYS
#define DO_TOGGLE_HOTKEYS 1
#endif

#if MENU_TOGGLE_HOTKEYS
#undef DO_TOGGLE_HOTKEYS
#define DO_TOGGLE_HOTKEYS 1
#endif

#if DO_TOGGLE_HOTKEYS
int hotkey_status = 0;
#endif

#ifdef MENU_ENCODING_LIST
#if MENU_DEFAULT_ENCODING
char *encoding[] = {
#endif
#if !MENU_DEFAULT_ENCODING
static char *encoding[] = {
#endif
MENU_ENCODING_LIST
};
#endif

#ifndef MENU_ENCODING_LIST
#if MENU
#define MENU_ENCODING_DEFAULT 1
GtkWidget *encoding_item;
#endif
#endif

#if MENU_TOGGLE_BACKGROUND
#ifndef BACKGROUND_IMAGE
#define BACKGROUND_IMAGE ".config/evilvte/background.png"
#endif
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#ifndef TOGGLE_BG_ORDER
#define TOGGLE_BG_ORDER "Image", "Transparent", "No background", "Opacity"
#endif
#ifndef CTRL_TOGGLE_BACKGROUND
#define DO_TOGGLE_BACKGROUND 1
static char *background_order[] = { TOGGLE_BG_ORDER };
static int background_order_size = sizeof(background_order) / sizeof(background_order[0]);
int background_status = 0;
#endif
#endif

#ifdef BACKGROUND_IMAGE
#define BACKGROUND_EXIST 1
char imgstr[64];
#endif

#ifdef BACKGROUND_TRANSPARENT
#undef BACKGROUND_EXIST
#define BACKGROUND_EXIST 1
#endif

#ifdef CTRL_COLOR_BACKGROUND
#ifndef BACKGROUND_TINT_COLOR
#define BACKGROUND_TINT_COLOR "black"
#endif
#endif

#if MENU_COLOR_BACKGROUND
#ifndef BACKGROUND_TINT_COLOR
#define BACKGROUND_TINT_COLOR "black"
#endif
#endif

#ifdef BACKGROUND_TINT_COLOR
#if BACKGROUND_EXIST
GdkColor color_tint;
#endif
#endif

#if PROGRAM_WM_CLASS
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#if MENU_OPEN_NEW_WINDOW
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#ifdef CTRL_TOGGLE_SCROLLBAR
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
#define DO_TOGGLE_SCROLLBAR 1
int scrollbar_status = 0;
#endif
#endif

#if MENU_TOGGLE_SCROLLBAR
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
#ifndef CTRL_TOGGLE_SCROLLBAR
#define DO_TOGGLE_SCROLLBAR 1
int scrollbar_status = 0;
#endif
#endif
#endif

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
#define VTE_HBOX term->hbox
#endif

#if !SCROLLBAR_LEFT && !SCROLLBAR_RIGHT
#define VTE_HBOX term->vte
#endif

#ifdef CTRL_TOGGLE_STATUS_BAR
#if STATUS_BAR
int status_bar_status = 0;
#endif
#endif

#if MENU_TOGGLE_STATUS_BAR && STATUS_BAR
#ifndef CTRL_TOGGLE_STATUS_BAR
int status_bar_status = 0;
#endif
#endif

#ifdef CTRL_TOGGLE_DECORATED
#ifdef SHOW_WINDOW_DECORATED
int window_decorated_status = !SHOW_WINDOW_DECORATED;
#endif
#ifndef SHOW_WINDOW_DECORATED
int window_decorated_status = 0;
#endif
#endif

#if MENU_TOGGLE_DECORATED
#ifndef CTRL_TOGGLE_DECORATED
#ifdef SHOW_WINDOW_DECORATED
int window_decorated_status = !SHOW_WINDOW_DECORATED;
#endif
#ifndef SHOW_WINDOW_DECORATED
int window_decorated_status = 0;
#endif
#endif
#endif

#ifdef CTRL_TOGGLE_FULLSCREEN
int window_fullscreen_status = 0;
#endif

#if MENU_TOGGLE_FULLSCREEN
#ifndef CTRL_TOGGLE_FULLSCREEN
int window_fullscreen_status = 0;
#endif
#endif

#ifdef CTRL_TOGGLE_ANTI_ALIAS
#define INT_ANTI_ALIAS_STATUS 1
#endif

#if MENU_TOGGLE_ANTI_ALIAS
#undef INT_ANTI_ALIAS_STATUS
#define INT_ANTI_ALIAS_STATUS 1
#endif

#if INT_ANTI_ALIAS_STATUS
#ifdef FONT_ANTI_ALIAS
#if FONT_ANTI_ALIAS
int antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#endif
#if !FONT_ANTI_ALIAS
int antialias_status = VTE_ANTI_ALIAS_FORCE_DISABLE;
#endif
#endif
#endif

#if INT_ANTI_ALIAS_STATUS
#ifndef FONT_ANTI_ALIAS
int antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#endif
#endif

#ifdef MENU_CUSTOM
static char *menu_custom[] = { MENU_CUSTOM };
int menu_item_success = 0;
#endif

#ifdef CTRL_TOGGLE_TABBAR
#define INT_TABBAR_STATUS 1
#endif

#if MENU_TOGGLE_TABBAR
#undef INT_TABBAR_STATUS
#define INT_TABBAR_STATUS 1
#endif

#ifdef TABBAR
#if !TABBAR && !INT_TABBAR_STATUS
#undef SHOW_WINDOW_BORDER
#define SHOW_WINDOW_BORDER 0
#endif
#endif

#if TABBAR_AUTOHIDE
#ifndef TABBAR
#define TABBAR 1
#endif
#endif

#if INT_TABBAR_STATUS
#ifdef TABBAR
int tabbar_status = !TABBAR;
#endif
#ifndef TABBAR
int tabbar_status = 0;
#endif
#define VTE_TABBAR !tabbar_status
#endif

#if !INT_TABBAR_STATUS
#if TABBAR
#define VTE_TABBAR 1
#endif
#if !TABBAR
#define VTE_TABBAR 0
#endif
#endif

#ifdef MENU_MATCH_STRING_EXEC
#ifndef MATCH_STRING
#define MATCH_STRING "((f|F)(t|T)(p|P)|((h|H)(t|T)(t|T)(p|P)(s|S)*))://[-a-zA-Z0-9.?$%&/=_~#.,:;+]*"
#endif
#endif

#ifdef FONT_ANTI_ALIAS
#if FONT_ANTI_ALIAS
#define VTE_ANTI_ALIAS VTE_ANTI_ALIAS_FORCE_ENABLE
#endif
#if !FONT_ANTI_ALIAS
#define VTE_ANTI_ALIAS VTE_ANTI_ALIAS_FORCE_DISABLE
#endif
#endif

#ifndef FONT_ANTI_ALIAS
#define VTE_ANTI_ALIAS VTE_ANTI_ALIAS_USE_DEFAULT
#endif

#if TAB_SHOW_INFO_AT_TITLE
#undef SHOW_WINDOW_TITLE
#define SHOW_WINDOW_TITLE 1
#endif

#if SHOW_WINDOW_TITLE
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#define COMMAND_SHOW_HELP_OPTION_S ""

#if COMMAND_SHOW_OPTIONS
#undef COMMAND_SHOW_HELP_OPTION_S
#define COMMAND_SHOW_HELP_OPTION_S "s"
#endif

#if COMMAND_TAB_NUMBERS
#undef COMMAND_SHOW_HELP_OPTION_S
#define COMMAND_SHOW_HELP_OPTION_S "s"
#endif

#if COMMAND_SHOW_VERSION
#undef COMMAND_SHOW_HELP_OPTION_S
#define COMMAND_SHOW_HELP_OPTION_S "s"
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#if COMMAND_SHOW_HELP
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#ifdef PROGRAM_VERSION
#define EVILVTE_PROGRAM_VERSION PROGRAM_VERSION
#endif
#ifndef PROGRAM_VERSION
#define EVILVTE_PROGRAM_VERSION EVILVTE_VERSION
#endif

#ifdef CTRL_OPEN_NEW_WINDOW
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#if COMMAND_SET_TITLE
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#define VTE_PROGRAM_NAME program_name
char program_name[64];
#endif

#if !COMMAND_SET_TITLE
#define VTE_PROGRAM_NAME PROGRAM_NAME
#endif

#ifdef COLOR_BACKGROUND
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_TEXT_BOLD
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef CURSOR_COLOR
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_TEXT_DIM
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_FOREGROUND
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_TEXT_HIGHLIGHTED
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_STYLE
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 0
#endif

GtkWidget *main_window;
GtkWidget *notebook;

#if SUSE_DETECTED
#ifndef TAB_LABEL
#define TAB_LABEL "Page"
#endif
#endif

#if TAB_CLOSE_BUTTON
#ifndef TAB_LABEL
#define TAB_LABEL "Page"
#endif
#define VTE_LABEL term->label
#endif

#ifdef TAB_LABEL_CUSTOM
#undef TAB_LABEL
#undef TAB_LABEL_POEM
#define TAB_LABEL_INIT 1
static char *label_style_custom[] = { TAB_LABEL_CUSTOM };
static int label_style_size = sizeof(label_style_custom) / sizeof(label_style_custom[0]);
#endif

#if TAB_LABEL_POEM
#undef TAB_LABEL
#define TAB_LABEL_INIT 1
/* Taken from http://zh.wikipedia.org/wiki/%E5%8D%83%E5%AD%97%E6%96%87 */
static char *label_style_poem[] = {
"天", "地", "玄", "黃", "宇", "宙", "洪", "荒",
"日", "月", "盈", "昃", "辰", "宿", "列", "張",
"寒", "來", "暑", "往", "秋", "收", "冬", "藏",
"閏", "餘", "成", "歲", "律", "呂", "調", "陽"
};
static int label_style_size = sizeof(label_style_poem) / sizeof(label_style_poem[0]);
#endif

#ifdef TAB_LABEL
#define TAB_LABEL_INIT 1
#endif

#if !TAB_CLOSE_BUTTON
#if TAB_LABEL_INIT
#define VTE_LABEL label
#endif
#if !TAB_LABEL_INIT
#define VTE_LABEL NULL
#endif
#endif

#ifndef TAB_LABEL
#undef TAB_LABEL_NUMBER
#endif

#if COLOR_VTE_FIXED
const GdkColor color_vte_fixed[16] =
{
  { 0, 0x0000, 0x0000, 0x0000 },
  { 0, 0xc0c0, 0x0000, 0x0000 },
  { 0, 0x0000, 0xc0c0, 0x0000 },
  { 0, 0xc0c0, 0xc0c0, 0x0000 },
  { 0, 0x0000, 0x0000, 0xc0c0 },
  { 0, 0xc0c0, 0x0000, 0xc0c0 },
  { 0, 0x0000, 0xc0c0, 0xc0c0 },
  { 0, 0xc0c0, 0xc0c0, 0xc0c0 },
  { 0, 0x3f3f, 0x3f3f, 0x3f3f },
  { 0, 0xffff, 0x3f3f, 0x3f3f },
  { 0, 0x3f3f, 0xffff, 0x3f3f },
  { 0, 0xffff, 0xffff, 0x3f3f },
  { 0, 0x3f3f, 0x3f3f, 0xffff },
  { 0, 0xffff, 0x3f3f, 0xffff },
  { 0, 0x3f3f, 0xffff, 0xffff },
  { 0, 0xffff, 0xffff, 0xffff }
};
#endif

#if COLOR_LINUX
const GdkColor color_linux[16] =
{
  { 0, 0x0000, 0x0000, 0x0000 },
  { 0, 0xaaaa, 0x0000, 0x0000 },
  { 0, 0x0000, 0xaaaa, 0x0000 },
  { 0, 0xaaaa, 0x5555, 0x0000 },
  { 0, 0x0000, 0x0000, 0xaaaa },
  { 0, 0xaaaa, 0x0000, 0xaaaa },
  { 0, 0x0000, 0xaaaa, 0xaaaa },
  { 0, 0xaaaa, 0xaaaa, 0xaaaa },
  { 0, 0x5555, 0x5555, 0x5555 },
  { 0, 0xffff, 0x5555, 0x5555 },
  { 0, 0x5555, 0xffff, 0x5555 },
  { 0, 0xffff, 0xffff, 0x5555 },
  { 0, 0x5555, 0x5555, 0xffff },
  { 0, 0xffff, 0x5555, 0xffff },
  { 0, 0x5555, 0xffff, 0xffff },
  { 0, 0xffff, 0xffff, 0xffff }
};
#endif

#if COLOR_RXVT
const GdkColor color_rxvt[16] =
{
  { 0, 0x0000, 0x0000, 0x0000 },
  { 0, 0xcdcd, 0x0000, 0x0000 },
  { 0, 0x0000, 0xcdcd, 0x0000 },
  { 0, 0xcdcd, 0xcdcd, 0x0000 },
  { 0, 0x0000, 0x0000, 0xcdcd },
  { 0, 0xcdcd, 0x0000, 0xcdcd },
  { 0, 0x0000, 0xcdcd, 0xcdcd },
  { 0, 0xfafa, 0xebeb, 0xd7d7 },
  { 0, 0x4040, 0x4040, 0x4040 },
  { 0, 0xffff, 0x0000, 0x0000 },
  { 0, 0x0000, 0xffff, 0x0000 },
  { 0, 0xffff, 0xffff, 0x0000 },
  { 0, 0x0000, 0x0000, 0xffff },
  { 0, 0xffff, 0x0000, 0xffff },
  { 0, 0x0000, 0xffff, 0xffff },
  { 0, 0xffff, 0xffff, 0xffff }
};
#endif

#if COLOR_TANGO
const GdkColor color_tango[16] =
{
  { 0, 0x2e2e, 0x3434, 0x3636 },
  { 0, 0xcccc, 0x0000, 0x0000 },
  { 0, 0x4e4e, 0x9a9a, 0x0606 },
  { 0, 0xc4c4, 0xa0a0, 0x0000 },
  { 0, 0x3434, 0x6565, 0xa4a4 },
  { 0, 0x7575, 0x5050, 0x7b7b },
  { 0, 0x0606, 0x9820, 0x9a9a },
  { 0, 0xd3d3, 0xd7d7, 0xcfcf },
  { 0, 0x5555, 0x5757, 0x5353 },
  { 0, 0xefef, 0x2929, 0x2929 },
  { 0, 0x8a8a, 0xe2e2, 0x3434 },
  { 0, 0xfcfc, 0xe9e9, 0x4f4f },
  { 0, 0x7272, 0x9f9f, 0xcfcf },
  { 0, 0xadad, 0x7f7f, 0xa8a8 },
  { 0, 0x3434, 0xe2e2, 0xe2e2 },
  { 0, 0xeeee, 0xeeee, 0xecec }
};
#endif

#if COLOR_XTERM
const GdkColor color_xterm[16] =
{
    {0, 0x0000, 0x0000, 0x0000 },
    {0, 0xcdcb, 0x0000, 0x0000 },
    {0, 0x0000, 0xcdcb, 0x0000 },
    {0, 0xcdcb, 0xcdcb, 0x0000 },
    {0, 0x1e1a, 0x908f, 0xffff },
    {0, 0xcdcb, 0x0000, 0xcdcb },
    {0, 0x0000, 0xcdcb, 0xcdcb },
    {0, 0xe5e2, 0xe5e2, 0xe5e2 },
    {0, 0x4ccc, 0x4ccc, 0x4ccc },
    {0, 0xffff, 0x0000, 0x0000 },
    {0, 0x0000, 0xffff, 0x0000 },
    {0, 0xffff, 0xffff, 0x0000 },
    {0, 0x4645, 0x8281, 0xb4ae },
    {0, 0xffff, 0x0000, 0xffff },
    {0, 0x0000, 0xffff, 0xffff },
    {0, 0xffff, 0xffff, 0xffff }
};
#endif

#ifdef FONT_ANTI_ALIAS
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#if MENU_FONT_BIGGER
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#if MENU_FONT_SMALLER
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#if MENU_FONT_DEFAULT_SIZE
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#if MENU_FONT_SELECT
#ifndef FONT
#define FONT "Monospace"
#endif
static char *number_char[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif

#if MENU_TOGGLE_ANTI_ALIAS
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#ifdef CTRL_TOGGLE_ANTI_ALIAS
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#ifdef CTRL_FONT_BIGGER
#define FONT_CHANGE_SIZE 1
#endif

#ifdef CTRL_FONT_SMALLER
#undef FONT_CHANGE_SIZE
#define FONT_CHANGE_SIZE 1
#endif

#ifdef CTRL_FONT_DEFAULT_SIZE
#undef FONT_CHANGE_SIZE
#define FONT_CHANGE_SIZE 1
#endif

#if FONT_CHANGE_SIZE
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#if FONT_SIZE
#ifndef FONT
#define FONT "Monospace"
#endif
#endif

#ifdef CTRL_FONT_SELECT
#ifndef FONT
#define FONT "Monospace"
#endif
#if !MENU_FONT_SELECT
static char *number_char[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif
#endif

#ifdef FONT
#ifndef FONT_SIZE
#define FONT_SIZE 10
#endif
char font_name[64];
char font_str[64];
int font_size = FONT_SIZE;
#endif

#if COMMAND_EXEC_PROGRAM
#undef COMMAND_SHOW_HELP_OPTION_S
#define COMMAND_SHOW_HELP_OPTION_S "s"
#define VTE_DEFAULT_COMMAND default_command
#define VTE_DEFAULT_ARGV default_argv
char default_command[64];
char **default_argv = NULL;
#endif
#if !COMMAND_EXEC_PROGRAM
#define VTE_DEFAULT_COMMAND DEFAULT_COMMAND
#define VTE_DEFAULT_ARGV DEFAULT_ARGV
#endif

#ifdef CTRL_SATURATION_MORE
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#endif

#ifdef CTRL_SATURATION_LESS
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#endif

#ifdef PROGRAM_ALWAYS_ON_TOP
int always_on_top = PROGRAM_ALWAYS_ON_TOP;
#endif

#ifdef BACKGROUND_SATURATION
double saturation_level = BACKGROUND_SATURATION;
#endif

#if COMMAND_AT_ROOT_WINDOW
#undef COMMAND_SHOW_HELP_OPTION_S
#define COMMAND_SHOW_HELP_OPTION_S "s"
int at_root_window = 0;
#endif

#if MENU
GtkWidget *menu;
#endif

#ifdef MENU_MATCH_STRING_EXEC
GtkWidget *match_open;
GtkWidget *match_copy;
GtkWidget *match_item;
char *matched_url = (char*)NULL;
#endif

#if STATUS_BAR
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

struct terminal {
  GtkWidget *vte;
#if GET_VTE_CHILD_PID
  int pid;
#endif
#if TAB_CLOSE_BUTTON
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *label_edit;
#endif
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  GtkWidget *hbox;
  GtkWidget *scrollbar;
#endif
#if DO_TAB_EDIT_LABEL
  int label_exist;
#endif
};

struct terminal *term;

#if TAB
GtkWidget *current_tab;
#endif

static int key_press_event(GtkWidget *widget, GdkEventKey *event);
static int menu_popup(GtkWidget *widget, GdkEventButton *event);
static int scroll_event(GtkWidget *widget, GdkEventScroll *event);
static void change_statusbar_encoding();
static void add_tab();
static void button_clicked(GtkWidget *widget, void *data);
static void del_tab(GtkWidget *widget, int do_close_dialog);
static void delete_event();
static void do_always_on_top();
static void do_change_saturation();
static void do_clear();
static void do_copy();
static void do_edit_label();
static void do_match_copy();
static void do_match_open();
static void do_menu_saturation();
static void do_menu_tint_color();
static void do_new_window();
static void do_paste();
static void do_reset();
static void do_select_all();
static void do_select_font();
static void do_toggle_antialias();
static void do_toggle_bg();
static void do_toggle_decorated();
static void do_toggle_fullscreen();
static void do_toggle_hotkeys();
static void do_toggle_scrollbar();
static void do_toggle_status_bar();
static void do_toggle_tabbar();
static void do_zoom_100();
static void do_zoom_in();
static void do_zoom_out();
static void set_encoding(GtkWidget *widget, void *data);
static void switch_page();

#if CTRL
static int key_press_event(GtkWidget *widget, GdkEventKey *event)
{
  if (event->state & GDK_CONTROL_MASK) {

#ifdef CTRL_TOGGLE_HOTKEYS
    if CTRL_TOGGLE_HOTKEYS {
      hotkey_status = !hotkey_status;
      return TRUE;
    }
#endif

#if DO_TOGGLE_HOTKEYS
    if (!hotkey_status) {
#endif

#ifdef CTRL_COLOR_BACKGROUND
      if CTRL_COLOR_BACKGROUND {
        GtkColorSelectionDialog *color_tint_dialog = (GtkColorSelectionDialog*)gtk_color_selection_dialog_new(LABEL_DIALOG_BACKGROUND_TINT);
        gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(color_tint_dialog->colorsel), &color_tint);
        if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(color_tint_dialog))) {
          gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(color_tint_dialog->colorsel), &color_tint);
#if TAB
          int i = 0;
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            vte_terminal_set_background_tint_color(VTE_TERMINAL(term->vte), &color_tint);
#if TAB
          }
#endif
        }
        gtk_widget_destroy((GtkWidget*)(color_tint_dialog));
        return TRUE;
      }
#endif

#ifdef CTRL_SATURATION_DIALOG
      if CTRL_SATURATION_DIALOG {
        double saturation_level_old = saturation_level;
        GtkWidget *saturation_dialog;
#if BUTTON_ORDER_BY_RCFILE
        if (button_order)
          saturation_dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        else
#endif
          saturation_dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
        adjustment = gtk_hscale_new_with_range(0, 1, 0.01);
        gtk_range_set_value(GTK_RANGE(adjustment), saturation_level_old);
        g_signal_connect_after(adjustment, "change-value", do_change_saturation, NULL);
        gtk_container_add(GTK_CONTAINER(GTK_DIALOG(saturation_dialog)->vbox), adjustment);
        gtk_dialog_set_default_response(GTK_DIALOG(saturation_dialog), GTK_RESPONSE_OK);
        gtk_widget_show_all(saturation_dialog);
        if (gtk_dialog_run(GTK_DIALOG(saturation_dialog)) == GTK_RESPONSE_OK)
          saturation_level = gtk_range_get_value(GTK_RANGE(adjustment));
        else
          saturation_level = saturation_level_old;
        gtk_widget_destroy(saturation_dialog);
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
          term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
          vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
          if (background_order[background_status] == "Opacity")
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef CTRL_TOGGLE_ANTI_ALIAS
      if CTRL_TOGGLE_ANTI_ALIAS {
        antialias_status++;
        if (antialias_status > VTE_ANTI_ALIAS_FORCE_DISABLE)
          antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
          term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
          vte_terminal_set_font_from_string_full(VTE_TERMINAL(term->vte), font_str, antialias_status);
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef CTRL_TOGGLE_DECORATED
      if CTRL_TOGGLE_DECORATED {
        window_decorated_status = !window_decorated_status;
        if (window_decorated_status)
          gtk_window_set_decorated(GTK_WINDOW(main_window), 0);
        else
          gtk_window_set_decorated(GTK_WINDOW(main_window), 1);
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        gtk_widget_hide_all(main_window);
        gtk_widget_show_all(main_window);
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
        gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);
        return TRUE;
      }
#endif

#ifdef CTRL_TOGGLE_FULLSCREEN
      if CTRL_TOGGLE_FULLSCREEN {
        window_fullscreen_status = !window_fullscreen_status;
        if (window_fullscreen_status)
          gtk_window_maximize(GTK_WINDOW(main_window));
        else
          gtk_window_unmaximize(GTK_WINDOW(main_window));
        return TRUE;
      }
#endif

#ifdef CTRL_TOGGLE_TABBAR
      if CTRL_TOGGLE_TABBAR {
        tabbar_status = !tabbar_status;
        if (tabbar_status)
          gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
        else
          gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 1);
#if TABBAR_AUTOHIDE
        if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1)
          gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
#endif
        gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
        return TRUE;
      }
#endif

#ifdef CTRL_TOGGLE_STATUS_BAR
      if CTRL_TOGGLE_STATUS_BAR {
        status_bar_status = !status_bar_status;
        if (status_bar_status)
          gtk_widget_hide(statusbar);
        else
          gtk_widget_show(statusbar);
        gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
        return TRUE;
      }
#endif

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
#ifdef CTRL_TOGGLE_SCROLLBAR
      if CTRL_TOGGLE_SCROLLBAR {
#if TAB
        int i = 0;
#endif
        scrollbar_status = !scrollbar_status;
        if (scrollbar_status) {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            gtk_widget_hide(term->scrollbar);
#if TAB
          }
#endif
        } else {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            gtk_widget_show(term->scrollbar);
#if TAB
          }
#endif
        }
        gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
        return TRUE;
      }
#endif
#endif /* SCROLLBAR_LEFT || SCROLLBAR_RIGHT */

#ifdef CTRL_OPEN_NEW_WINDOW
      if CTRL_OPEN_NEW_WINDOW {
        system(PROGRAM_NAME " &");
        return TRUE;
      }
#endif

#ifdef CTRL_TOGGLE_ON_TOP
      if CTRL_TOGGLE_ON_TOP {
        always_on_top = !always_on_top;
        gtk_window_set_keep_above(GTK_WINDOW(main_window), always_on_top);
#if COMMAND_AT_ROOT_WINDOW
        if (at_root_window && !always_on_top) {
          gtk_window_set_keep_below(GTK_WINDOW(main_window), 1);
          gtk_window_set_decorated(GTK_WINDOW(main_window), 0);
          gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), 1);
          gtk_window_set_skip_pager_hint(GTK_WINDOW(main_window), 1);
        }
#endif
        return TRUE;
      }
#endif

#ifdef CTRL_SELECT_ALL
      if CTRL_SELECT_ALL {
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        vte_terminal_select_all(VTE_TERMINAL(term->vte));
        return TRUE;
      }
#endif

#ifdef CTRL_EDIT_ENCODING
      if CTRL_EDIT_ENCODING {
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        char *encoding_name = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term->vte));
        GtkWidget *encoding_entry = gtk_entry_new();
        GtkWidget *encoding_dialog;
#if BUTTON_ORDER_BY_RCFILE
        if (button_order)
          encoding_dialog = gtk_dialog_new_with_buttons(encoding_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        else
#endif
          encoding_dialog = gtk_dialog_new_with_buttons(encoding_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
        gtk_dialog_set_default_response(GTK_DIALOG(encoding_dialog), GTK_RESPONSE_OK);
        gtk_entry_set_text(GTK_ENTRY(encoding_entry), encoding_name);
        gtk_entry_set_activates_default(GTK_ENTRY(encoding_entry), 1);
        gtk_container_add(GTK_CONTAINER(GTK_DIALOG(encoding_dialog)->vbox), encoding_entry);
        gtk_widget_show_all(encoding_dialog);
        if (gtk_dialog_run(GTK_DIALOG(encoding_dialog)) == GTK_RESPONSE_OK) {
          vte_terminal_set_encoding(VTE_TERMINAL(term->vte), gtk_entry_get_text(GTK_ENTRY(encoding_entry)));
#if STATUS_BAR
          change_statusbar_encoding();
#endif
        }
        gtk_widget_destroy(encoding_dialog);
        return TRUE;
      }
#endif /* CTRL_EDIT_ENCODING */

#ifdef CTRL_TAB_EDIT_LABEL
      if CTRL_TAB_EDIT_LABEL {
        char *label_name = "";
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        if (term->label_exist) {
#if TAB_CLOSE_BUTTON
          label_name = (char*)gtk_label_get_text(GTK_LABEL(term->label_edit));
#endif
#if !TAB_CLOSE_BUTTON
          label_name = (char*)gtk_label_get_text(GTK_LABEL(gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index))));
#endif
        }
        GtkWidget *entry = gtk_entry_new();
        GtkWidget *dialog;
#if BUTTON_ORDER_BY_RCFILE
        if (button_order)
          dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        else
#endif
          dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
        gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
        gtk_entry_set_text(GTK_ENTRY(entry), label_name);
        gtk_entry_set_activates_default(GTK_ENTRY(entry), 1);
        gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), entry);
        gtk_widget_show_all(dialog);
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
#if TAB_CLOSE_BUTTON
          term->label = gtk_hbox_new(0, 0);
          term->button = gtk_button_new();
          GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
          GtkRcStyle *rcstyle = gtk_rc_style_new();
          gtk_button_set_image(GTK_BUTTON(term->button), img_button);
          rcstyle->xthickness = 0;
          rcstyle->ythickness = 0;
          gtk_widget_modify_style(term->button, rcstyle);
          gtk_rc_style_unref(rcstyle);
          term->label_edit = gtk_label_new(gtk_entry_get_text(GTK_ENTRY(entry)));
          gtk_box_pack_start(GTK_BOX(term->label), term->label_edit, 1, 1, 0);
          gtk_box_pack_start(GTK_BOX(term->label), term->button, 0, 0, 0);
          gtk_widget_show_all(term->label);
          g_signal_connect(term->button, "clicked", G_CALLBACK(button_clicked), term->button);
          gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), VTE_HBOX, term->label);
#endif
#if !TAB_CLOSE_BUTTON
          gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook), VTE_HBOX, gtk_entry_get_text(GTK_ENTRY(entry)));
#endif
          term->label_exist = 1;
        }
        gtk_widget_destroy(dialog);
        return TRUE;
      }
#endif /* CTRL_TAB_EDIT_LABEL */

#if CTRL_TAB_GO_TO_NUMBER
      if (event->keyval <= GDK_9 && event->keyval >= GDK_0) {
        int index = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
        switch (event->keyval) {
          case GDK_1:
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
            break;
          case GDK_2:
            if (index > 1)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
            else
              return FALSE;
            break;
          case GDK_3:
            if (index > 2)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);
            else
              return FALSE;
            break;
          case GDK_4:
            if (index > 3)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);
            else
              return FALSE;
            break;
          case GDK_5:
            if (index > 4)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 4);
            else
              return FALSE;
            break;
          case GDK_6:
            if (index > 5)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 5);
            else
              return FALSE;
            break;
          case GDK_7:
            if (index > 6)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 6);
            else
              return FALSE;
            break;
          case GDK_8:
            if (index > 7)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 7);
            else
              return FALSE;
            break;
          case GDK_9:
            if (index > 8)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 8);
            else
              return FALSE;
            break;
          case GDK_0:
            if (index > 9)
              gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 9);
            else
              return FALSE;
            break;
        }
        return TRUE;
      }
#endif /* CTRL_TAB_GO_TO_NUMBER */

#ifdef CTRL_TOGGLE_BACKGROUND
      if CTRL_TOGGLE_BACKGROUND {
        background_status++;
#if TAB
        int i = 0;
#endif
        if (background_status == background_order_size)
          background_status = 0;
#if TOGGLE_BG_TRANSPARENT
        if (background_order[background_status] == "Transparent") {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 1);
#if TAB
          }
#endif
          return TRUE;
        }
#endif
#if TOGGLE_BG_IMAGE
        if (background_order[background_status] == "Image") {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
            vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), imgstr);
#if TAB
          }
#endif
          return TRUE;
        }
#endif
#if TOGGLE_BG_NO_BACKGROUND
        if (background_order[background_status] == "No background") {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
            vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), "/dev/null");
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), 65535);
#if TAB
          }
#endif
          return TRUE;
        }
#endif
#if TOGGLE_BG_OPACITY
        if (background_order[background_status] == "Opacity") {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
            vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), "/dev/null");
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#if TAB
          }
#endif
          return TRUE;
        }
#endif
        return FALSE;
      }
#endif

#ifdef CTRL_SATURATION_MORE
      if CTRL_SATURATION_MORE {
        saturation_level += 0.1;
        if (saturation_level > 1)
          saturation_level = 1;
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
          term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
          vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
          if (background_order[background_status] == "Opacity")
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef CTRL_SATURATION_LESS
      if CTRL_SATURATION_LESS {
        saturation_level -= 0.1;
        if (saturation_level < 0)
          saturation_level = 0;
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
          term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
          vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
          if (background_order[background_status] == "Opacity")
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef CTRL_RESET_TERMINAL
      if CTRL_RESET_TERMINAL {
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 0);
        return TRUE;
      }
#endif

#ifdef CTRL_RESET_AND_CLEAR
      if CTRL_RESET_AND_CLEAR {
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 1);
        return TRUE;
      }
#endif

#ifdef CTRL_COPY
      if CTRL_COPY {
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        vte_terminal_copy_clipboard(VTE_TERMINAL(term->vte));
        return TRUE;
      }
#endif

#ifdef CTRL_PASTE
      if CTRL_PASTE {
#if TAB
        current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
        term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
        vte_terminal_paste_clipboard(VTE_TERMINAL(term->vte));
        return TRUE;
      }
#endif

#ifdef CTRL_FONT_BIGGER
      if CTRL_FONT_BIGGER {
        font_size++;
        goto font_size_changed;
      }
#endif

#ifdef CTRL_FONT_SMALLER
      if CTRL_FONT_SMALLER {
        font_size--;
        goto font_size_changed;
      }
#endif

#ifdef CTRL_FONT_DEFAULT_SIZE
      if CTRL_FONT_DEFAULT_SIZE {
        font_size = FONT_SIZE;
        goto font_size_changed;
      }
#endif

#ifdef CTRL_FONT_SELECT
      if CTRL_FONT_SELECT {
        GtkWidget *font_dialog = gtk_font_selection_dialog_new(font_str);
        gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog), font_str);
        if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(font_dialog))) {
          sprintf(font_name, "%s", gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog)));
          int len = strlen(font_name) - 1;
          font_size = 0;
          int i = 0;
          if (isdigit(font_name[len - 1])) {
            for (i = 0 ; i < 10 ; i++) {
              if (font_name[len - 1] == *number_char[i])
                font_size += i * 10;
              if (font_name[len] == *number_char[i])
                font_size += i;
            }
          } else {
            for (i = 0 ; i < 10 ; i++) {
              if (font_name[len] == *number_char[i])
                font_size += i;
            }
          }
          while (len > 0 && isdigit(font_name[len]))
            font_name[len--] = 0;
          while (len > 0 && font_name[len] == ' ')
            font_name[len--] = 0;
          sprintf(font_str, "%s %d", font_name, font_size);
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
            term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
            vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
          }
#endif
          gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
        }
        gtk_widget_destroy(font_dialog);
        return TRUE;
      }
#endif /* CTRL_FONT_SELECT */

#ifdef CTRL_TAB_FIRST
      if CTRL_TAB_FIRST {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
        return TRUE;
      }
#endif

#ifdef CTRL_TAB_LAST
      if CTRL_TAB_LAST {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
        return TRUE;
      }
#endif

#ifdef CTRL_TAB_PREVIOUS
      if CTRL_TAB_PREVIOUS {
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        if (index)
          gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index - 1);
        else
          gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
        return TRUE;
      }
#endif

#ifdef CTRL_TAB_NEXT
      if CTRL_TAB_NEXT {
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        if (index == (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1))
          gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
        else
          gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index + 1);
        return TRUE;
      }
#endif

#ifdef CTRL_TAB_ADD
      if CTRL_TAB_ADD {
        add_tab();
        return TRUE;
      }
#endif

#ifdef CTRL_TAB_REMOVE
      if CTRL_TAB_REMOVE {
        del_tab(NULL, DO_CLOSE_DIALOG);
        return TRUE;
      }
#endif

#if DO_TOGGLE_HOTKEYS
    }
#endif

  }
  return FALSE;

#if FONT_CHANGE_SIZE
font_size_changed:
  sprintf(font_str, "%s %d", font_name, font_size);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  return TRUE;
#endif
}
#endif /* CTRL */

#if MENU
static int menu_popup(GtkWidget *widget, GdkEventButton *event)
{
  GdkEventButton *event_button;
  if (event->type == GDK_BUTTON_PRESS) {
    event_button = (GdkEventButton*)event;
    if (event_button->button == 3) {
#ifdef MENU_MATCH_STRING_EXEC
      int tag = -1;
#if TAB
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      matched_url = vte_terminal_match_check(VTE_TERMINAL(term->vte), event->x / vte_terminal_get_char_width(VTE_TERMINAL(term->vte)), event->y / vte_terminal_get_char_height(VTE_TERMINAL(term->vte)), &tag);
      if (matched_url != NULL) {
        gtk_widget_show_all(menu);
#ifdef MENU_CUSTOM
        if (menu_item_success == 100)
          gtk_widget_hide(match_item);
#endif
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event_button->button, event_button->time);
      } else {
        gtk_widget_hide(match_open);
        gtk_widget_hide(match_copy);
        gtk_widget_hide(match_item);
#ifdef MENU_CUSTOM
        if (menu_item_success > 100)
#endif
          gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event_button->button, event_button->time);
      }
#endif /* MENU_MATCH_STRING_EXEC */

#ifndef MENU_MATCH_STRING_EXEC
#ifdef MENU_CUSTOM
      if (menu_item_success > 0)
#endif
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event_button->button, event_button->time);
#endif
      return TRUE;
    }
  }
  return FALSE;
}
#endif /* MENU */

#if MOUSE_CTRL_SATURATION
static int scroll_event(GtkWidget *widget, GdkEventScroll *event)
{
  if (event->direction == GDK_SCROLL_RIGHT) {
    saturation_level += 0.025;
    if (saturation_level > 1)
      saturation_level = 1;
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
      if (background_order[background_status] == "Opacity")
        vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
    }
#endif
    return TRUE;
  }

  if (event->direction == GDK_SCROLL_LEFT) {
    saturation_level -= 0.025;
    if (saturation_level < 0)
      saturation_level = 0;
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
      if (background_order[background_status] == "Opacity")
        vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
    }
#endif
    return TRUE;
  }
  return FALSE;
}
#endif /* MOUSE_CTRL_SATURATION */

#if STATUS_BAR
static void change_statusbar_encoding()
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, vte_terminal_get_encoding(VTE_TERMINAL(term->vte)));
}
#endif

static void delete_event()
{
#if CLOSE_DIALOG && TAB
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 1) {
    GtkWidget *dialog;
    GtkWidget *dialog_hbox;
    GtkWidget *dialog_icon;
    GtkWidget *dialog_string;
#if BUTTON_ORDER_BY_RCFILE
    if (button_order)
      dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_CLOSE, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
    else
#endif
      dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_CLOSE, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);
    dialog_hbox = gtk_hbox_new(0, 0);
    dialog_icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_DIALOG);
    dialog_string = gtk_label_new(LABEL_DIALOG_CLOSE);
    gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), dialog_hbox);
    gtk_box_pack_start(GTK_BOX(dialog_hbox), dialog_icon, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(dialog_hbox), dialog_string, 1, 0, 0);
    gtk_widget_show_all(dialog);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_CLOSE) {
      gtk_widget_destroy(dialog);
#if !CLOSE_SAFELY
      gtk_main_quit();
#endif
    } else {
      gtk_widget_destroy(dialog);
      return;
    }
  }
#endif
#if TAB
  int i = 0;
  for (i = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i > 0 ; i--) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), i - 1);
    del_tab(NULL, 0);
  }
#endif
#if !TAB
  del_tab(NULL, DO_CLOSE_DIALOG);
#endif
}

#if MENU_TOGGLE_ON_TOP
static void do_always_on_top()
{
  always_on_top = !always_on_top;
  gtk_window_set_keep_above(GTK_WINDOW(main_window), always_on_top);
#if COMMAND_AT_ROOT_WINDOW
  if (at_root_window && !always_on_top) {
    gtk_window_set_keep_below(GTK_WINDOW(main_window), 1);
    gtk_window_set_decorated(GTK_WINDOW(main_window), 0);
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), 1);
    gtk_window_set_skip_pager_hint(GTK_WINDOW(main_window), 1);
  }
#endif
}
#endif

#if DO_CHANGE_SATURATION
static void do_change_saturation()
{
  saturation_level = gtk_range_get_value(GTK_RANGE(adjustment));
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
  vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
  if (background_order[background_status] == "Opacity")
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
}
#endif

#if MENU_RESET_AND_CLEAR
static void do_clear()
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 1);
}
#endif

#if MENU_COPY
static void do_copy()
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_copy_clipboard(VTE_TERMINAL(term->vte));
}
#endif

#if MENU_TAB_EDIT_LABEL
static void do_edit_label()
{
  char *label_name = "";
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  if (term->label_exist) {
#if TAB_CLOSE_BUTTON
    label_name = (char*)gtk_label_get_text(GTK_LABEL(term->label_edit));
#endif
#if !TAB_CLOSE_BUTTON
    label_name = (char*)gtk_label_get_text(GTK_LABEL(gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index))));
#endif
  }
  GtkWidget *entry = gtk_entry_new();
  GtkWidget *dialog;
#if BUTTON_ORDER_BY_RCFILE
  if (button_order)
    dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
  else
#endif
    dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
  gtk_entry_set_text(GTK_ENTRY(entry), label_name);
  gtk_entry_set_activates_default(GTK_ENTRY(entry), 1);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), entry);
  gtk_widget_show_all(dialog);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
#if TAB_CLOSE_BUTTON
    term->label = gtk_hbox_new(0, 0);
    term->button = gtk_button_new();
    GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
    GtkRcStyle *rcstyle = gtk_rc_style_new();
    gtk_button_set_image(GTK_BUTTON(term->button), img_button);
    rcstyle->xthickness = 0;
    rcstyle->ythickness = 0;
    gtk_widget_modify_style(term->button, rcstyle);
    gtk_rc_style_unref(rcstyle);
    term->label_edit = gtk_label_new(gtk_entry_get_text(GTK_ENTRY(entry)));
    gtk_box_pack_start(GTK_BOX(term->label), term->label_edit, 1, 1, 0);
    gtk_box_pack_start(GTK_BOX(term->label), term->button, 0, 0, 0);
    gtk_widget_show_all(term->label);
    g_signal_connect(term->button, "clicked", G_CALLBACK(button_clicked), term->button);
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), VTE_HBOX, term->label);
#endif
#if !TAB_CLOSE_BUTTON
    gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook), VTE_HBOX, gtk_entry_get_text(GTK_ENTRY(entry)));
#endif
    term->label_exist = 1;
  }
  gtk_widget_destroy(dialog);
}
#endif /* MENU_TAB_EDIT_LABEL */

#ifdef MENU_MATCH_STRING_EXEC
static void do_match_copy()
{
  gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), matched_url, -1);
  matched_url = NULL;
}
#endif

#ifdef MENU_MATCH_STRING_EXEC
static void do_match_open()
{
  char new_window_str[256];
  sprintf(new_window_str, "%s %s &\0", MENU_MATCH_STRING_EXEC, matched_url);
  system(new_window_str);
  matched_url = NULL;
}
#endif

#if MENU_CHANGE_SATURATION
static void do_menu_saturation()
{
  double saturation_level_old = saturation_level;
  GtkWidget *saturation_dialog;
#if BUTTON_ORDER_BY_RCFILE
  if (button_order)
    saturation_dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
  else
#endif
    saturation_dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
  adjustment = gtk_hscale_new_with_range(0, 1, 0.001);
  gtk_range_set_value(GTK_RANGE(adjustment), saturation_level_old);
  g_signal_connect_after(adjustment, "change-value", do_change_saturation, NULL);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(saturation_dialog)->vbox), adjustment);
  gtk_dialog_set_default_response(GTK_DIALOG(saturation_dialog), GTK_RESPONSE_OK);
  gtk_widget_show_all(saturation_dialog);
  if (gtk_dialog_run(GTK_DIALOG(saturation_dialog)) == GTK_RESPONSE_OK)
    saturation_level = gtk_range_get_value(GTK_RANGE(adjustment));
  else
    saturation_level = saturation_level_old;
  gtk_widget_destroy(saturation_dialog);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
    vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
    if (background_order[background_status] == "Opacity")
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
  }
#endif
}
#endif /* MENU_CHANGE_SATURATION */

#if MENU_COLOR_BACKGROUND
static void do_menu_tint_color()
{
  GtkColorSelectionDialog *color_tint_dialog = (GtkColorSelectionDialog*)gtk_color_selection_dialog_new(LABEL_DIALOG_BACKGROUND_TINT);
  gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(color_tint_dialog->colorsel), &color_tint);
  if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(color_tint_dialog))) {
    gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(color_tint_dialog->colorsel), &color_tint);
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_tint_color(VTE_TERMINAL(term->vte), &color_tint);
#if TAB
    }
#endif
  }
  gtk_widget_destroy((GtkWidget*)(color_tint_dialog));
}
#endif

#if MENU_OPEN_NEW_WINDOW
static void do_new_window()
{
  system(PROGRAM_NAME " &");
}
#endif

#if MENU_PASTE
static void do_paste()
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_paste_clipboard(VTE_TERMINAL(term->vte));
}
#endif

#if MENU_RESET_TERMINAL
static void do_reset()
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 0);
}
#endif

#if MENU_SELECT_ALL
static void do_select_all()
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_select_all(VTE_TERMINAL(term->vte));
}
#endif

#if MENU_FONT_SELECT
static void do_select_font()
{
  GtkWidget *font_dialog = gtk_font_selection_dialog_new(font_str);
  gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog), font_str);
  if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(font_dialog))) {
    sprintf(font_name, "%s", gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog)));
    int len = strlen(font_name) - 1;
    font_size = 0;
    int i = 0;
    if (isdigit(font_name[len - 1])) {
      for (i = 0 ; i < 10 ; i++) {
        if (font_name[len - 1] == *number_char[i])
          font_size += i * 10;
        if (font_name[len] == *number_char[i])
          font_size += i;
      }
    } else {
      for (i = 0 ; i < 10 ; i++) {
        if (font_name[len] == *number_char[i])
          font_size += i;
      }
    }
    while (len > 0 && isdigit(font_name[len]))
      font_name[len--] = 0;
    while (len > 0 && font_name[len] == ' ')
      font_name[len--] = 0;
    sprintf(font_str, "%s %d", font_name, font_size);
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
    }
#endif
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
  gtk_widget_destroy(font_dialog);
}
#endif /* MENU_FONT_SELECT */

#if MENU_TOGGLE_ANTI_ALIAS
static void do_toggle_antialias()
{
  antialias_status++;
  if (antialias_status > VTE_ANTI_ALIAS_FORCE_DISABLE)
    antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
    vte_terminal_set_font_from_string_full(VTE_TERMINAL(term->vte), font_str, antialias_status);
#if TAB
  }
#endif
}
#endif

#if MENU_TOGGLE_BACKGROUND
static void do_toggle_bg()
{
  background_status++;
  if (background_status == background_order_size)
    background_status = 0;
#if TAB
  int i = 0;
#endif
#if TOGGLE_BG_TRANSPARENT
  if (background_order[background_status] == "Transparent") {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 1);
#if TAB
    }
#endif
  }
#endif
#if TOGGLE_BG_IMAGE
  if (background_order[background_status] == "Image") {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
      vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), imgstr);
#if TAB
    }
#endif
  }
#endif
#if TOGGLE_BG_NO_BACKGROUND
  if (background_order[background_status] == "No background") {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
      vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), "/dev/null");
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), 65535);
#if TAB
    }
#endif
  }
#endif
#if TOGGLE_BG_OPACITY
  if (background_order[background_status] == "Opacity") {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
      vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), "/dev/null");
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#if TAB
    }
#endif
  }
#endif
}
#endif

#if MENU_TOGGLE_DECORATED
static void do_toggle_decorated()
{
  window_decorated_status = !window_decorated_status;
  if (window_decorated_status)
    gtk_window_set_decorated(GTK_WINDOW(main_window), 0);
  else
    gtk_window_set_decorated(GTK_WINDOW(main_window), 1);
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  gtk_widget_hide_all(main_window);
  gtk_widget_show_all(main_window);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
  gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);
}
#endif

#if MENU_TOGGLE_FULLSCREEN
static void do_toggle_fullscreen()
{
  window_fullscreen_status = !window_fullscreen_status;
  if (window_fullscreen_status)
    gtk_window_maximize(GTK_WINDOW(main_window));
  else
    gtk_window_unmaximize(GTK_WINDOW(main_window));
}
#endif

#if MENU_TOGGLE_HOTKEYS
static void do_toggle_hotkeys()
{
  hotkey_status = !hotkey_status;
}
#endif

#if MENU_TOGGLE_SCROLLBAR
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
static void do_toggle_scrollbar()
{
#if TAB
  int i = 0;
#endif
  scrollbar_status = !scrollbar_status;
  if (scrollbar_status) {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      gtk_widget_hide(term->scrollbar);
#if TAB
    }
#endif
  } else {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      gtk_widget_show(term->scrollbar);
#if TAB
    }
#endif
  }
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif
#endif /* MENU_TOGGLE_SCROLLBAR */

#if MENU_TOGGLE_STATUS_BAR && STATUS_BAR
static void do_toggle_status_bar()
{
  status_bar_status = !status_bar_status;
  if (status_bar_status)
    gtk_widget_hide(statusbar);
  else
    gtk_widget_show(statusbar);
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_TOGGLE_TABBAR
static void do_toggle_tabbar()
{
  tabbar_status = !tabbar_status;
  if (tabbar_status)
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
  else
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 1);
#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1)
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_FONT_DEFAULT_SIZE
static void do_zoom_100()
{
  font_size = FONT_SIZE;
  sprintf(font_str, "%s %d", font_name, font_size);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_FONT_BIGGER
static void do_zoom_in()
{
  font_size++;
  sprintf(font_str, "%s %d", font_name, font_size);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_FONT_SMALLER
static void do_zoom_out()
{
  font_size--;
  sprintf(font_str, "%s %d", font_name, font_size);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

static void add_tab()
{
  term = g_malloc(sizeof(struct terminal));

#if DO_TAB_EDIT_LABEL
  int label_exist = 0;
#endif

#if TAB_LABEL_INIT
  GtkWidget *label;
#endif

#ifdef TAB_LABEL
#if TAB_LABEL_NUMBER
  label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) + 1)));
#endif
#if !TAB_LABEL_NUMBER
  label = gtk_label_new(TAB_LABEL);
#endif
#if DO_TAB_EDIT_LABEL
  label_exist = 1;
#endif
#endif

#ifdef TAB_LABEL_CUSTOM
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < label_style_size)
    label = gtk_label_new(g_strdup_printf("%s", label_style_custom[gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))]));
  else
    label = NULL;
#if DO_TAB_EDIT_LABEL
  label_exist = 1;
#endif
#endif

#if TAB_LABEL_POEM
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < label_style_size)
    label = gtk_label_new(g_strdup_printf("%s", label_style_poem[gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))]));
  else
    label = NULL;
#if DO_TAB_EDIT_LABEL
  label_exist = 1;
#endif
#endif

#if TAB_CLOSE_BUTTON
  term->label = gtk_hbox_new(0, 0);
  term->button = gtk_button_new();
  GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  GtkRcStyle *rcstyle = gtk_rc_style_new();
  gtk_button_set_image(GTK_BUTTON(term->button), img_button);
  rcstyle->xthickness = 0;
  rcstyle->ythickness = 0;
  gtk_widget_modify_style(term->button, rcstyle);
  gtk_rc_style_unref(rcstyle);
  term->label_edit = label;
  gtk_box_pack_start(GTK_BOX(term->label), term->label_edit, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(term->label), term->button, 0, 0, 0);
  gtk_widget_show_all(term->label);
  g_signal_connect(term->button, "clicked", G_CALLBACK(button_clicked), term->button);
#endif

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  term->hbox = gtk_hbox_new(0, 0);
#endif

  term->vte = vte_terminal_new();

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  term->scrollbar = gtk_vscrollbar_new(vte_terminal_get_adjustment(VTE_TERMINAL(term->vte)));
#endif

#if SCROLLBAR_LEFT
  gtk_box_pack_start(GTK_BOX(term->hbox), term->scrollbar, 0, 0, 0);
#endif

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  gtk_box_pack_start(GTK_BOX(term->hbox), term->vte, 1, 1, 0);
#endif

#if SCROLLBAR_RIGHT
  gtk_box_pack_start(GTK_BOX(term->hbox), term->scrollbar, 0, 0, 0);
#endif

#if TAB_NEW_PATH_EQUAL_OLD
  char *default_directory = (char*)DEFAULT_DIRECTORY;
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0) {
    struct terminal *oldterm;
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
    oldterm = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
    default_directory = g_file_read_link(g_strdup_printf("/proc/%d/cwd", oldterm->pid), NULL);
  }
#endif

#if GET_VTE_CHILD_PID
  term->pid = vte_terminal_fork_command(VTE_TERMINAL(term->vte), VTE_DEFAULT_COMMAND, VTE_DEFAULT_ARGV, DEFAULT_ENVV, VTE_DEFAULT_DIRECTORY, RECORD_LASTLOG, RECORD_UTMP, RECORD_WTMP);
#endif
#if !GET_VTE_CHILD_PID
  vte_terminal_fork_command(VTE_TERMINAL(term->vte), VTE_DEFAULT_COMMAND, VTE_DEFAULT_ARGV, DEFAULT_ENVV, VTE_DEFAULT_DIRECTORY, RECORD_LASTLOG, RECORD_UTMP, RECORD_WTMP);
#endif

#ifdef MATCH_STRING
   vte_terminal_match_add(VTE_TERMINAL(term->vte), MATCH_STRING);
#endif

#ifdef DEFAULT_EMULATION_TYPE
  vte_terminal_set_emulation(VTE_TERMINAL(term->vte), DEFAULT_EMULATION_TYPE);
#endif

#ifdef FONT_ENABLE_BOLD_TEXT
  vte_terminal_set_allow_bold(VTE_TERMINAL(term->vte), FONT_ENABLE_BOLD_TEXT);
#endif

#ifdef BELL_AUDIBLE
  vte_terminal_set_audible_bell(VTE_TERMINAL(term->vte), BELL_AUDIBLE);
#endif

#ifdef BELL_VISIBLE
  vte_terminal_set_visible_bell(VTE_TERMINAL(term->vte), BELL_VISIBLE);
#endif

#ifdef BACKGROUND_IMAGE
  vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), imgstr);
#endif

#if BACKGROUND_OPACITY
  vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif

#ifdef BACKGROUND_TINT_COLOR
#if BACKGROUND_EXIST
  vte_terminal_set_background_tint_color(VTE_TERMINAL(term->vte), &color_tint);
#endif
#endif

#ifdef BACKGROUND_SATURATION
#if BACKGROUND_EXIST
  vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#endif
#endif

#ifdef BACKGROUND_TRANSPARENT
  vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), BACKGROUND_TRANSPARENT);
#endif

#ifdef BACKSPACE_KEY
  vte_terminal_set_backspace_binding(VTE_TERMINAL(term->vte), BACKSPACE_KEY);
#endif

#ifdef DELETE_KEY
  vte_terminal_set_delete_binding(VTE_TERMINAL(term->vte), DELETE_KEY);
#endif

#ifdef COLOR_STYLE
  vte_terminal_set_colors(VTE_TERMINAL(term->vte), NULL, NULL, COLOR_STYLE, 16);
#endif

#if SET_DEFAULT_COLORS
  vte_terminal_set_default_colors(VTE_TERMINAL(term->vte));
#endif

#ifdef COLOR_BACKGROUND
  GdkColor color_bg;
  gdk_color_parse(COLOR_BACKGROUND, &color_bg);
  vte_terminal_set_color_background(VTE_TERMINAL(term->vte), &color_bg);
#endif

#ifdef COLOR_TEXT_BOLD
  GdkColor color_bold;
  gdk_color_parse(COLOR_TEXT_BOLD, &color_bold);
  vte_terminal_set_color_bold(VTE_TERMINAL(term->vte), &color_bold);
#endif

#ifdef CURSOR_COLOR
  GdkColor color_cursor;
  gdk_color_parse(CURSOR_COLOR, &color_cursor);
  vte_terminal_set_color_cursor(VTE_TERMINAL(term->vte), &color_cursor);
#endif

#ifdef COLOR_TEXT_DIM
  GdkColor color_dim;
  gdk_color_parse(COLOR_TEXT_DIM, &color_dim);
  vte_terminal_set_color_dim(VTE_TERMINAL(term->vte), &color_dim);
#endif

#ifdef COLOR_FOREGROUND
  GdkColor color_fg;
  gdk_color_parse(COLOR_FOREGROUND, &color_fg);
  vte_terminal_set_color_foreground(VTE_TERMINAL(term->vte), &color_fg);
#endif

#ifdef COLOR_TEXT_HIGHLIGHTED
  GdkColor color_highlight;
  gdk_color_parse(COLOR_TEXT_HIGHLIGHTED, &color_highlight);
  vte_terminal_set_color_highlight(VTE_TERMINAL(term->vte), &color_highlight);
#endif

#ifdef CURSOR_BLINKS
  vte_terminal_set_cursor_blinks(VTE_TERMINAL(term->vte), CURSOR_BLINKS);
#endif

#ifdef DEFAULT_ENCODING
  vte_terminal_set_encoding(VTE_TERMINAL(term->vte), DEFAULT_ENCODING);
#endif

#ifdef FONT
#if INT_ANTI_ALIAS_STATUS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(term->vte), font_str, antialias_status);
#endif
#if !INT_ANTI_ALIAS_STATUS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(term->vte), font_str, VTE_ANTI_ALIAS);
#endif
#endif

#ifdef MOUSE_CURSOR_AUTOHIDE
  vte_terminal_set_mouse_autohide(VTE_TERMINAL(term->vte), MOUSE_CURSOR_AUTOHIDE);
#endif

#ifdef BACKGROUND_SCROLLABLE
  vte_terminal_set_scroll_background(VTE_TERMINAL(term->vte), BACKGROUND_SCROLLABLE);
#endif

#ifdef SCROLL_ON_KEYSTROKE
  vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(term->vte), SCROLL_ON_KEYSTROKE);
#endif

#ifdef SCROLL_ON_OUTPUT
  vte_terminal_set_scroll_on_output(VTE_TERMINAL(term->vte), SCROLL_ON_OUTPUT);
#endif

#ifdef SCROLL_LINES
  vte_terminal_set_scrollback_lines(VTE_TERMINAL(term->vte), SCROLL_LINES);
#endif

#if DEFAULT_COLUMNS && DEFAULT_ROWS
  vte_terminal_set_size(VTE_TERMINAL(term->vte), DEFAULT_COLUMNS, DEFAULT_ROWS);
#endif

#ifdef WORD_CHARS
  vte_terminal_set_word_chars(VTE_TERMINAL(term->vte), WORD_CHARS);
#endif

  gtk_widget_show_all(VTE_HBOX);
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), VTE_HBOX, VTE_LABEL);

#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
  g_object_set_data(G_OBJECT(current_tab), "current_tab", term);
#endif

  g_signal_connect(term->vte, "child-exited", G_CALLBACK(del_tab), (int*)DO_CLOSE_DIALOG);

#if MENU
  g_signal_connect(term->vte, "button-press-event", G_CALLBACK(menu_popup), NULL);
#endif

#if MOUSE_CTRL_SATURATION
  g_signal_connect(term->vte, "scroll-event", G_CALLBACK(scroll_event), NULL);
#endif

#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1)
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
  else
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), VTE_TABBAR);
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
#endif

#if DO_TOGGLE_BACKGROUND
#if TOGGLE_BG_TRANSPARENT
  if (background_order[background_status] == "Transparent")
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 1);
#endif
#if TOGGLE_BG_IMAGE
  if (background_order[background_status] == "Image") {
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
    vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), imgstr);
  }
#endif
#if TOGGLE_BG_NO_BACKGROUND
  if (background_order[background_status] == "No background") {
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
    vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), "/dev/null");
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), 65535);
  }
#endif
#if TOGGLE_BG_OPACITY
  if (background_order[background_status] == "Opacity") {
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
    vte_terminal_set_background_image_file(VTE_TERMINAL(term->vte), "/dev/null");
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
  }
#endif
#endif

  gtk_widget_show_all(notebook);

#if DO_TOGGLE_SCROLLBAR
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  if (scrollbar_status) {
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
      term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
      gtk_widget_hide(term->scrollbar);
#if TAB
    }
#endif
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
#endif
#endif /* DO_TOGGLE_SCROLLBAR */

#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif

#if TAB_EXPANDED_WIDTH
  gtk_container_child_set(GTK_CONTAINER(notebook), VTE_HBOX, "tab-expand", 1, NULL);
#endif

#if TAB_REORDERABLE
  gtk_notebook_set_tab_reorderable(GTK_NOTEBOOK(notebook), VTE_HBOX, 1);
#endif

  gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);

#if DO_TAB_EDIT_LABEL
  term->label_exist = label_exist;
#endif
}

#if TAB_CLOSE_BUTTON
static void button_clicked(GtkWidget *widget, void *data)
{
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
  int i = 0;
  int killed = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), i);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
    if (data == term->button) {
      gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), i);
      del_tab(NULL, DO_CLOSE_DIALOG);
      killed = i;
    }
  }
  if (killed < index)
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index - 1);
  else
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
}
#endif

static void del_tab(GtkWidget *widget, int do_close_dialog)
{
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));

#if CLOSE_DIALOG
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  int user_want_to_close = 0;
  if (do_close_dialog) {
#if 1 /* Maybe only work on GNU/Linux */
    char *stat = NULL;
    char **stats = NULL;
    gsize length;
    if (g_file_get_contents(g_strdup_printf("/proc/%d/stat", term->pid), &stat, &length, NULL)) {
      stats = g_strsplit_set(stat, " ", 8);
      if (atoi(stats[7]) != term->pid) {
        GtkWidget *dialog;
        GtkWidget *dialog_hbox;
        GtkWidget *dialog_icon;
        GtkWidget *dialog_string;
#if BUTTON_ORDER_BY_RCFILE
        if (button_order)
          dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_CLOSE, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        else
#endif
          dialog = gtk_dialog_new_with_buttons(LABEL_DIALOG_CLOSE, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_CLOSE, GTK_RESPONSE_CLOSE, NULL);
        gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);
        dialog_hbox = gtk_hbox_new (0, 0);
        dialog_icon = gtk_image_new_from_stock (GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_DIALOG);
        dialog_string = gtk_label_new (LABEL_DIALOG_CLOSE);
        gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), dialog_hbox);
        gtk_box_pack_start(GTK_BOX(dialog_hbox), dialog_icon, 0, 0, 0);
        gtk_box_pack_start(GTK_BOX(dialog_hbox), dialog_string, 1, 0, 0);
        gtk_widget_show_all(dialog);
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_CLOSE) {
          gtk_widget_destroy(dialog);
          user_want_to_close = 1;
        } else {
          gtk_widget_destroy(dialog);
          return;
        }
      }
    }
#endif /* Maybe only work on GNU/Linux */
  }
#endif /* CLOSE_DIALOG */

#if CLOSE_SAFELY
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), index);
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
#if CLOSE_DIALOG
  if (!user_want_to_close && do_close_dialog) {
#endif
#if 1 /* Maybe only work on GNU/Linux */
    char *stat = NULL;
    char **stats = NULL;
    gsize length;
    if (g_file_get_contents(g_strdup_printf("/proc/%d/stat", term->pid), &stat, &length, NULL)) {
      stats = g_strsplit_set(stat, " ", 8);
      if (atoi(stats[7]) != term->pid)
        return;
      else
        kill(term->pid, 9);
    }
#endif
#if CLOSE_DIALOG
  } else
    kill(term->pid, 9);
#endif
#endif /* CLOSE_SAFELY */

#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  g_free(term);

  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < 2)
    gtk_main_quit();
  else
    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), index);

#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif

#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) {
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
#endif
}

static void set_encoding(GtkWidget *widget, void *data)
{
#if TAB
  current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
#endif
  vte_terminal_set_encoding(VTE_TERMINAL(term->vte), (char*)data);
#if STATUS_BAR
  change_statusbar_encoding();
#endif
}

static void switch_page()
{
#if STATUS_BAR
  change_statusbar_encoding();
#endif
#if TAB_SHOW_INFO_AT_TITLE
  int index = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
  if (index == 1)
    gtk_window_set_title(GTK_WINDOW(main_window), VTE_PROGRAM_NAME);
  else {
    char tabtitle[64];
    sprintf(tabtitle, "%s - tab %d of %d", VTE_PROGRAM_NAME, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) + 1, index);
    gtk_window_set_title(GTK_WINDOW(main_window), tabtitle);
  }
#endif
}

int main(int argc, char **argv)
{
#if COMMAND_EXEC_PROGRAM
  sprintf(default_command, "%s", DEFAULT_COMMAND);
  if (argc > 2) {
    int i = 0;
    for (i = 0 ; i < argc ; i++) {
      if (!strcmp(argv[i], "-e")) {
        if (i < argc - 1)
          sprintf(default_command, "%s", argv[i + 1]);
        if (++i != argc)
          default_argv = &(argv[i]);
      }
    }
  }
#endif

#if COMMAND_SET_TITLE
  sprintf(program_name, "%s", PROGRAM_NAME);
  if (argc > 2 && !strcmp(argv[1], "-title"))
    sprintf(program_name, "%s", argv[2]);
  if (argc > 3 && !strcmp(argv[2], "-title"))
    sprintf(program_name, "%s", argv[3]);
#endif

#if COMMAND_SHOW_VERSION
  if (argc > 1 && !strcmp(argv[1], "-v")) {
    printf("%s, version %s\n", PROGRAM_NAME, EVILVTE_PROGRAM_VERSION);
    return 0;
  }
#endif

#if COMMAND_SHOW_OPTIONS
  if (argc > 1 && !strcmp(argv[1], "-o")) {
    system("showvte");
    return 0;
  }
#endif

#if COMMAND_SHOW_HELP
  if (argc > 1 && !strcmp(argv[1], "-h")) {
    printf("%s, version %s\n\nUsage:\n\t%s [option%s]\n\nOption%s:\n\t-h                    \tshow this help\n", PROGRAM_NAME, EVILVTE_PROGRAM_VERSION, PROGRAM_NAME, COMMAND_SHOW_HELP_OPTION_S, COMMAND_SHOW_HELP_OPTION_S);
#if COMMAND_AT_ROOT_WINDOW
    printf("\t-r                    \tmake %s run in root window\n", PROGRAM_NAME);
#endif
#if COMMAND_EXEC_PROGRAM
    printf("\t-e [program] [options]\tspecifies the program to be run in %s\n", PROGRAM_NAME);
#endif
#if COMMAND_FULLSCREEN
    printf("\t-f                    \tstart %s in fullscreen mode\n", PROGRAM_NAME);
#endif
#if COMMAND_SHOW_OPTIONS
    printf("\t-o                    \tshow build-time configuration\n");
#endif
#if COMMAND_SHOW_VERSION
    printf("\t-v                    \tshow version\n");
#endif
#if COMMAND_TAB_NUMBERS
    printf("\t-2 to -9              \tspecifies the initial tab numbers\n");
#endif
#if COMMAND_SET_TITLE
    printf("\t-title [string]       \tset program title\n");
#endif
#ifdef BACKGROUND_IMAGE
    printf("\nBackground image:\n\t$HOME/%s\n", BACKGROUND_IMAGE);
#endif
    return 0;
  }
#endif

#ifdef BACKGROUND_IMAGE
  sprintf(imgstr, "%s/%s", g_getenv("HOME"), BACKGROUND_IMAGE);
#endif

#ifdef FONT
  sprintf(font_name, "%s", FONT);
  sprintf(font_str, "%s %d", font_name, font_size);
#endif

  gtk_init(NULL, NULL);

#if BUTTON_ORDER_BY_RCFILE
  g_object_get(gtk_settings_get_default(), "gtk-alternative-button-order", &button_order, NULL);
#endif

#ifdef BACKGROUND_TINT_COLOR
#if BACKGROUND_EXIST
  gdk_color_parse(BACKGROUND_TINT_COLOR, &color_tint);
#endif
#endif

  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#if PROGRAM_WM_CLASS
  gtk_window_set_wmclass(GTK_WINDOW(main_window), VTE_PROGRAM_NAME, PROGRAM_NAME);
#endif

#ifdef SHOW_WINDOW_DECORATED
  gtk_window_set_decorated(GTK_WINDOW(main_window), SHOW_WINDOW_DECORATED);
#endif

#if SHOW_WINDOW_TITLE
  gtk_window_set_title(GTK_WINDOW(main_window), VTE_PROGRAM_NAME);
#endif

#if SHOW_WINDOW_ICON
  gtk_window_set_icon_from_file(GTK_WINDOW(main_window), ICON_DIR"/evilvte.png", NULL);
#endif

#if STATUS_BAR
  vbox = gtk_vbox_new(0, 0);
  gtk_container_add(GTK_CONTAINER(main_window), vbox);
#endif

  notebook = gtk_notebook_new();

#ifdef SHOW_WINDOW_BORDER
  gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), SHOW_WINDOW_BORDER);
#endif

#ifdef TAB_BORDER
  gtk_notebook_set_tab_border(GTK_NOTEBOOK(notebook), TAB_BORDER);
#endif

#ifdef TAB_BORDER_VERTICAL
  gtk_notebook_set_tab_hborder(GTK_NOTEBOOK(notebook), TAB_BORDER_VERTICAL);
#endif

#ifdef TAB_BORDER_HORIZONTAL
  gtk_notebook_set_tab_vborder(GTK_NOTEBOOK(notebook), TAB_BORDER_HORIZONTAL);
#endif

#ifdef TABBAR
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), VTE_TABBAR);
#endif

#if TABBAR_SCROLLABLE
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TABBAR_SCROLLABLE);
#endif

#if TABBAR_MENU_SELECT_TAB
  gtk_notebook_popup_enable(GTK_NOTEBOOK(notebook));
#endif

#ifdef TABBAR_PLACE
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), TABBAR_PLACE);
#endif

#if !TAB
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
#endif

#if INIT_OPACITY
  gtk_widget_set_colormap(main_window, gdk_screen_get_rgba_colormap(gtk_widget_get_screen(main_window)));
#endif

  add_tab();

#if COMMAND_EXEC_PROGRAM && TAB
  sprintf(default_command, "%s", DEFAULT_COMMAND);
  default_argv = DEFAULT_ARGV;
#endif

#if COMMAND_TAB_NUMBERS
  if (argc > 1) {
    int i = 1;
    if (!strcmp(argv[1], "-2"))
      i = 2;
    if (!strcmp(argv[1], "-3"))
      i = 3;
    if (!strcmp(argv[1], "-4"))
      i = 4;
    if (!strcmp(argv[1], "-5"))
      i = 5;
    if (!strcmp(argv[1], "-6"))
      i = 6;
    if (!strcmp(argv[1], "-7"))
      i = 7;
    if (!strcmp(argv[1], "-8"))
      i = 8;
    if (!strcmp(argv[1], "-9"))
      i = 9;
    if (argc > 2) {
      if (!strcmp(argv[2], "-2"))
        i = 2;
      if (!strcmp(argv[2], "-3"))
        i = 3;
      if (!strcmp(argv[2], "-4"))
        i = 4;
      if (!strcmp(argv[2], "-5"))
        i = 5;
      if (!strcmp(argv[2], "-6"))
        i = 6;
      if (!strcmp(argv[2], "-7"))
        i = 7;
      if (!strcmp(argv[2], "-8"))
        i = 8;
      if (!strcmp(argv[2], "-9"))
        i = 9;
    }
    while (i > 1) {
      add_tab();
      i--;
    }
    current_tab = gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), 0);
    term = (struct terminal*)g_object_get_data(G_OBJECT(current_tab), "current_tab");
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
    gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);
  }
#endif

#if STATUS_BAR
  gtk_box_pack_start(GTK_BOX(vbox), notebook, 1, 1, 0);
  statusbar = gtk_statusbar_new();
  change_statusbar_encoding();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, 0, 1, 0);
#endif
#if !STATUS_BAR
  gtk_container_add(GTK_CONTAINER(main_window), notebook);
#endif

  g_signal_connect(main_window, "delete_event", delete_event, NULL);

#if CTRL
  g_signal_connect(main_window, "key-press-event", G_CALLBACK(key_press_event), NULL);
#endif

#if TAB
  g_signal_connect_after(notebook, "switch-page", switch_page, NULL);
#endif

#if TAB_REORDERABLE
  g_signal_connect(notebook, "page-reordered", switch_page, NULL);
#endif

#if COMMAND_AT_ROOT_WINDOW
  if (argc > 1 && !strcmp(argv[1], "-r"))
    at_root_window = 1;
  if (argc > 2 && !strcmp(argv[2], "-r"))
    at_root_window = 1;
  if (at_root_window) {
    gtk_window_set_keep_below(GTK_WINDOW(main_window), 1);
    gtk_window_set_decorated(GTK_WINDOW(main_window), 0);
#ifdef CTRL_TOGGLE_DECORATED
    window_decorated_status = 1;
#endif
#if MENU_TOGGLE_DECORATED
    window_decorated_status = 1;
#endif
    gtk_window_set_skip_taskbar_hint(GTK_WINDOW(main_window), 1);
    gtk_window_set_skip_pager_hint(GTK_WINDOW(main_window), 1);
  }
#endif

#ifdef PROGRAM_ALWAYS_ON_TOP
#if COMMAND_AT_ROOT_WINDOW
  if (!at_root_window)
#endif
    gtk_window_set_keep_above(GTK_WINDOW(main_window), always_on_top);
#endif

  gtk_widget_show_all(main_window);

#if COMMAND_FULLSCREEN
  if (argc > 1 && !strcmp(argv[1], "-f")) {
    gtk_window_maximize(GTK_WINDOW(main_window));
#ifdef CTRL_TOGGLE_FULLSCREEN
    window_fullscreen_status = 1;
#endif
#if MENU_TOGGLE_FULLSCREEN
    window_fullscreen_status = 1;
#endif
  }
  if (argc > 2 && !strcmp(argv[2], "-f")) {
    gtk_window_maximize(GTK_WINDOW(main_window));
#ifdef CTRL_TOGGLE_FULLSCREEN
    window_fullscreen_status = 1;
#endif
#if MENU_TOGGLE_FULLSCREEN
    window_fullscreen_status = 1;
#endif
  }
#endif

#if MENU
  menu = gtk_menu_new();
#ifdef MENU_MATCH_STRING_EXEC
  match_open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  gtk_menu_append(menu, match_open);
  g_signal_connect(match_open, "activate", do_match_open, NULL);
  match_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, NULL);
  gtk_menu_append(menu, match_copy);
  g_signal_connect(match_copy, "activate", do_match_copy, NULL);
  match_item = gtk_separator_menu_item_new();
  gtk_menu_append(menu, match_item);
#ifdef MENU_CUSTOM
  menu_item_success += 100;
#endif
#endif
#endif /* MENU */

#ifdef MENU_CUSTOM
  GtkWidget *menu_item;
  static int menu_custom_size = sizeof(menu_custom) / sizeof(menu_custom[0]);
  int m = 0;
  for (m = 0 ; m < menu_custom_size ; m++)
#endif
  {
#if MENU_COPY
    if (menu_custom[m] == "Copy") {
      GtkWidget *menu_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, NULL);
      gtk_menu_append(menu, menu_copy);
      g_signal_connect(menu_copy, "activate", do_copy, NULL);
      menu_item_success++;
    }
#endif

#if MENU_PASTE
    if (menu_custom[m] == "Paste") {
      GtkWidget *menu_paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, NULL);
      gtk_menu_append(menu, menu_paste);
      g_signal_connect(menu_paste, "activate", do_paste, NULL);
      menu_item_success++;
    }
#endif

#if MENU_SELECT_ALL
    if (menu_custom[m] == "Select_all") {
      GtkWidget *menu_select_all = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_ALL, NULL);
      gtk_menu_append(menu, menu_select_all);
      g_signal_connect(menu_select_all, "activate", do_select_all, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TAB_ADD
    if (menu_custom[m] == "Add tab") {
      GtkWidget *menu_add_tab = gtk_image_menu_item_new_from_stock(GTK_STOCK_ADD, NULL);
      gtk_menu_append(menu, menu_add_tab);
      g_signal_connect(menu_add_tab, "activate", add_tab, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TAB_REMOVE
    if (menu_custom[m] == "Remove tab") {
      GtkWidget *menu_close_tab = gtk_image_menu_item_new_from_stock(GTK_STOCK_REMOVE, NULL);
      gtk_menu_append(menu, menu_close_tab);
      g_signal_connect(menu_close_tab, "activate", G_CALLBACK(del_tab), (int*)DO_CLOSE_DIALOG);
      menu_item_success++;
    }
#endif

#if MENU_TAB_EDIT_LABEL
    if (menu_custom[m] == "Edit label") {
      GtkWidget *menu_edit_label = gtk_image_menu_item_new_from_stock(GTK_STOCK_EDIT, NULL);
      gtk_menu_append(menu, menu_edit_label);
      g_signal_connect(menu_edit_label, "activate", do_edit_label, NULL);
      menu_item_success++;
    }
#endif

#if MENU_OPEN_NEW_WINDOW
    if (menu_custom[m] == "New window") {
      GtkWidget *menu_new_window = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
      gtk_menu_append(menu, menu_new_window);
      g_signal_connect(menu_new_window, "activate", do_new_window, NULL);
      menu_item_success++;
    }
#endif

#if MENU_QUIT
    if (menu_custom[m] == "Quit") {
      GtkWidget *menu_quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
      gtk_menu_append(menu, menu_quit);
#if CLOSE_SAFELY || CLOSE_DIALOG
      g_signal_connect(menu_quit, "activate", delete_event, NULL);
#endif
#if !CLOSE_SAFELY && !CLOSE_DIALOG
      g_signal_connect(menu_quit, "activate", gtk_main_quit, NULL);
#endif
      menu_item_success++;
    }
#endif

#if MENU_FONT_BIGGER
    if (menu_custom[m] == "Zoom in") {
      GtkWidget *menu_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
      gtk_menu_append(menu, menu_zoom_in);
      g_signal_connect(menu_zoom_in, "activate", do_zoom_in, NULL);
      menu_item_success++;
    }
#endif

#if MENU_FONT_SMALLER
    if (menu_custom[m] == "Zoom out") {
      GtkWidget *menu_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
      gtk_menu_append(menu, menu_zoom_out);
      g_signal_connect(menu_zoom_out, "activate", do_zoom_out, NULL);
      menu_item_success++;
    }
#endif

#if MENU_FONT_DEFAULT_SIZE
    if (menu_custom[m] == "Zoom default") {
      GtkWidget *menu_zoom_100 = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
      gtk_menu_append(menu, menu_zoom_100);
      g_signal_connect(menu_zoom_100, "activate", do_zoom_100, NULL);
      menu_item_success++;
    }
#endif

#if MENU_RESET_TERMINAL
    if (menu_custom[m] == "Reset terminal") {
      GtkWidget *menu_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_REFRESH, NULL);
      gtk_menu_append(menu, menu_reset);
      g_signal_connect(menu_reset, "activate", do_reset, NULL);
      menu_item_success++;
    }
#endif

#if MENU_RESET_AND_CLEAR
    if (menu_custom[m] == "Reset and clear") {
      GtkWidget *menu_clear = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLEAR, NULL);
      gtk_menu_append(menu, menu_clear);
      g_signal_connect(menu_clear, "activate", do_clear, NULL);
      menu_item_success++;
    }
#endif

#if MENU_FONT_SELECT
    if (menu_custom[m] == "Select font") {
      GtkWidget *menu_font = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_FONT, NULL);
      gtk_menu_append(menu, menu_font);
      g_signal_connect(menu_font, "activate", do_select_font, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_ON_TOP
    if (menu_custom[m] == "Toggle always on top") {
      GtkWidget *menu_toggle_on_top = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_ON_TOP);
      GtkWidget *image_toggle_on_top = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_on_top), image_toggle_on_top);
      gtk_menu_append(menu, menu_toggle_on_top);
      g_signal_connect(menu_toggle_on_top, "activate", do_always_on_top, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_BACKGROUND
    if (menu_custom[m] == "Toggle background") {
      GtkWidget *menu_toggle_bg = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_BG);
      GtkWidget *image_toggle_bg = gtk_image_new_from_stock(GTK_STOCK_DND_MULTIPLE, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_bg), image_toggle_bg);
      gtk_menu_append(menu, menu_toggle_bg);
      g_signal_connect(menu_toggle_bg, "activate", do_toggle_bg, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_HOTKEYS
    if (menu_custom[m] == "Toggle hotkeys locking") {
      GtkWidget *menu_toggle_hotkeys = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_HOTKEYS);
      GtkWidget *image_toggle_hotkeys = gtk_image_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_hotkeys), image_toggle_hotkeys);
      gtk_menu_append(menu, menu_toggle_hotkeys);
      g_signal_connect(menu_toggle_hotkeys, "activate", do_toggle_hotkeys, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_SCROLLBAR
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
    if (menu_custom[m] == "Toggle scrollbar") {
      GtkWidget *menu_toggle_scrollbar = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_SCROLLBAR);
      GtkWidget *image_toggle_scrollbar = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_scrollbar), image_toggle_scrollbar);
      gtk_menu_append(menu, menu_toggle_scrollbar);
      g_signal_connect(menu_toggle_scrollbar, "activate", do_toggle_scrollbar, NULL);
      menu_item_success++;
    }
#endif
#endif

#if MENU_TOGGLE_STATUS_BAR && STATUS_BAR
    if (menu_custom[m] == "Toggle status bar") {
      GtkWidget *menu_toggle_status_bar = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_STATUS_BAR);
      GtkWidget *image_toggle_status_bar = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_status_bar), image_toggle_status_bar);
      gtk_menu_append(menu, menu_toggle_status_bar);
      g_signal_connect(menu_toggle_status_bar, "activate", do_toggle_status_bar, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_TABBAR
    if (menu_custom[m] == "Toggle tabbar") {
      GtkWidget *menu_toggle_tabbar = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_TABBAR);
      GtkWidget *image_toggle_tabbar = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_tabbar), image_toggle_tabbar);
      gtk_menu_append(menu, menu_toggle_tabbar);
      g_signal_connect(menu_toggle_tabbar, "activate", do_toggle_tabbar, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_DECORATED
    if (menu_custom[m] == "Toggle window decorated") {
      GtkWidget *menu_toggle_decorated = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_DECORATED);
      GtkWidget *image_toggle_decorated = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_decorated), image_toggle_decorated);
      gtk_menu_append(menu, menu_toggle_decorated);
      g_signal_connect(menu_toggle_decorated, "activate", do_toggle_decorated, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_FULLSCREEN
    if (menu_custom[m] == "Toggle fullscreen") {
      GtkWidget *menu_toggle_fullscreen = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_FULLSCREEN);
      GtkWidget *image_toggle_fullscreen = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_fullscreen), image_toggle_fullscreen);
      gtk_menu_append(menu, menu_toggle_fullscreen);
      g_signal_connect(menu_toggle_fullscreen, "activate", do_toggle_fullscreen, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_ANTI_ALIAS
    if (menu_custom[m] == "Toggle anti-alias") {
      GtkWidget *menu_toggle_antialias = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_ANTI_ALIAS);
      GtkWidget *image_toggle_antialias = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_antialias), image_toggle_antialias);
      gtk_menu_append(menu, menu_toggle_antialias);
      g_signal_connect(menu_toggle_antialias, "activate", do_toggle_antialias, NULL);
      menu_item_success++;
    }
#endif

#if MENU_CHANGE_SATURATION
    if (menu_custom[m] == "Adjust saturation") {
      GtkWidget *menu_change_saturation = gtk_image_menu_item_new_with_label(LABEL_MENU_SATURATION);
      GtkWidget *image_change_saturation = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_change_saturation), image_change_saturation);
      gtk_menu_append(menu, menu_change_saturation);
      g_signal_connect(menu_change_saturation, "activate", do_menu_saturation, NULL);
      menu_item_success++;
    }
#endif

#if MENU_COLOR_BACKGROUND
    if (menu_custom[m] == "Background tint") {
      GtkWidget *menu_change_tint = gtk_image_menu_item_new_with_label(LABEL_DIALOG_BACKGROUND_TINT);
      GtkWidget *image_change_tint = gtk_image_new_from_stock(GTK_STOCK_SELECT_COLOR, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_change_tint), image_change_tint);
      gtk_menu_append(menu, menu_change_tint);
      g_signal_connect(menu_change_tint, "activate", do_menu_tint_color, NULL);
      menu_item_success++;
    }
#endif

#if MENU_SEPARATOR
    if (menu_custom[m] == "Separator") {
      menu_item = gtk_separator_menu_item_new();
      gtk_menu_append(menu, menu_item);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Encoding list")
#endif
#ifdef MENU_ENCODING_LIST
    {
      static int encoding_size = sizeof(encoding) / sizeof(encoding[0]);
      GtkWidget *encoding_item[encoding_size];
#ifdef MENU_CUSTOM
      GtkWidget *image_menu_enc;
#endif
      int i = 0;
      for (i = 0 ; i < encoding_size ; i++) {
#if MENU_DEFAULT_ENCODING
        if (encoding[i] == "Default Encoding") {
          encoding[i] = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term->vte));
#ifdef MENU_CUSTOM
          encoding_item[i] = gtk_image_menu_item_new_with_label(LABEL_DEFAULT_ENCODING);
          image_menu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
          gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(encoding_item[i]), image_menu_enc);
#endif
#ifndef MENU_CUSTOM
          encoding_item[i] = gtk_menu_item_new_with_label(LABEL_DEFAULT_ENCODING);
#endif
        } else {
#endif /* MENU_DEFAULT_ENCODING */
#ifdef MENU_CUSTOM
          encoding_item[i] = gtk_image_menu_item_new_with_label(encoding[i]);
          image_menu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
          gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(encoding_item[i]), image_menu_enc);
#endif
#ifndef MENU_CUSTOM
          encoding_item[i] = gtk_menu_item_new_with_label(encoding[i]);
#endif
#if MENU_DEFAULT_ENCODING
        }
#endif
        gtk_menu_append(menu, encoding_item[i]);
        g_signal_connect(encoding_item[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
#ifdef MENU_CUSTOM
        menu_item_success++;
#endif
      }
    }
#endif /* MENU_ENCODING_LIST */
#if MENU_ENCODING_DEFAULT
    {
#ifdef MENU_CUSTOM
      encoding_item = gtk_image_menu_item_new_with_label("UTF-8");
      GtkWidget *image_menu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(encoding_item), image_menu_enc);
#endif
#ifndef MENU_CUSTOM
      encoding_item = gtk_menu_item_new_with_label("UTF-8");
#endif
      gtk_menu_append(menu, encoding_item);
      g_signal_connect(encoding_item, "activate", G_CALLBACK(set_encoding), "UTF-8");
#ifdef MENU_CUSTOM
      menu_item_success++;
#endif
    }
#endif /* MENU_ENCODING_DEFAULT */

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Input method")
#endif
#if MENU_INPUT_METHOD
    {
      vte_terminal_im_append_menuitems(VTE_TERMINAL(term->vte), GTK_MENU_SHELL(menu));
#ifdef MENU_CUSTOM
      menu_item_success++;
#endif
    }
#endif

#if SUBMENU_ENCODING_LIST
    if (menu_custom[m] == "Submenu encoding list")
#ifdef MENU_ENCODING_LIST
    {
      GtkWidget *subitem_enc = gtk_image_menu_item_new_with_label(LABEL_SUBMENU_ENCODING);
      GtkWidget *submenu_enc = gtk_menu_new();
      GtkWidget *image_submenu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), subitem_enc);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(subitem_enc), image_submenu_enc);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(subitem_enc), submenu_enc);
      static int encoding_size = sizeof(encoding) / sizeof(encoding[0]);
      GtkWidget *encoding_sub[encoding_size];
      int i = 0;
      for (i = 0 ; i < encoding_size ; i++) {
#if MENU_DEFAULT_ENCODING
        if (encoding[i] == "Default Encoding") {
          encoding[i] = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term->vte));
          encoding_sub[i] = gtk_menu_item_new_with_label(LABEL_DEFAULT_ENCODING);
        } else
#endif
          encoding_sub[i] = gtk_menu_item_new_with_label(encoding[i]);
        gtk_menu_append(submenu_enc, encoding_sub[i]);
        g_signal_connect(encoding_sub[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
        menu_item_success++;
      }
    }
#endif
#if MENU_ENCODING_DEFAULT
    {
      GtkWidget *subitem_enc = gtk_image_menu_item_new_with_label(LABEL_SUBMENU_ENCODING);
      GtkWidget *submenu_enc = gtk_menu_new();
      GtkWidget *image_submenu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), subitem_enc);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(subitem_enc), image_submenu_enc);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(subitem_enc), submenu_enc);
      encoding_item = gtk_menu_item_new_with_label("UTF-8");
      gtk_menu_append(submenu_enc, encoding_item);
      g_signal_connect(encoding_item, "activate", G_CALLBACK(set_encoding), "UTF-8");
      menu_item_success++;
    }
#endif
#endif /* SUBMENU_ENCODING_LIST */

#if SUBMENU_INPUT_METHOD
    if (menu_custom[m] == "Submenu input method") {
      GtkWidget *subitem_ime = gtk_image_menu_item_new_with_label(LABEL_SUBMENU_IME);
      GtkWidget *submenu_ime = gtk_menu_new();
      GtkWidget *image_submenu_ime = gtk_image_new_from_stock(GTK_STOCK_INFO, GTK_ICON_SIZE_MENU);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), subitem_ime);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(subitem_ime), image_submenu_ime);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(subitem_ime), submenu_ime);
      vte_terminal_im_append_menuitems(VTE_TERMINAL(term->vte), GTK_MENU_SHELL(submenu_ime));
      menu_item_success++;
    }
#endif
  }

#if MENU
  gtk_widget_show_all(menu);
#endif

#ifdef MENU_MATCH_STRING_EXEC
  gtk_widget_hide(match_open);
  gtk_widget_hide(match_copy);
  gtk_widget_hide(match_item);
#endif

  gtk_main();
  return 0;
}
