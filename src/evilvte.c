/* Forked from sakura 2.0.1, http://www.pleyades.net/david/sakura.php
 * Copyright (C) 2006-2008  David Gómez <david@pleyades.net>
 * Copyright (C) 2008-2010  Wen-Yen Chuang <caleb AT calno DOT com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <ctype.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <libintl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <vte/vte.h>

#ifndef VTE_CHECK_VERSION
#define VTE_CHECK_VERSION(major,minor,micro) \
  (VTE_MAJOR_VERSION > (major) || \
  (VTE_MAJOR_VERSION == (major) && VTE_MINOR_VERSION > (minor)) || \
  (VTE_MAJOR_VERSION == (major) && VTE_MINOR_VERSION == (minor) && VTE_MICRO_VERSION >= (micro)))
#endif

#define AUTO            VTE_ERASE_AUTO
#define BACKSPACE       VTE_ERASE_ASCII_BACKSPACE
#define DELETE          VTE_ERASE_ASCII_DELETE
#define DELETE_SEQUENCE VTE_ERASE_DELETE_SEQUENCE
#define ERASE_TTY       VTE_ERASE_AUTO
#if VTE_CHECK_VERSION(0,20,4)
#undef ERASE_TTY
#define ERASE_TTY       VTE_ERASE_TTY
#endif

#define BLOCK     VTE_CURSOR_SHAPE_BLOCK
#define IBEAM     VTE_CURSOR_SHAPE_IBEAM
#define UNDERLINE VTE_CURSOR_SHAPE_UNDERLINE

#define LINUX     1
#define RXVT      2
#define TANGO     3
#define VTE_FIXED 4
#define XTERM     5

#define LEFT   0
#define RIGHT  1
#define TOP    2
#define BOTTOM 3

#define CTRL event->keyval ==

#include "custom.h"
#include "evilvte.h"

#if !GTK_CHECK_VERSION(2,7,1)
#define GTK_STOCK_INFO GTK_STOCK_BOLD
#define GTK_STOCK_LEAVE_FULLSCREEN GTK_STOCK_YES
#endif

#if !GTK_CHECK_VERSION(2,10,0)
#undef TAB_REORDERABLE
#endif

#if !GTK_CHECK_VERSION(2,13,4)
#define gtk_color_selection_dialog_get_color_selection(x) (x)->colorsel
#define gtk_dialog_get_content_area(x) (x)->vbox
#endif

#if !VTE_CHECK_VERSION(0,11,0)
#undef BACKGROUND_SCROLLABLE
#undef BACKGROUND_TINT_COLOR
#undef HOTKEY_COLOR_BACKGROUND
#undef MENU_COLOR_BACKGROUND
#endif

#if !VTE_CHECK_VERSION(0,11,11)
#undef COLOR_TEXT_HIGHLIGHTED
#undef CURSOR_COLOR
#undef FONT_ANTI_ALIAS
#undef HOTKEY_TOGGLE_ANTI_ALIAS
#undef MENU_TOGGLE_ANTI_ALIAS
#endif

#if !VTE_CHECK_VERSION(0,13,3)
#undef BACKGROUND_OPACITY
#undef TOGGLE_BG_OPACITY
#endif

#if !VTE_CHECK_VERSION(0,16,0)
#undef HOTKEY_SELECT_ALL
#undef MENU_SELECT_ALL
#endif

#if VTE_CHECK_VERSION(0,17,1) && (COLOR_STYLE == VTE_FIXED)
#undef COLOR_STYLE
#endif

#define CURRENT_TAB(x) gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), (x))

#ifdef CURSOR_BLINKS
#if CURSOR_BLINKS
#define VTE_CURSOR_BLINKS VTE_CURSOR_BLINK_ON
#endif
#if !CURSOR_BLINKS
#define VTE_CURSOR_BLINKS VTE_CURSOR_BLINK_OFF
#endif
#endif

#ifndef DEFAULT_COMMAND
#if COMMAND_EXEC_PROGRAM
#define DEFAULT_COMMAND g_getenv("SHELL")
#endif
#if !COMMAND_EXEC_PROGRAM
#define DEFAULT_COMMAND NULL
#endif
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

#ifdef CLOSE_DIALOG
#if CLOSE_DIALOG
#define DEL_TAB(major,minor) del_tab(major,minor)
#endif
#if !CLOSE_DIALOG
#undef CLOSE_DIALOG
#endif
#endif

#ifndef CLOSE_DIALOG
#define CLOSE_DIALOG 0
#define DEL_TAB(major,minor) del_tab()
#endif

#if DEF_TAB_LABEL && !defined(TAB_LABEL)
#define TAB_LABEL "Page %u"
#endif

#if !TAB
#undef HOTKEY_TAB_ADD
#undef HOTKEY_TAB_REMOVE
#undef HOTKEY_TAB_PREVIOUS
#undef HOTKEY_TAB_NEXT
#undef HOTKEY_TAB_FIRST
#undef HOTKEY_TAB_LAST
#undef HOTKEY_TAB_GO_TO_NUMBER
#undef HOTKEY_TAB_EDIT_LABEL
#undef HOTKEY_TOGGLE_TABBAR
#undef MENU_TAB_ADD
#undef MENU_TAB_REMOVE
#undef MENU_TAB_EDIT_LABEL
#undef MENU_TOGGLE_TABBAR
#undef SHOW_WINDOW_BORDER
#undef TAB_BORDER
#undef TAB_BORDER_HORIZONTAL
#undef TAB_BORDER_VERTICAL
#undef TAB_CLOSE_BUTTON
#undef TAB_EXPANDED_WIDTH
#undef COMMAND_TAB_NUMBERS
#undef TAB_LABEL
#undef TAB_LABEL_DYNAMIC
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

#define GET_VTE_CHILD_PID
#if TAB_NEW_PATH_EQUAL_OLD || CLOSE_DIALOG || CLOSE_SAFELY
#undef GET_VTE_CHILD_PID
#define GET_VTE_CHILD_PID term->pid =
#endif

#if TAB_NEW_PATH_EQUAL_OLD
#ifndef DEFAULT_DIRECTORY
#define DEFAULT_DIRECTORY g_get_current_dir()
#endif
#define VTE_DEFAULT_DIRECTORY default_directory
#endif

#if !TAB_NEW_PATH_EQUAL_OLD
#ifndef DEFAULT_DIRECTORY
#define DEFAULT_DIRECTORY NULL
#endif
#define VTE_DEFAULT_DIRECTORY DEFAULT_DIRECTORY
#endif

#if CTRL_ALT
#undef HOTKEY
#define HOTKEY TRUE
#endif

#if !HOTKEY
#undef HOTKEY_COPY
#undef HOTKEY_PASTE
#undef HOTKEY_SELECT_ALL
#undef HOTKEY_COLOR_BACKGROUND
#undef HOTKEY_EDIT_ENCODING
#undef HOTKEY_FONT_BIGGER
#undef HOTKEY_FONT_SMALLER
#undef HOTKEY_FONT_DEFAULT_SIZE
#undef HOTKEY_FONT_SELECT
#undef HOTKEY_OPEN_NEW_WINDOW
#undef HOTKEY_RESET_TERMINAL
#undef HOTKEY_RESET_AND_CLEAR
#undef HOTKEY_SATURATION_DIALOG
#undef HOTKEY_SATURATION_MORE
#undef HOTKEY_SATURATION_LESS
#undef HOTKEY_TAB_ADD
#undef HOTKEY_TAB_REMOVE
#undef HOTKEY_TAB_PREVIOUS
#undef HOTKEY_TAB_NEXT
#undef HOTKEY_TAB_FIRST
#undef HOTKEY_TAB_LAST
#undef HOTKEY_TAB_GO_TO_NUMBER
#undef HOTKEY_TAB_EDIT_LABEL
#undef HOTKEY_TOGGLE_ANTI_ALIAS
#undef HOTKEY_TOGGLE_DECORATED
#undef HOTKEY_TOGGLE_FULLSCREEN
#undef HOTKEY_TOGGLE_HOTKEYS
#undef HOTKEY_TOGGLE_ON_TOP
#undef HOTKEY_TOGGLE_SCROLLBAR
#undef HOTKEY_TOGGLE_STATUS_BAR
#undef HOTKEY_TOGGLE_TABBAR
#undef HOTKEY_TOGGLE_BACKGROUND
#endif

#if BUTTON_ORDER_BY_RCFILE
int button_order = 0;
#endif

#define MENU_INPUT_METHOD 1

#if !MENU
#undef MENU_INPUT_METHOD
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

#if defined(HOTKEY_TOGGLE_ON_TOP) || MENU_TOGGLE_ON_TOP
#ifndef PROGRAM_ALWAYS_ON_TOP
#define PROGRAM_ALWAYS_ON_TOP 0
#endif
#endif

#if defined(HOTKEY_SATURATION_DIALOG) || MENU_CHANGE_SATURATION
GtkWidget *adjustment;
#endif

#if defined(HOTKEY_TOGGLE_HOTKEYS) || MENU_TOGGLE_HOTKEYS
int hotkey_status = 0;
#endif

#ifdef MENU_ENCODING_LIST
char *encoding[] = { MENU_ENCODING_LIST };
#endif

#if !defined(MENU_ENCODING_LIST) && MENU
GtkWidget *encoding_item;
#endif

#if TOGGLE_BG_ORDER_SIZE == 1
#undef HOTKEY_TOGGLE_BACKGROUND
#undef MENU_TOGGLE_BACKGROUND
#endif

#if defined(HOTKEY_TOGGLE_BACKGROUND) || MENU_TOGGLE_BACKGROUND
#ifndef BACKGROUND_IMAGE
#define BACKGROUND_IMAGE ".config/evilvte/background.png"
#endif
#ifndef TOGGLE_BG_ORDER
#if !VTE_CHECK_VERSION(0,13,3)
#define TOGGLE_BG_ORDER "Image", "Transparent", "No background"
#define TOGGLE_BG_ORDER_SIZE 3
#endif
#if VTE_CHECK_VERSION(0,13,3)
#define TOGGLE_BG_ORDER "Image", "Transparent", "No background", "Opacity"
#define TOGGLE_BG_ORDER_SIZE 4
#define TOGGLE_BG_OPACITY 1
#endif
#define TOGGLE_BG_IMAGE 1
#define TOGGLE_BG_TRANSPARENT 1
#define TOGGLE_BG_NO_BACKGROUND 1
#endif
#if TOGGLE_BG_OPACITY
#define INIT_OPACITY 1
#endif
#define DO_TOGGLE_BACKGROUND 1
const char *background_order[] = { TOGGLE_BG_ORDER };
int background_status = 0;
#endif

#ifdef BACKGROUND_IMAGE
char imgstr[sizeof(BACKGROUND_IMAGE) + 64];
#endif

#ifdef PROGRAM_ICON
char iconstr[sizeof(PROGRAM_ICON) + 64];
#endif

#if defined(BACKGROUND_IMAGE) || BACKGROUND_TRANSPARENT || TOGGLE_BG_TRANSPARENT
#define BACKGROUND_EXIST 1
#endif

#if defined(HOTKEY_COLOR_BACKGROUND) || MENU_COLOR_BACKGROUND
#ifndef BACKGROUND_TINT_COLOR
#define BACKGROUND_TINT_COLOR "black"
#endif
#endif

#if defined(BACKGROUND_TINT_COLOR) && BACKGROUND_EXIST
GdkColor color_tint;
#endif

#if defined(HOTKEY_TOGGLE_SCROLLBAR) || MENU_TOGGLE_SCROLLBAR
#ifdef SCROLLBAR
#define DO_TOGGLE_SCROLLBAR 1
int scrollbar_status = 0;
#endif
#endif

#ifdef SCROLLBAR
#define VTE_HBOX term->hbox
#endif
#ifndef SCROLLBAR
#define VTE_HBOX term->vte
#endif

#if defined(HOTKEY_TOGGLE_STATUS_BAR) || MENU_TOGGLE_STATUS_BAR
#ifdef STATUS_BAR
#define DO_TOGGLE_STATUS_BAR 1
int status_bar_status = 0;
#endif
#endif

#if defined(HOTKEY_TOGGLE_DECORATED) || MENU_TOGGLE_DECORATED
#ifdef SHOW_WINDOW_DECORATED
int window_decorated_status = !SHOW_WINDOW_DECORATED;
#endif
#ifndef SHOW_WINDOW_DECORATED
int window_decorated_status = 0;
#endif
#endif

#if defined(HOTKEY_TOGGLE_FULLSCREEN) || MENU_TOGGLE_FULLSCREEN
int window_fullscreen_status = 0;
#endif

#if defined(HOTKEY_TOGGLE_ANTI_ALIAS) || MENU_TOGGLE_ANTI_ALIAS
#if defined(FONT_ANTI_ALIAS) && !FONT_ANTI_ALIAS
int antialias_status = VTE_ANTI_ALIAS_FORCE_DISABLE;
#endif
#if !defined(FONT_ANTI_ALIAS) || FONT_ANTI_ALIAS
int antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#endif
#endif

#ifdef MENU_CUSTOM
const char *menu_custom[] = { MENU_CUSTOM };
int menu_item_success = 0;
#endif

#if defined(HOTKEY_TOGGLE_TABBAR) || MENU_TOGGLE_TABBAR
#define INT_TABBAR_STATUS 1
#endif

#if TABBAR_AUTOHIDE && !defined(TABBAR)
#define TABBAR 1
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

#if defined(MATCH_STRING_EXEC) && !defined(MATCH_STRING_L)
#define MATCH_STRING_L MATCH_STRING_EXEC
#endif

#if !defined(MATCH_STRING)
#if defined(MENU_MATCH_STRING_EXEC) || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
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

#define EVILVTE_SET_FONT(major,minor,micro) vte_terminal_set_font_from_string(major,minor)
#if defined(FONT_ANTI_ALIAS) || MENU_TOGGLE_ANTI_ALIAS || defined(HOTKEY_TOGGLE_ANTI_ALIAS)
#undef EVILVTE_SET_FONT
#define EVILVTE_SET_FONT(major,minor,micro) vte_terminal_set_font_from_string_full(major,minor,micro)
#endif

#if PROGRAM_WM_CLASS || MENU_OPEN_NEW_WINDOW || COMMAND_SHOW_VERSION || COMMAND_SHOW_HELP || defined(HOTKEY_OPEN_NEW_WINDOW) || COMMAND_SET_TITLE || TAB_SHOW_INFO_AT_TITLE
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#ifndef PROGRAM_VERSION
#define PROGRAM_VERSION EVILVTE_VERSION
#endif

#if COMMAND_SET_TITLE
#define VTE_PROGRAM_NAME program_name
char *program_name = PROGRAM_NAME;
#endif

#if !COMMAND_SET_TITLE
#define VTE_PROGRAM_NAME PROGRAM_NAME
#endif

#if defined(COLOR_BACKGROUND) || defined(COLOR_TEXT_BOLD) || defined(CURSOR_COLOR) || defined(COLOR_TEXT_DIM) || defined(COLOR_FOREGROUND) || defined(COLOR_TEXT_HIGHLIGHTED)
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_STYLE
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 0
#endif

GtkWidget *main_window;
GtkWidget *notebook;

#define VTE_LABEL NULL
#if defined(TAB_LABEL) || defined(TAB_LABEL_CUSTOM)
#undef VTE_LABEL
#define VTE_LABEL label
#endif

#if TAB_CLOSE_BUTTON
#ifndef TAB_LABEL
#define TAB_LABEL "Page %u"
#endif
#undef VTE_LABEL
#define VTE_LABEL term->label
#endif

#ifdef TAB_LABEL_CUSTOM
#undef TAB_LABEL
const char *label_style_custom[] = { TAB_LABEL_CUSTOM };
const int label_style_size = sizeof(label_style_custom) / sizeof(label_style_custom[0]);
#endif

#ifdef COLOR_STYLE
const GdkColor color_style[16] = {
#if COLOR_STYLE == LINUX
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
#endif
#if COLOR_STYLE == RXVT
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
#endif
#if COLOR_STYLE == TANGO
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
#endif
#if COLOR_STYLE == VTE_FIXED
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
#endif
#if COLOR_STYLE == XTERM
  { 0, 0x0000, 0x0000, 0x0000 },
  { 0, 0xcdcb, 0x0000, 0x0000 },
  { 0, 0x0000, 0xcdcb, 0x0000 },
  { 0, 0xcdcb, 0xcdcb, 0x0000 },
  { 0, 0x1e1a, 0x908f, 0xffff },
  { 0, 0xcdcb, 0x0000, 0xcdcb },
  { 0, 0x0000, 0xcdcb, 0xcdcb },
  { 0, 0xe5e2, 0xe5e2, 0xe5e2 },
  { 0, 0x4ccc, 0x4ccc, 0x4ccc },
  { 0, 0xffff, 0x0000, 0x0000 },
  { 0, 0x0000, 0xffff, 0x0000 },
  { 0, 0xffff, 0xffff, 0x0000 },
  { 0, 0x4645, 0x8281, 0xb4ae },
  { 0, 0xffff, 0x0000, 0xffff },
  { 0, 0x0000, 0xffff, 0xffff },
  { 0, 0xffff, 0xffff, 0xffff }
#endif
};
#endif

#if defined(FONT_ANTI_ALIAS) || MENU_FONT_BIGGER || MENU_FONT_SMALLER || MENU_FONT_SELECT || MENU_TOGGLE_ANTI_ALIAS || defined(HOTKEY_TOGGLE_ANTI_ALIAS) || defined(HOTKEY_FONT_BIGGER) || defined(HOTKEY_FONT_SMALLER) || defined(HOTKEY_FONT_SELECT)
#define FONT_CHANGE_SIZE 1
#ifndef FONT
#define FONT "Monospace 10"
#endif
#endif

#if !FONT_CHANGE_SIZE
#undef MENU_FONT_DEFAULT_SIZE
#undef HOTKEY_FONT_DEFAULT_SIZE
#endif

#ifdef FONT
char font_name[125];
char font_str[128];
int font_size;
int font_size_default;
const char *number_char[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
#endif

#if COMMAND_EXEC_PROGRAM
#define VTE_DEFAULT_COMMAND default_command
#define DEFAULT_ARGV default_argv
char default_command[256];
char **default_argv = NULL;
#endif

#if !COMMAND_EXEC_PROGRAM
#define VTE_DEFAULT_COMMAND DEFAULT_COMMAND
#define DEFAULT_ARGV NULL
#endif

#if COMMAND_LOGIN_SHELL
char *login_shell[] = { "-", NULL };
int login_shell_flag = 0;
#endif

#if defined(HOTKEY_SATURATION_MORE) || defined(HOTKEY_SATURATION_LESS) || MOUSE_CTRL_SATURATION || BACKGROUND_OPACITY || defined(HOTKEY_SATURATION_DIALOG) || MENU_CHANGE_SATURATION || defined(HOTKEY_TOGGLE_BACKGROUND) || MENU_TOGGLE_BACKGROUND
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
int at_root_window = 0;
#endif

#if MENU
GtkWidget *menu;
#endif

#ifdef MENU_MATCH_STRING_EXEC
GtkWidget *match_open;
GtkWidget *match_copy;
GtkWidget *match_item;
#endif

#if defined(MENU_MATCH_STRING_EXEC) || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
char *matched_url = (char*)NULL;
#endif

#if STATUS_BAR
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

struct terminal {
  GtkWidget *vte;
#if CLOSE_DIALOG || CLOSE_SAFELY || TAB_NEW_PATH_EQUAL_OLD
  int pid;
#endif
#if TAB_CLOSE_BUTTON
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *label_edit;
#endif
#ifdef SCROLLBAR
  GtkWidget *hbox;
  GtkWidget *scrollbar;
#endif
#if defined(HOTKEY_TAB_EDIT_LABEL) || MENU_TAB_EDIT_LABEL
  int label_exist;
#endif
};

struct terminal *term;

void DEL_TAB(GtkWidget *widget, int do_close_dialog)
{
#if TAB
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
#endif
#if CLOSE_DIALOG
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(index)), "current_tab");
#endif
  int user_want_to_close = 0;
  if (do_close_dialog) {
    /* Maybe only work on GNU/Linux */
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
        dialog_hbox = gtk_hbox_new(0, 0);
        dialog_icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_DIALOG);
        dialog_string = gtk_label_new(LABEL_DIALOG_CLOSE);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), dialog_hbox);
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
  }
#endif /* CLOSE_DIALOG */

#if CLOSE_SAFELY
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(index)), "current_tab");
#endif
#if CLOSE_DIALOG
  if (!user_want_to_close && do_close_dialog) {
#endif
    /* Maybe only work on GNU/Linux */
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
#if CLOSE_DIALOG
  } else
    kill(term->pid, 9);
#endif
#endif /* CLOSE_SAFELY */

#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  g_free(term);

#if TAB
  (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < 2) ? gtk_main_quit() : gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), index);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif
#if !TAB
  gtk_main_quit();
#endif

#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) {
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
#endif
}

#if TAB_CLOSE_BUTTON
void button_clicked(GtkWidget *widget, void *data)
{
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
  int i = 0;
  int killed = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
    if (data == term->button) {
      gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), i);
      DEL_TAB(NULL, CLOSE_DIALOG);
      killed = i;
    }
  }
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), (killed < index) ? index - 1 : index);
}
#endif

#if WINDOW_TITLE_DYNAMIC || TAB_LABEL_DYNAMIC
void do_title_changed()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
#if TAB_LABEL_DYNAMIC
#if TAB_CLOSE_BUTTON
  term->label = gtk_hbox_new(0, 0);
  term->button = gtk_button_new();
  GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  GtkRcStyle *rcstyle = gtk_rc_style_new();
  gtk_button_set_image(GTK_BUTTON(term->button), img_button);
  gtk_button_set_relief(GTK_BUTTON(term->button), GTK_RELIEF_NONE);
  rcstyle->xthickness = 0;
  rcstyle->ythickness = 0;
  gtk_widget_modify_style(term->button, rcstyle);
  g_object_unref(rcstyle);
  term->label_edit = gtk_label_new(vte_terminal_get_window_title(VTE_TERMINAL(term->vte)));
  gtk_box_pack_start(GTK_BOX(term->label), term->label_edit, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(term->label), term->button, 0, 0, 0);
  gtk_widget_show_all(term->label);
  g_signal_connect(term->button, "clicked", G_CALLBACK(button_clicked), term->button);
  gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), VTE_HBOX, term->label);
#endif
#if !TAB_CLOSE_BUTTON
  gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook), VTE_HBOX, vte_terminal_get_window_title(VTE_TERMINAL(term->vte)));
#endif
#endif
#if WINDOW_TITLE_DYNAMIC
  gtk_window_set_title(GTK_WINDOW(main_window), vte_terminal_get_window_title(VTE_TERMINAL(term->vte)));
#endif
}
#endif

#if MENU || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
int menu_popup(GtkWidget *widget, GdkEventButton *event)
{
#if defined(MENU_MATCH_STRING_EXEC) || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
  int tag = -1;
#endif

#ifdef MATCH_STRING_L
  if (event->button == 1)
    goto match_string;
#endif

#ifdef MATCH_STRING_M
  if (event->button == 2)
    goto match_string;
#endif

#if MENU
  if (event->button == 3) {
#ifdef MENU_MATCH_STRING_EXEC
#if TAB
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
      matched_url = vte_terminal_match_check(VTE_TERMINAL(term->vte), event->x / vte_terminal_get_char_width(VTE_TERMINAL(term->vte)), event->y / vte_terminal_get_char_height(VTE_TERMINAL(term->vte)), &tag);
      if (matched_url != NULL) {
        gtk_widget_show_all(menu);
#ifdef MENU_CUSTOM
        if (menu_item_success == 100)
          gtk_widget_hide(match_item);
#endif
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
      } else {
        gtk_widget_hide(match_open);
        gtk_widget_hide(match_copy);
        gtk_widget_hide(match_item);
#ifdef MENU_CUSTOM
        if (menu_item_success > 100)
#endif
          gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
      }
#endif /* MENU_MATCH_STRING_EXEC */

#ifndef MENU_MATCH_STRING_EXEC
#ifdef MENU_CUSTOM
      if (menu_item_success > 0)
#endif
        gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event->button, event->time);
#endif
      return TRUE;
  }
  return FALSE;
#endif /* MENU */

#if defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
match_string:
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  matched_url = vte_terminal_match_check(VTE_TERMINAL(term->vte), event->x / vte_terminal_get_char_width(VTE_TERMINAL(term->vte)), event->y / vte_terminal_get_char_height(VTE_TERMINAL(term->vte)), &tag);
  if (matched_url != NULL) {
    char new_window_str[256];
#ifdef MATCH_STRING_L
    if (event->button == 1)
      g_snprintf(new_window_str, sizeof(new_window_str), "%s '%s' &", MATCH_STRING_L, matched_url);
#endif
#ifdef MATCH_STRING_M
    if (event->button == 2)
      g_snprintf(new_window_str, sizeof(new_window_str), "%s '%s' &", MATCH_STRING_M, matched_url);
#endif
    system(new_window_str);
    matched_url = NULL;
    return TRUE;
  }
#endif
  return FALSE;
}
#endif

#if MOUSE_CTRL_SATURATION
int scroll_event(GtkWidget *widget, GdkEventScroll *event)
{
#if HOTKEY
#if !CTRL_ALT
  if (event->state & GDK_CONTROL_MASK) {
#endif
#if CTRL_ALT
  if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_MOD1_MASK)) {
#endif
    if (event->direction == GDK_SCROLL_UP) {
      saturation_level += 0.025;
      if (saturation_level > 1)
        saturation_level = 1;
#if TAB
      int i = 0;
      for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
        vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
        vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
        if (!strncmp(background_order[background_status], "Opacity", 8))
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
      }
#endif
      return TRUE;
    }
    if (event->direction == GDK_SCROLL_DOWN) {
      saturation_level -= 0.025;
      if (saturation_level < 0)
        saturation_level = 0;
#if TAB
      int i = 0;
      for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
        vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
        vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
        if (!strncmp(background_order[background_status], "Opacity", 8))
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
      }
#endif
      return TRUE;
    }
  }
#endif /* HOTKEY */

  if (event->direction == GDK_SCROLL_RIGHT) {
    saturation_level += 0.025;
    if (saturation_level > 1)
      saturation_level = 1;
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
      if (!strncmp(background_order[background_status], "Opacity", 8))
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
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
      if (!strncmp(background_order[background_status], "Opacity", 8))
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

void add_tab()
{
  term = g_malloc(sizeof(struct terminal));

#if defined(HOTKEY_TAB_EDIT_LABEL) || MENU_TAB_EDIT_LABEL
  int label_exist = 0;
#endif

#if defined(TAB_LABEL) || defined(TAB_LABEL_CUSTOM)
  GtkWidget *label;
#endif

#ifdef TAB_LABEL
  label = gtk_label_new(g_strdup_printf(gettext(TAB_LABEL), (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) + 1)));
#if defined(HOTKEY_TAB_EDIT_LABEL) || MENU_TAB_EDIT_LABEL
  label_exist = 1;
#endif
#endif

#ifdef TAB_LABEL_CUSTOM
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < label_style_size) {
    label = gtk_label_new(g_strdup_printf("%s", label_style_custom[gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))]));
#if defined(HOTKEY_TAB_EDIT_LABEL) || MENU_TAB_EDIT_LABEL
    label_exist = 1;
#endif
  } else
    label = NULL;
#endif

#if TAB_CLOSE_BUTTON
  term->label = gtk_hbox_new(0, 0);
  term->button = gtk_button_new();
  GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  GtkRcStyle *rcstyle = gtk_rc_style_new();
  gtk_button_set_image(GTK_BUTTON(term->button), img_button);
  gtk_button_set_relief(GTK_BUTTON(term->button), GTK_RELIEF_NONE);
  rcstyle->xthickness = 0;
  rcstyle->ythickness = 0;
  gtk_widget_modify_style(term->button, rcstyle);
  g_object_unref(rcstyle);
  term->label_edit = label;
  gtk_box_pack_start(GTK_BOX(term->label), term->label_edit, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(term->label), term->button, 0, 0, 0);
  gtk_widget_show_all(term->label);
  g_signal_connect(term->button, "clicked", G_CALLBACK(button_clicked), term->button);
#endif

#ifdef SCROLLBAR
  term->hbox = gtk_hbox_new(0, 0);
#endif

  term->vte = vte_terminal_new();

#ifdef SCROLLBAR
  term->scrollbar = gtk_vscrollbar_new(vte_terminal_get_adjustment(VTE_TERMINAL(term->vte)));
#endif

#ifdef SCROLLBAR
#if !SCROLLBAR
  gtk_box_pack_start(GTK_BOX(term->hbox), term->scrollbar, 0, 0, 0);
#endif
  gtk_box_pack_start(GTK_BOX(term->hbox), term->vte, 1, 1, 0);
#if SCROLLBAR
  gtk_box_pack_start(GTK_BOX(term->hbox), term->scrollbar, 0, 0, 0);
#endif
#endif

#if TAB_NEW_PATH_EQUAL_OLD
  char *default_directory = (char*)DEFAULT_DIRECTORY;
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0) {
    struct terminal *oldterm = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
    default_directory = g_file_read_link(g_strdup_printf("/proc/%d/cwd", oldterm->pid), NULL);
  }
#endif

#if COMMAND_LOGIN_SHELL
  if (login_shell_flag & 1)
    GET_VTE_CHILD_PID vte_terminal_fork_command(VTE_TERMINAL(term->vte), VTE_DEFAULT_COMMAND, login_shell, NULL, VTE_DEFAULT_DIRECTORY, RECORD_LASTLOG, RECORD_UTMP, RECORD_WTMP);
  else
#endif
    GET_VTE_CHILD_PID vte_terminal_fork_command(VTE_TERMINAL(term->vte), VTE_DEFAULT_COMMAND, DEFAULT_ARGV, NULL, VTE_DEFAULT_DIRECTORY, RECORD_LASTLOG, RECORD_UTMP, RECORD_WTMP);

#ifdef MATCH_STRING
#if !VTE_CHECK_VERSION(0,17,1)
  vte_terminal_match_add(VTE_TERMINAL(term->vte), MATCH_STRING);
#endif
#if VTE_CHECK_VERSION(0,17,1)
  vte_terminal_match_add_gregex(VTE_TERMINAL(term->vte), g_regex_new(MATCH_STRING, 0, 0, NULL), 0);
#endif
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
  vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_file(imgstr, NULL));
#endif

#if BACKGROUND_OPACITY
  vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif

#if defined(BACKGROUND_TINT_COLOR) && BACKGROUND_EXIST
  vte_terminal_set_background_tint_color(VTE_TERMINAL(term->vte), &color_tint);
#endif

#if defined(BACKGROUND_SATURATION) && BACKGROUND_EXIST
  vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
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
  vte_terminal_set_colors(VTE_TERMINAL(term->vte), NULL, NULL, color_style, 16);
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
#if !VTE_CHECK_VERSION(0,17,1)
  vte_terminal_set_cursor_blinks(VTE_TERMINAL(term->vte), CURSOR_BLINKS);
#endif
#if VTE_CHECK_VERSION(0,17,1)
  vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(term->vte), VTE_CURSOR_BLINKS);
#endif
#endif

#if defined(CURSOR_SHAPE) && VTE_CHECK_VERSION(0,19,1)
  vte_terminal_set_cursor_shape(VTE_TERMINAL(term->vte), CURSOR_SHAPE);
#endif

#ifdef DEFAULT_ENCODING
  vte_terminal_set_encoding(VTE_TERMINAL(term->vte), DEFAULT_ENCODING);
#endif

#ifdef FONT
#if defined(HOTKEY_TOGGLE_ANTI_ALIAS) || MENU_TOGGLE_ANTI_ALIAS
  EVILVTE_SET_FONT(VTE_TERMINAL(term->vte), font_str, antialias_status);
#endif
#if !defined(HOTKEY_TOGGLE_ANTI_ALIAS) && !MENU_TOGGLE_ANTI_ALIAS
  EVILVTE_SET_FONT(VTE_TERMINAL(term->vte), font_str, VTE_ANTI_ALIAS);
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

#if VTE_COLUMNS && VTE_ROWS
  vte_terminal_set_size(VTE_TERMINAL(term->vte), VTE_COLUMNS, VTE_ROWS);
#endif

#ifdef WORD_CHARS
  vte_terminal_set_word_chars(VTE_TERMINAL(term->vte), WORD_CHARS);
#endif

  gtk_widget_show_all(VTE_HBOX);

#if !TAB
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), VTE_HBOX, VTE_LABEL);
#endif
#if TAB
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), VTE_HBOX, VTE_LABEL);
  g_object_set_data(G_OBJECT(CURRENT_TAB(index)), "current_tab", term);
#endif

  g_signal_connect(term->vte, "child-exited", G_CALLBACK(del_tab), (int*)CLOSE_DIALOG);

#if WINDOW_TITLE_DYNAMIC || TAB_LABEL_DYNAMIC
  g_signal_connect(term->vte, "window-title-changed", do_title_changed, NULL);
#endif

#if MENU || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
  g_signal_connect(term->vte, "button-press-event", G_CALLBACK(menu_popup), NULL);
#endif

#if MOUSE_CTRL_SATURATION
  g_signal_connect(term->vte, "scroll-event", G_CALLBACK(scroll_event), NULL);
#endif

#if TABBAR_AUTOHIDE
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) ? 0 : VTE_TABBAR);
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
#endif

#if DO_TOGGLE_BACKGROUND
#if TOGGLE_BG_TRANSPARENT
  if (!strncmp(background_order[background_status], "Transparent", 12))
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 1);
#endif
#if TOGGLE_BG_IMAGE
  if (!strncmp(background_order[background_status], "Image", 6)) {
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
    vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_file(imgstr, NULL));
  }
#endif
#if TOGGLE_BG_NO_BACKGROUND
  if (!strncmp(background_order[background_status], "No background", 14)) {
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
    vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_xpm_data(NULL));
#if INIT_OPACITY || BACKGROUND_OPACITY
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), 65535);
#endif
  }
#endif
#if TOGGLE_BG_OPACITY
  if (!strncmp(background_order[background_status], "Opacity", 8)) {
    vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
    vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_xpm_data(NULL));
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
  }
#endif
#endif

  gtk_widget_show_all(notebook);

#if DO_TOGGLE_SCROLLBAR
  if (scrollbar_status) {
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      gtk_widget_hide(term->scrollbar);
#if TAB
    }
#endif
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
#endif

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

#if defined(HOTKEY_TAB_EDIT_LABEL) || MENU_TAB_EDIT_LABEL
  term->label_exist = label_exist;
#endif
}

#if STATUS_BAR
void change_statusbar_encoding()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, vte_terminal_get_encoding(VTE_TERMINAL(term->vte)));
}
#endif

#if defined(HOTKEY_SATURATION_DIALOG) || MENU_CHANGE_SATURATION
void do_change_saturation()
{
  saturation_level = gtk_range_get_value(GTK_RANGE(adjustment));
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
  vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
  if (!strncmp(background_order[background_status], "Opacity", 8))
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
}
#endif

#if FONT_CHANGE_SIZE
void font_size_changed()
{
  g_snprintf(font_str, sizeof(font_str), "%s %d", font_name, font_size);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term->vte), font_str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if DO_TOGGLE_SCROLLBAR || DO_TOGGLE_STATUS_BAR
void recover_window_status()
{
#if DO_TOGGLE_SCROLLBAR
  if (scrollbar_status) {
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      gtk_widget_hide(term->scrollbar);
#if TAB
    }
#endif
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
#endif

#if DO_TOGGLE_STATUS_BAR
  if (status_bar_status)
    gtk_widget_hide(statusbar);
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
#endif
}
#endif

#if HOTKEY
int key_press_event(GtkWidget *widget, GdkEventKey *event)
{
#if !CTRL_ALT
  if (event->state & GDK_CONTROL_MASK) {
#endif
#if CTRL_ALT
  if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_MOD1_MASK)) {
#endif

#ifdef HOTKEY_TOGGLE_HOTKEYS
    if (HOTKEY_TOGGLE_HOTKEYS) {
      hotkey_status ^= 1;
      return TRUE;
    }
#endif

#if defined(HOTKEY_TOGGLE_HOTKEYS) || MENU_TOGGLE_HOTKEYS
    if (!hotkey_status) {
#endif

#ifdef HOTKEY_COLOR_BACKGROUND
      if (HOTKEY_COLOR_BACKGROUND) {
        GtkColorSelectionDialog *color_tint_dialog = (GtkColorSelectionDialog*)gtk_color_selection_dialog_new(LABEL_DIALOG_BACKGROUND_TINT);
        gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(color_tint_dialog)), &color_tint);
        if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(color_tint_dialog))) {
          gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(color_tint_dialog)), &color_tint);
#if TAB
          int i = 0;
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
            vte_terminal_set_background_tint_color(VTE_TERMINAL(term->vte), &color_tint);
#if TAB
          }
#endif
        }
        gtk_widget_destroy(GTK_WIDGET(color_tint_dialog));
        return TRUE;
      }
#endif

#ifdef HOTKEY_SATURATION_DIALOG
      if (HOTKEY_SATURATION_DIALOG) {
        double saturation_level_old = saturation_level;
        GtkWidget *saturation_dialog;
#if BUTTON_ORDER_BY_RCFILE
        if (button_order)
          saturation_dialog = gtk_dialog_new_with_buttons(LABEL_MENU_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
        else
#endif
          saturation_dialog = gtk_dialog_new_with_buttons(LABEL_MENU_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
        adjustment = gtk_hscale_new_with_range(0, 1, 0.01);
        gtk_range_set_value(GTK_RANGE(adjustment), saturation_level_old);
        g_signal_connect_after(adjustment, "change-value", do_change_saturation, NULL);
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(saturation_dialog))), adjustment);
        gtk_dialog_set_default_response(GTK_DIALOG(saturation_dialog), GTK_RESPONSE_OK);
        gtk_widget_show_all(saturation_dialog);
        saturation_level = (gtk_dialog_run(GTK_DIALOG(saturation_dialog)) == GTK_RESPONSE_OK) ? gtk_range_get_value(GTK_RANGE(adjustment)) : saturation_level_old;
        gtk_widget_destroy(saturation_dialog);
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
          vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
          if (!strncmp(background_order[background_status], "Opacity", 8))
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_ANTI_ALIAS
      if (HOTKEY_TOGGLE_ANTI_ALIAS) {
        antialias_status++;
        if (antialias_status > VTE_ANTI_ALIAS_FORCE_DISABLE)
          antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
          vte_terminal_set_font_from_string_full(VTE_TERMINAL(term->vte), font_str, antialias_status);
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_DECORATED
      if (HOTKEY_TOGGLE_DECORATED) {
        window_decorated_status ^= 1;
        gtk_window_set_decorated(GTK_WINDOW(main_window), window_decorated_status ? 0 : 1);
#if TAB
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(index)), "current_tab");
#endif
        gtk_widget_hide_all(main_window);
        gtk_widget_show_all(main_window);
#if DO_TOGGLE_SCROLLBAR || DO_TOGGLE_STATUS_BAR
        recover_window_status();
#endif
#if TAB
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif
        gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_FULLSCREEN
      if (HOTKEY_TOGGLE_FULLSCREEN) {
        window_fullscreen_status ^= 1;
        window_fullscreen_status ? gtk_window_maximize(GTK_WINDOW(main_window)) : gtk_window_unmaximize(GTK_WINDOW(main_window));
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_TABBAR
      if (HOTKEY_TOGGLE_TABBAR) {
        tabbar_status ^= 1;
        gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), tabbar_status ? 0 : 1);
#if TABBAR_AUTOHIDE
        if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1)
          gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
#endif
        gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_STATUS_BAR
      if (HOTKEY_TOGGLE_STATUS_BAR) {
        status_bar_status ^= 1;
        status_bar_status ? gtk_widget_hide(statusbar) : gtk_widget_show(statusbar);
        gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
        return TRUE;
      }
#endif

#if defined(HOTKEY_TOGGLE_SCROLLBAR) && defined(SCROLLBAR)
      if (HOTKEY_TOGGLE_SCROLLBAR) {
#if TAB
        int i = 0;
#endif
        scrollbar_status ^= 1;
        if (scrollbar_status) {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
            gtk_widget_hide(term->scrollbar);
#if TAB
          }
#endif
        } else {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
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

#ifdef HOTKEY_OPEN_NEW_WINDOW
      if (HOTKEY_OPEN_NEW_WINDOW) {
        system(PROGRAM_NAME " &");
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_ON_TOP
      if (HOTKEY_TOGGLE_ON_TOP) {
        always_on_top ^= 1;
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

#ifdef HOTKEY_SELECT_ALL
      if (HOTKEY_SELECT_ALL) {
#if TAB
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
        vte_terminal_select_all(VTE_TERMINAL(term->vte));
        return TRUE;
      }
#endif

#ifdef HOTKEY_EDIT_ENCODING
      if (HOTKEY_EDIT_ENCODING) {
#if TAB
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
        const char *encoding_name = vte_terminal_get_encoding(VTE_TERMINAL(term->vte));
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
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(encoding_dialog))), encoding_entry);
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
#endif /* HOTKEY_EDIT_ENCODING */

#ifdef HOTKEY_TAB_EDIT_LABEL
      if (HOTKEY_TAB_EDIT_LABEL) {
        char *label_name = "";
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(index)), "current_tab");
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
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry);
        gtk_widget_show_all(dialog);
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
#if TAB_CLOSE_BUTTON
          term->label = gtk_hbox_new(0, 0);
          term->button = gtk_button_new();
          GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
          GtkRcStyle *rcstyle = gtk_rc_style_new();
          gtk_button_set_image(GTK_BUTTON(term->button), img_button);
          gtk_button_set_relief(GTK_BUTTON(term->button), GTK_RELIEF_NONE);
          rcstyle->xthickness = 0;
          rcstyle->ythickness = 0;
          gtk_widget_modify_style(term->button, rcstyle);
          g_object_unref(rcstyle);
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
#endif /* HOTKEY_TAB_EDIT_LABEL */

#if HOTKEY_TAB_GO_TO_NUMBER
      if (event->keyval == GDK_1) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
        return TRUE;
      }
      if ((event->keyval == GDK_2) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 1)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
        return TRUE;
      }
      if ((event->keyval == GDK_3) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 2)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);
        return TRUE;
      }
      if ((event->keyval == GDK_4) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 3)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);
        return TRUE;
      }
      if ((event->keyval == GDK_5) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 4)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 4);
        return TRUE;
      }
      if ((event->keyval == GDK_6) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 5)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 5);
        return TRUE;
      }
      if ((event->keyval == GDK_7) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 6)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 6);
        return TRUE;
      }
      if ((event->keyval == GDK_8) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 7)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 7);
        return TRUE;
      }
      if ((event->keyval == GDK_9) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 8)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 8);
        return TRUE;
      }
      if ((event->keyval == GDK_0) && (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 9)) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 9);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TOGGLE_BACKGROUND
      if (HOTKEY_TOGGLE_BACKGROUND) {
        background_status++;
#if TAB
        int i = 0;
#endif
        if (background_status >= TOGGLE_BG_ORDER_SIZE)
          background_status = 0;
#if TOGGLE_BG_TRANSPARENT
        if (!strncmp(background_order[background_status], "Transparent", 12)) {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 1);
#if TAB
          }
#endif
          return TRUE;
        }
#endif
#if TOGGLE_BG_IMAGE
        if (!strncmp(background_order[background_status], "Image", 6)) {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
            vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_file(imgstr, NULL));
#if TAB
          }
#endif
          return TRUE;
        }
#endif
#if TOGGLE_BG_NO_BACKGROUND
        if (!strncmp(background_order[background_status], "No background", 14)) {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
            vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_xpm_data(NULL));
#if INIT_OPACITY || BACKGROUND_OPACITY
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), 65535);
#endif
#if TAB
          }
#endif
          return TRUE;
        }
#endif
#if TOGGLE_BG_OPACITY
        if (!strncmp(background_order[background_status], "Opacity", 8)) {
#if TAB
          for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
            term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
            vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
            vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_xpm_data(NULL));
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

#ifdef HOTKEY_SATURATION_MORE
      if (HOTKEY_SATURATION_MORE) {
        saturation_level += 0.1;
        if (saturation_level > 1)
          saturation_level = 1;
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
          vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
          if (!strncmp(background_order[background_status], "Opacity", 8))
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef HOTKEY_SATURATION_LESS
      if (HOTKEY_SATURATION_LESS) {
        saturation_level -= 0.1;
        if (saturation_level < 0)
          saturation_level = 0;
#if TAB
        int i = 0;
        for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
          term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
          vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
          vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
          if (!strncmp(background_order[background_status], "Opacity", 8))
            vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
        }
#endif
        return TRUE;
      }
#endif

#ifdef HOTKEY_RESET_TERMINAL
      if (HOTKEY_RESET_TERMINAL) {
#if TAB
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
        vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 0);
        return TRUE;
      }
#endif

#ifdef HOTKEY_RESET_AND_CLEAR
      if (HOTKEY_RESET_AND_CLEAR) {
#if TAB
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
        vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 1);
        return TRUE;
      }
#endif

#ifdef HOTKEY_COPY
      if (HOTKEY_COPY) {
#if TAB
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
        vte_terminal_copy_clipboard(VTE_TERMINAL(term->vte));
        return TRUE;
      }
#endif

#ifdef HOTKEY_PASTE
      if (HOTKEY_PASTE) {
#if TAB
        term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
        vte_terminal_paste_clipboard(VTE_TERMINAL(term->vte));
        return TRUE;
      }
#endif

#ifdef HOTKEY_FONT_BIGGER
      if ((HOTKEY_FONT_BIGGER) && (font_size < 99)) {
        font_size++;
        font_size_changed();
        return TRUE;
      }
#endif

#ifdef HOTKEY_FONT_SMALLER
      if ((HOTKEY_FONT_SMALLER) && (font_size > 1)) {
        font_size--;
        font_size_changed();
        return TRUE;
      }
#endif

#ifdef HOTKEY_FONT_DEFAULT_SIZE
      if (HOTKEY_FONT_DEFAULT_SIZE) {
        font_size = font_size_default;
        font_size_changed();
        return TRUE;
      }
#endif

#ifdef HOTKEY_FONT_SELECT
      if (HOTKEY_FONT_SELECT) {
        GtkWidget *font_dialog = gtk_font_selection_dialog_new(font_str);
        gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog), font_str);
        if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(font_dialog))) {
          g_snprintf(font_name, sizeof(font_name), "%s", gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog)));
          int len = strlen(font_name) - 1;
          int old_font_size = font_size;
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
          if (isdigit(font_name[len - 2]))
            font_size = old_font_size;
          while (len > 0 && isdigit(font_name[len]))
            font_name[len--] = 0;
          while (len > 0 && font_name[len] == ' ')
            font_name[len--] = 0;
          font_size_changed();
        }
        gtk_widget_destroy(font_dialog);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TAB_FIRST
      if (HOTKEY_TAB_FIRST) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TAB_LAST
      if (HOTKEY_TAB_LAST) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TAB_PREVIOUS
      if (HOTKEY_TAB_PREVIOUS) {
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index ? index - 1 : gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TAB_NEXT
      if (HOTKEY_TAB_NEXT) {
        int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), (index == (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1)) ? 0 : index + 1);
        return TRUE;
      }
#endif

#ifdef HOTKEY_TAB_ADD
      if (HOTKEY_TAB_ADD) {
        add_tab();
        return TRUE;
      }
#endif

#ifdef HOTKEY_TAB_REMOVE
      if (HOTKEY_TAB_REMOVE) {
        DEL_TAB(NULL, CLOSE_DIALOG);
        return TRUE;
      }
#endif

#if defined(HOTKEY_TOGGLE_HOTKEYS) || MENU_TOGGLE_HOTKEYS
    }
#endif

  }
  return FALSE;
}
#endif /* HOTKEY */

#if CLOSE_DIALOG
gboolean delete_event()
#endif
#if !CLOSE_DIALOG
void delete_event()
#endif
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
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), dialog_hbox);
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
#if CLOSE_DIALOG
      return TRUE;
#endif
#if !CLOSE_DIALOG
      return;
#endif
    }
  }
#endif
#if TAB
  int i = 0;
  for (i = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i > 0 ; i--) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), i - 1);
    DEL_TAB(NULL, 0);
  }
#endif
#if !TAB
  DEL_TAB(NULL, CLOSE_DIALOG);
#endif
#if CLOSE_DIALOG
  return FALSE;
#endif
}

#if MENU_TOGGLE_ON_TOP
void do_always_on_top()
{
  always_on_top ^= 1;
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

#if MENU_RESET_AND_CLEAR
void do_clear()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 1);
}
#endif

#if MENU_COPY
void do_copy()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_copy_clipboard(VTE_TERMINAL(term->vte));
}
#endif

#if MENU_TAB_EDIT_LABEL
void do_edit_label()
{
  char *label_name = "";
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(index)), "current_tab");
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
  gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry);
  gtk_widget_show_all(dialog);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
#if TAB_CLOSE_BUTTON
    term->label = gtk_hbox_new(0, 0);
    term->button = gtk_button_new();
    GtkWidget *img_button = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
    GtkRcStyle *rcstyle = gtk_rc_style_new();
    gtk_button_set_image(GTK_BUTTON(term->button), img_button);
    gtk_button_set_relief(GTK_BUTTON(term->button), GTK_RELIEF_NONE);
    rcstyle->xthickness = 0;
    rcstyle->ythickness = 0;
    gtk_widget_modify_style(term->button, rcstyle);
    g_object_unref(rcstyle);
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
void do_match_copy()
{
  gtk_clipboard_set_text(gtk_clipboard_get(GDK_SELECTION_CLIPBOARD), matched_url, -1);
  matched_url = NULL;
}
#endif

#ifdef MENU_MATCH_STRING_EXEC
void do_match_open()
{
  char new_window_str[256];
  g_snprintf(new_window_str, sizeof(new_window_str), "%s '%s' &", MENU_MATCH_STRING_EXEC, matched_url);
  system(new_window_str);
  matched_url = NULL;
}
#endif

#if MENU_CHANGE_SATURATION
void do_menu_saturation()
{
  double saturation_level_old = saturation_level;
  GtkWidget *saturation_dialog;
#if BUTTON_ORDER_BY_RCFILE
  if (button_order)
    saturation_dialog = gtk_dialog_new_with_buttons(LABEL_MENU_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
  else
#endif
    saturation_dialog = gtk_dialog_new_with_buttons(LABEL_MENU_SATURATION, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
  adjustment = gtk_hscale_new_with_range(0, 1, 0.001);
  gtk_range_set_value(GTK_RANGE(adjustment), saturation_level_old);
  g_signal_connect_after(adjustment, "change-value", do_change_saturation, NULL);
  gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(saturation_dialog))), adjustment);
  gtk_dialog_set_default_response(GTK_DIALOG(saturation_dialog), GTK_RESPONSE_OK);
  gtk_widget_show_all(saturation_dialog);
  saturation_level = (gtk_dialog_run(GTK_DIALOG(saturation_dialog)) == GTK_RESPONSE_OK) ? gtk_range_get_value(GTK_RANGE(adjustment)) : saturation_level_old;
  gtk_widget_destroy(saturation_dialog);
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
    vte_terminal_set_background_saturation(VTE_TERMINAL(term->vte), saturation_level);
#if BACKGROUND_OPACITY
    vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if DO_TOGGLE_BACKGROUND && TOGGLE_BG_OPACITY
    if (!strncmp(background_order[background_status], "Opacity", 8))
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#endif
#if TAB
  }
#endif
}
#endif /* MENU_CHANGE_SATURATION */

#if MENU_COLOR_BACKGROUND
void do_menu_tint_color()
{
  GtkColorSelectionDialog *color_tint_dialog = (GtkColorSelectionDialog*)gtk_color_selection_dialog_new(LABEL_DIALOG_BACKGROUND_TINT);
  gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(color_tint_dialog)), &color_tint);
  if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(color_tint_dialog))) {
    gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(gtk_color_selection_dialog_get_color_selection(color_tint_dialog)), &color_tint);
#if TAB
    int i = 0;
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_tint_color(VTE_TERMINAL(term->vte), &color_tint);
#if TAB
    }
#endif
  }
  gtk_widget_destroy(GTK_WIDGET(color_tint_dialog));
}
#endif

#if MENU_OPEN_NEW_WINDOW
void do_new_window()
{
  system(PROGRAM_NAME " &");
}
#endif

#if MENU_PASTE
void do_paste()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_paste_clipboard(VTE_TERMINAL(term->vte));
}
#endif

#if MENU_RESET_TERMINAL
void do_reset()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_reset(VTE_TERMINAL(term->vte), 1, 0);
}
#endif

#if MENU_SELECT_ALL
void do_select_all()
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_select_all(VTE_TERMINAL(term->vte));
}
#endif

#if MENU_FONT_SELECT
void do_select_font()
{
  GtkWidget *font_dialog = gtk_font_selection_dialog_new(font_str);
  gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog), font_str);
  if (GTK_RESPONSE_OK == gtk_dialog_run(GTK_DIALOG(font_dialog))) {
    g_snprintf(font_name, sizeof(font_name), "%s", gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_dialog)));
    int len = strlen(font_name) - 1;
    int old_font_size = font_size;
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
    if (isdigit(font_name[len - 2]))
      font_size = old_font_size;
    while (len > 0 && isdigit(font_name[len]))
      font_name[len--] = 0;
    while (len > 0 && font_name[len] == ' ')
      font_name[len--] = 0;
    font_size_changed();
  }
  gtk_widget_destroy(font_dialog);
}
#endif

#if MENU_TOGGLE_ANTI_ALIAS
void do_toggle_antialias()
{
  antialias_status++;
  if (antialias_status > VTE_ANTI_ALIAS_FORCE_DISABLE)
    antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#if TAB
  int i = 0;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
    vte_terminal_set_font_from_string_full(VTE_TERMINAL(term->vte), font_str, antialias_status);
#if TAB
  }
#endif
}
#endif

#if MENU_TOGGLE_BACKGROUND
void do_toggle_bg()
{
  background_status++;
  if (background_status >= TOGGLE_BG_ORDER_SIZE)
    background_status = 0;
#if TAB
  int i = 0;
#endif
#if TOGGLE_BG_TRANSPARENT
  if (!strncmp(background_order[background_status], "Transparent", 12)) {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 1);
#if TAB
    }
#endif
  }
#endif
#if TOGGLE_BG_IMAGE
  if (!strncmp(background_order[background_status], "Image", 6)) {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
      vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_file(imgstr, NULL));
#if TAB
    }
#endif
  }
#endif
#if TOGGLE_BG_NO_BACKGROUND
  if (!strncmp(background_order[background_status], "No background", 14)) {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
      vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_xpm_data(NULL));
#if INIT_OPACITY || BACKGROUND_OPACITY
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), 65535);
#endif
#if TAB
    }
#endif
  }
#endif
#if TOGGLE_BG_OPACITY
  if (!strncmp(background_order[background_status], "Opacity", 8)) {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      vte_terminal_set_background_transparent(VTE_TERMINAL(term->vte), 0);
      vte_terminal_set_background_image(VTE_TERMINAL(term->vte), gdk_pixbuf_new_from_xpm_data(NULL));
      vte_terminal_set_opacity(VTE_TERMINAL(term->vte), (1 - saturation_level) * 65535);
#if TAB
    }
#endif
  }
#endif
}
#endif

#if MENU_TOGGLE_DECORATED
void do_toggle_decorated()
{
  gtk_window_set_decorated(GTK_WINDOW(main_window), window_decorated_status);
  window_decorated_status ^= 1;
#if TAB
  int index = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(index)), "current_tab");
#endif
  gtk_widget_hide_all(main_window);
  gtk_widget_show_all(main_window);
#if DO_TOGGLE_SCROLLBAR || DO_TOGGLE_STATUS_BAR
  recover_window_status();
#endif
#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif
  gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);
}
#endif

#if MENU_TOGGLE_FULLSCREEN
void do_toggle_fullscreen()
{
  window_fullscreen_status ^= 1;
  window_fullscreen_status ? gtk_window_maximize(GTK_WINDOW(main_window)) : gtk_window_unmaximize(GTK_WINDOW(main_window));
}
#endif

#if MENU_TOGGLE_HOTKEYS
void do_toggle_hotkeys()
{
  hotkey_status ^= 1;
}
#endif

#if MENU_TOGGLE_SCROLLBAR && defined(SCROLLBAR)
void do_toggle_scrollbar()
{
#if TAB
  int i = 0;
#endif
  scrollbar_status ^= 1;
  if (scrollbar_status) {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      gtk_widget_hide(term->scrollbar);
#if TAB
    }
#endif
  } else {
#if TAB
    for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
      term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(i)), "current_tab");
#endif
      gtk_widget_show(term->scrollbar);
#if TAB
    }
#endif
  }
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_TOGGLE_STATUS_BAR && STATUS_BAR
void do_toggle_status_bar()
{
  status_bar_status ^= 1;
  status_bar_status ? gtk_widget_hide(statusbar) : gtk_widget_show(statusbar);
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_TOGGLE_TABBAR
void do_toggle_tabbar()
{
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), tabbar_status);
  tabbar_status ^= 1;
#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1)
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), 0);
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_FONT_DEFAULT_SIZE
void do_zoom_100()
{
  font_size = font_size_default;
  font_size_changed();
}
#endif

#if MENU_FONT_BIGGER
void do_zoom_in()
{
  if (font_size < 99) {
    font_size++;
    font_size_changed();
  }
}
#endif

#if MENU_FONT_SMALLER
void do_zoom_out()
{
  if (font_size > 1) {
    font_size--;
    font_size_changed();
  }
}
#endif

void set_encoding(GtkWidget *widget, void *data)
{
#if TAB
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
#endif
  vte_terminal_set_encoding(VTE_TERMINAL(term->vte), (char*)data);
#if STATUS_BAR
  change_statusbar_encoding();
#endif
}

void switch_page()
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
    g_snprintf(tabtitle, sizeof(tabtitle), "%s - tab %d of %d", VTE_PROGRAM_NAME, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) + 1, index);
    gtk_window_set_title(GTK_WINDOW(main_window), tabtitle);
  }
#endif
#if WINDOW_TITLE_DYNAMIC
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)))), "current_tab");
  gtk_window_set_title(GTK_WINDOW(main_window), vte_terminal_get_window_title(VTE_TERMINAL(term->vte)));
#endif
}

int main(int argc, char **argv)
{
#if COMMAND_DOCK_MODE
int at_dock_mode = 0;
#endif

#ifdef TAB_LABEL
  textdomain("gtk20");
#endif

#if COMMAND_AT_ROOT_WINDOW || COMMAND_DOCK_MODE || COMMAND_EXEC_PROGRAM || COMMAND_LOGIN_SHELL || COMMAND_SET_TITLE || COMMAND_SHOW_HELP || COMMAND_SHOW_OPTIONS || COMMAND_SHOW_VERSION || COMMAND_TAB_NUMBERS || defined(MENU_ENCODING_LIST)
  int i;
#endif
#if COMMAND_AT_ROOT_WINDOW || COMMAND_DOCK_MODE || COMMAND_FULLSCREEN || COMMAND_LOGIN_SHELL || COMMAND_SET_TITLE || COMMAND_SHOW_HELP || COMMAND_SHOW_OPTIONS || COMMAND_SHOW_VERSION || COMMAND_TAB_NUMBERS || defined(MENU_CUSTOM)
  int j;
#endif

#if COMMAND_EXEC_PROGRAM
  g_snprintf(default_command, sizeof(default_command), "%s", DEFAULT_COMMAND);
  if (argc > 2) {
    for (i = 0 ; i < argc ; i++) {
      if (!strncmp(argv[i], "-e", 3)) {
        if (i < argc - 1) {
          g_snprintf(default_command, sizeof(default_command), "%s", argv[i + 1]);
#if COMMAND_LOGIN_SHELL
          login_shell_flag = 2;
#endif
        }
        if (++i != argc)
          default_argv = &(argv[i]);
      }
    }
  }
#endif

#if COMMAND_AT_ROOT_WINDOW || COMMAND_DOCK_MODE || COMMAND_LOGIN_SHELL || COMMAND_SET_TITLE || COMMAND_SHOW_HELP || COMMAND_SHOW_OPTIONS || COMMAND_SHOW_VERSION || COMMAND_TAB_NUMBERS
  i = 1;
  j = 1;
  while ((j < argc) && strncmp(argv[j], "-e", 3)) {
#if COMMAND_SET_TITLE
    if (argc > (j + 1) && (!strncmp(argv[j], "-title", 7) || !strncmp(argv[j], "-T", 3)))
      program_name = argv[j + 1];
#endif

#if COMMAND_SHOW_VERSION
    if (!strncmp(argv[j], "-v", 3)) {
      printf("%s, version %s\n", PROGRAM_NAME, PROGRAM_VERSION);
      return 0;
    }
#endif

#if COMMAND_SHOW_OPTIONS
    if (!strncmp(argv[j], "-o", 3)) {
      system("showvte");
      return 0;
    }
#endif

#if COMMAND_SHOW_HELP
    if (!strncmp(argv[j], "-h", 3)) {
      printf("%s, version %s\n\nUsage:\n\t%s [option%s]\n\nOption%s:\n", PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_NAME,
#if COMMAND_AT_ROOT_WINDOW || COMMAND_DOCK_MODE || COMMAND_EXEC_PROGRAM || COMMAND_FULLSCREEN || COMMAND_LOGIN_SHELL || COMMAND_SET_TITLE || COMMAND_SHOW_OPTIONS || COMMAND_SHOW_VERSION || COMMAND_TAB_NUMBERS
             "s", "s");
#endif
#if !COMMAND_AT_ROOT_WINDOW && !COMMAND_DOCK_MODE && !COMMAND_EXEC_PROGRAM && !COMMAND_FULLSCREEN && !COMMAND_LOGIN_SHELL && !COMMAND_SET_TITLE && !COMMAND_SHOW_OPTIONS && !COMMAND_SHOW_VERSION && !COMMAND_TAB_NUMBERS
             "", "");
#endif
#if COMMAND_DOCK_MODE
      printf("\t-d                    \tstart %s as a dock\n", PROGRAM_NAME);
#endif
#if COMMAND_EXEC_PROGRAM
      printf("\t-e [program] [options]\tspecify the program to be run in %s\n", PROGRAM_NAME);
#endif
#if COMMAND_FULLSCREEN
      printf("\t-f                    \tstart %s in fullscreen mode\n", PROGRAM_NAME);
#endif
      printf("\t-h                    \tshow this help\n");
#if COMMAND_LOGIN_SHELL
      printf("\t-ls                   \tuse login shell\n");
#endif
#if COMMAND_SHOW_OPTIONS
      printf("\t-o                    \tshow build-time configuration\n");
#endif
#if COMMAND_AT_ROOT_WINDOW
      printf("\t-r                    \tmake %s run in root window\n", PROGRAM_NAME);
#endif
#if COMMAND_SET_TITLE
      printf("\t-T [string]           \tset program title\n");
      printf("\t-title [string]       \tset program title\n");
#endif
#if COMMAND_SHOW_VERSION
      printf("\t-v                    \tshow version\n");
#endif
#if COMMAND_TAB_NUMBERS
      printf("\t-2 to -9              \tspecify the initial tab numbers\n");
#endif
#ifdef BACKGROUND_IMAGE
      printf("\nBackground image:\n\t$HOME/%s\n", BACKGROUND_IMAGE);
#endif
      printf("\n");
      return 0;
    }
#endif

#if COMMAND_LOGIN_SHELL
    if (!strncmp(argv[j], "-ls", 4) && !(login_shell_flag & 4))
      login_shell_flag ^= 4;
    if (login_shell_flag == 4)
      login_shell_flag = 5;
#endif

#if COMMAND_AT_ROOT_WINDOW
    if (!strncmp(argv[j], "-r", 3))
      at_root_window = 1;
#endif

#if COMMAND_DOCK_MODE
    if (!strncmp(argv[j], "-d", 3))
      at_dock_mode = 1;
#endif

#if COMMAND_TAB_NUMBERS
    if (!strncmp(argv[j], "-2", 3))
      i = 2;
    if (!strncmp(argv[j], "-3", 3))
      i = 3;
    if (!strncmp(argv[j], "-4", 3))
      i = 4;
    if (!strncmp(argv[j], "-5", 3))
      i = 5;
    if (!strncmp(argv[j], "-6", 3))
      i = 6;
    if (!strncmp(argv[j], "-7", 3))
      i = 7;
    if (!strncmp(argv[j], "-8", 3))
      i = 8;
    if (!strncmp(argv[j], "-9", 3))
      i = 9;
#endif

    j++;
  }
#endif /* COMMAND_AT_ROOT_WINDOW || COMMAND_DOCK_MODE || COMMAND_LOGIN_SHELL || COMMAND_SET_TITLE || COMMAND_SHOW_HELP || COMMAND_SHOW_OPTIONS || COMMAND_SHOW_VERSION || COMMAND_TAB_NUMBERS */

#ifdef BACKGROUND_IMAGE
  g_snprintf(imgstr, sizeof(imgstr), "%s/%s", g_getenv("HOME"), BACKGROUND_IMAGE);
#endif

#ifdef PROGRAM_ICON
  g_snprintf(iconstr, sizeof(iconstr), "%s/%s", g_getenv("HOME"), PROGRAM_ICON);
#endif

#ifdef FONT
  g_snprintf(font_name, sizeof(font_name), "%s", FONT);
  int len = strlen(font_name) - 1;
  font_size = 0;
  int k = 0;
  if (isdigit(font_name[len - 1])) {
    for (k = 0 ; k < 10 ; k++) {
      if (font_name[len - 1] == *number_char[k])
        font_size += k * 10;
      if (font_name[len] == *number_char[k])
        font_size += k;
    }
  } else {
    for (k = 0 ; k < 10 ; k++) {
      if (font_name[len] == *number_char[k])
        font_size += k;
    }
  }
  if (isdigit(font_name[len - 2]))
    font_size = 10;
  else if (font_size < 1)
    font_size = 10;
  while (len > 0 && isdigit(font_name[len]))
    font_name[len--] = 0;
  while (len > 0 && font_name[len] == ' ')
    font_name[len--] = 0;
  g_snprintf(font_str, sizeof(font_str), "%s %d", font_name, font_size);
  font_size_default = font_size;
#endif

  gtk_init(&argc, &argv);

#if BUTTON_ORDER_BY_RCFILE
  g_object_get(gtk_settings_get_default(), "gtk-alternative-button-order", &button_order, NULL);
#endif

#if defined(BACKGROUND_TINT_COLOR) && BACKGROUND_EXIST
  gdk_color_parse(BACKGROUND_TINT_COLOR, &color_tint);
#endif

  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#if PROGRAM_WM_CLASS
  gtk_window_set_wmclass(GTK_WINDOW(main_window), VTE_PROGRAM_NAME, PROGRAM_NAME);
#endif

#ifdef SHOW_WINDOW_DECORATED
  gtk_window_set_decorated(GTK_WINDOW(main_window), SHOW_WINDOW_DECORATED);
#endif

#if COMMAND_SET_TITLE
  gtk_window_set_title(GTK_WINDOW(main_window), VTE_PROGRAM_NAME);
#endif

#ifdef PROGRAM_ICON
  if (!gtk_window_set_icon_from_file(GTK_WINDOW(main_window), iconstr, NULL))
    gtk_window_set_icon_from_file(GTK_WINDOW(main_window), ICON_DIR"/evilvte.png", NULL);
#endif

#if !defined(PROGRAM_ICON) && SHOW_WINDOW_ICON
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

#if INIT_OPACITY || BACKGROUND_OPACITY
  gtk_widget_set_colormap(main_window, gdk_screen_get_rgba_colormap(gtk_widget_get_screen(main_window)));
#endif

  add_tab();

#if COMMAND_EXEC_PROGRAM && TAB
  g_snprintf(default_command, sizeof(default_command), "%s", DEFAULT_COMMAND);
  default_argv = NULL;
#if COMMAND_LOGIN_SHELL
  if (login_shell_flag & 4)
    login_shell_flag = 5;
#endif
#endif

#if COMMAND_TAB_NUMBERS
  while (i > 1) {
    add_tab();
    i--;
  }
  term = (struct terminal*)g_object_get_data(G_OBJECT(CURRENT_TAB(0)), "current_tab");
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
  gtk_window_set_focus(GTK_WINDOW(main_window), term->vte);
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

  g_signal_connect(main_window, "delete_event", G_CALLBACK(delete_event), NULL);

#if HOTKEY
  g_signal_connect(main_window, "key-press-event", G_CALLBACK(key_press_event), NULL);
#endif

#if TAB
  g_signal_connect_after(notebook, "switch-page", switch_page, NULL);
#endif

#if TAB_REORDERABLE
  g_signal_connect(notebook, "page-reordered", switch_page, NULL);
#endif

#if COMMAND_AT_ROOT_WINDOW
  if (at_root_window) {
    gtk_window_set_keep_below(GTK_WINDOW(main_window), 1);
    gtk_window_set_decorated(GTK_WINDOW(main_window), 0);
#ifdef HOTKEY_TOGGLE_DECORATED
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

#if COMMAND_DOCK_MODE
  if (at_dock_mode)
    gtk_window_set_type_hint(GTK_WINDOW(main_window), GDK_WINDOW_TYPE_HINT_DOCK);
#endif

  gtk_widget_show_all(main_window);

#if COMMAND_FULLSCREEN
  j = 1;
  while ((j < argc) && strncmp(argv[j], "-e", 3)) {
    if (!strncmp(argv[j], "-f", 3)) {
      gtk_window_maximize(GTK_WINDOW(main_window));
#ifdef HOTKEY_TOGGLE_FULLSCREEN
      window_fullscreen_status = 1;
#endif
#if MENU_TOGGLE_FULLSCREEN
      window_fullscreen_status = 1;
#endif
    }
    j++;
  }
#endif

#if MENU
  menu = gtk_menu_new();
#ifdef MENU_MATCH_STRING_EXEC
  match_open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), match_open);
  g_signal_connect(match_open, "activate", do_match_open, NULL);
  match_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), match_copy);
  g_signal_connect(match_copy, "activate", do_match_copy, NULL);
  match_item = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), match_item);
#ifdef MENU_CUSTOM
  menu_item_success += 100;
#endif
#endif
#endif

#ifdef MENU_CUSTOM
#if MENU_SEPARATOR
  GtkWidget *menu_item;
#endif
  for (j = 0 ; j < MENU_CUSTOM_SIZE ; j++)
#endif
  {
#if MENU_COPY
    if (!strncmp(menu_custom[j], "Copy", 5)) {
      GtkWidget *menu_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_copy);
      g_signal_connect(menu_copy, "activate", do_copy, NULL);
      menu_item_success++;
    }
#endif

#if MENU_PASTE
    if (!strncmp(menu_custom[j], "Paste", 6)) {
      GtkWidget *menu_paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_paste);
      g_signal_connect(menu_paste, "activate", do_paste, NULL);
      menu_item_success++;
    }
#endif

#if MENU_SELECT_ALL
    if (!strncmp(menu_custom[j], "Select all", 11)) {
      GtkWidget *menu_select_all = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_ALL, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_select_all);
      g_signal_connect(menu_select_all, "activate", do_select_all, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TAB_ADD
    if (!strncmp(menu_custom[j], "Add tab", 8)) {
      GtkWidget *menu_add_tab = gtk_image_menu_item_new_from_stock(GTK_STOCK_ADD, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_add_tab);
      g_signal_connect(menu_add_tab, "activate", add_tab, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TAB_REMOVE
    if (!strncmp(menu_custom[j], "Remove tab", 11)) {
      GtkWidget *menu_close_tab = gtk_image_menu_item_new_from_stock(GTK_STOCK_REMOVE, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_close_tab);
      g_signal_connect(menu_close_tab, "activate", G_CALLBACK(del_tab), (int*)CLOSE_DIALOG);
      menu_item_success++;
    }
#endif

#if MENU_TAB_EDIT_LABEL
    if (!strncmp(menu_custom[j], "Edit label", 11)) {
      GtkWidget *menu_edit_label = gtk_image_menu_item_new_from_stock(GTK_STOCK_EDIT, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_edit_label);
      g_signal_connect(menu_edit_label, "activate", do_edit_label, NULL);
      menu_item_success++;
    }
#endif

#if MENU_OPEN_NEW_WINDOW
    if (!strncmp(menu_custom[j], "New window", 11)) {
      GtkWidget *menu_new_window = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_new_window);
      g_signal_connect(menu_new_window, "activate", do_new_window, NULL);
      menu_item_success++;
    }
#endif

#if MENU_QUIT
    if (!strncmp(menu_custom[j], "Quit", 5)) {
      GtkWidget *menu_quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_quit);
#if CLOSE_SAFELY || CLOSE_DIALOG
      g_signal_connect(menu_quit, "activate", G_CALLBACK(delete_event), NULL);
#endif
#if !CLOSE_SAFELY && !CLOSE_DIALOG
      g_signal_connect(menu_quit, "activate", gtk_main_quit, NULL);
#endif
      menu_item_success++;
    }
#endif

#if MENU_FONT_BIGGER
    if (!strncmp(menu_custom[j], "Zoom in", 8)) {
      GtkWidget *menu_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_zoom_in);
      g_signal_connect(menu_zoom_in, "activate", do_zoom_in, NULL);
      menu_item_success++;
    }
#endif

#if MENU_FONT_SMALLER
    if (!strncmp(menu_custom[j], "Zoom out", 9)) {
      GtkWidget *menu_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_zoom_out);
      g_signal_connect(menu_zoom_out, "activate", do_zoom_out, NULL);
      menu_item_success++;
    }
#endif

#if MENU_FONT_DEFAULT_SIZE
    if (!strncmp(menu_custom[j], "Zoom default", 13)) {
      GtkWidget *menu_zoom_100 = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_zoom_100);
      g_signal_connect(menu_zoom_100, "activate", do_zoom_100, NULL);
      menu_item_success++;
    }
#endif

#if MENU_RESET_TERMINAL
    if (!strncmp(menu_custom[j], "Reset terminal", 15)) {
      GtkWidget *menu_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_REFRESH, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_reset);
      g_signal_connect(menu_reset, "activate", do_reset, NULL);
      menu_item_success++;
    }
#endif

#if MENU_RESET_AND_CLEAR
    if (!strncmp(menu_custom[j], "Reset and clear", 16)) {
      GtkWidget *menu_clear = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLEAR, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_clear);
      g_signal_connect(menu_clear, "activate", do_clear, NULL);
      menu_item_success++;
    }
#endif

#if MENU_FONT_SELECT
    if (!strncmp(menu_custom[j], "Select font", 12)) {
      GtkWidget *menu_font = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_FONT, NULL);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_font);
      g_signal_connect(menu_font, "activate", do_select_font, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_ON_TOP
    if (!strncmp(menu_custom[j], "Toggle always on top", 21)) {
      GtkWidget *menu_toggle_on_top = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_ON_TOP);
      GtkWidget *image_toggle_on_top = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_on_top), image_toggle_on_top);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_on_top);
      g_signal_connect(menu_toggle_on_top, "activate", do_always_on_top, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_BACKGROUND
    if (!strncmp(menu_custom[j], "Toggle background", 18)) {
      GtkWidget *menu_toggle_bg = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_BG);
      GtkWidget *image_toggle_bg = gtk_image_new_from_stock(GTK_STOCK_DND_MULTIPLE, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_bg), image_toggle_bg);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_bg);
      g_signal_connect(menu_toggle_bg, "activate", do_toggle_bg, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_HOTKEYS
    if (!strncmp(menu_custom[j], "Toggle hotkeys locking", 23)) {
      GtkWidget *menu_toggle_hotkeys = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_HOTKEYS);
      GtkWidget *image_toggle_hotkeys = gtk_image_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_hotkeys), image_toggle_hotkeys);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_hotkeys);
      g_signal_connect(menu_toggle_hotkeys, "activate", do_toggle_hotkeys, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_SCROLLBAR && defined(SCROLLBAR)
    if (!strncmp(menu_custom[j], "Toggle scrollbar", 17)) {
      GtkWidget *menu_toggle_scrollbar = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_SCROLLBAR);
      GtkWidget *image_toggle_scrollbar = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_scrollbar), image_toggle_scrollbar);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_scrollbar);
      g_signal_connect(menu_toggle_scrollbar, "activate", do_toggle_scrollbar, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_STATUS_BAR && STATUS_BAR
    if (!strncmp(menu_custom[j], "Toggle status bar", 18)) {
      GtkWidget *menu_toggle_status_bar = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_STATUS_BAR);
      GtkWidget *image_toggle_status_bar = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_status_bar), image_toggle_status_bar);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_status_bar);
      g_signal_connect(menu_toggle_status_bar, "activate", do_toggle_status_bar, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_TABBAR
    if (!strncmp(menu_custom[j], "Toggle tabbar", 14)) {
      GtkWidget *menu_toggle_tabbar = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_TABBAR);
      GtkWidget *image_toggle_tabbar = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_tabbar), image_toggle_tabbar);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_tabbar);
      g_signal_connect(menu_toggle_tabbar, "activate", do_toggle_tabbar, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_DECORATED
    if (!strncmp(menu_custom[j], "Toggle window decorated", 24)) {
      GtkWidget *menu_toggle_decorated = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_DECORATED);
      GtkWidget *image_toggle_decorated = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_decorated), image_toggle_decorated);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_decorated);
      g_signal_connect(menu_toggle_decorated, "activate", do_toggle_decorated, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_FULLSCREEN
    if (!strncmp(menu_custom[j], "Toggle fullscreen", 18)) {
      GtkWidget *menu_toggle_fullscreen = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_FULLSCREEN);
      GtkWidget *image_toggle_fullscreen = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_fullscreen), image_toggle_fullscreen);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_fullscreen);
      g_signal_connect(menu_toggle_fullscreen, "activate", do_toggle_fullscreen, NULL);
      menu_item_success++;
    }
#endif

#if MENU_TOGGLE_ANTI_ALIAS
    if (!strncmp(menu_custom[j], "Toggle anti-alias", 18)) {
      GtkWidget *menu_toggle_antialias = gtk_image_menu_item_new_with_label(LABEL_MENU_TOGGLE_ANTI_ALIAS);
      GtkWidget *image_toggle_antialias = gtk_image_new_from_stock(GTK_STOCK_LEAVE_FULLSCREEN, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_toggle_antialias), image_toggle_antialias);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_toggle_antialias);
      g_signal_connect(menu_toggle_antialias, "activate", do_toggle_antialias, NULL);
      menu_item_success++;
    }
#endif

#if MENU_CHANGE_SATURATION
    if (!strncmp(menu_custom[j], "Adjust saturation", 18)) {
      GtkWidget *menu_change_saturation = gtk_image_menu_item_new_with_label(LABEL_MENU_SATURATION);
      GtkWidget *image_change_saturation = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_change_saturation), image_change_saturation);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_change_saturation);
      g_signal_connect(menu_change_saturation, "activate", do_menu_saturation, NULL);
      menu_item_success++;
    }
#endif

#if MENU_COLOR_BACKGROUND
    if (!strncmp(menu_custom[j], "Background tint", 16)) {
      GtkWidget *menu_change_tint = gtk_image_menu_item_new_with_label(LABEL_DIALOG_BACKGROUND_TINT);
      GtkWidget *image_change_tint = gtk_image_new_from_stock(GTK_STOCK_SELECT_COLOR, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(menu_change_tint), image_change_tint);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_change_tint);
      g_signal_connect(menu_change_tint, "activate", do_menu_tint_color, NULL);
      menu_item_success++;
    }
#endif

#if MENU_SEPARATOR
    if (!strncmp(menu_custom[j], "Separator", 10)) {
      menu_item = gtk_separator_menu_item_new();
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);
    }
#endif

#ifdef MENU_CUSTOM
    if (!strncmp(menu_custom[j], "Encoding list", 14))
#endif
#ifdef MENU_ENCODING_LIST
    {
      GtkWidget *encoding_item[MENU_ENCODING_LIST_SIZE];
#ifdef MENU_CUSTOM
      GtkWidget *image_menu_enc;
#endif
      for (i = 0 ; i < MENU_ENCODING_LIST_SIZE ; i++) {
#if MENU_DEFAULT_ENCODING
        if (!strncmp(encoding[i], "Default Encoding", 17)) {
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
        gtk_menu_shell_append(GTK_MENU_SHELL(menu), encoding_item[i]);
        g_signal_connect(encoding_item[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
#ifdef MENU_CUSTOM
        menu_item_success++;
#endif
      }
    }
#endif /* MENU_ENCODING_LIST */
#if !defined(MENU_ENCODING_LIST) && MENU
    {
#ifdef MENU_CUSTOM
      encoding_item = gtk_image_menu_item_new_with_label("UTF-8");
      GtkWidget *image_menu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(encoding_item), image_menu_enc);
#endif
#ifndef MENU_CUSTOM
      encoding_item = gtk_menu_item_new_with_label("UTF-8");
#endif
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), encoding_item);
      g_signal_connect(encoding_item, "activate", G_CALLBACK(set_encoding), "UTF-8");
#ifdef MENU_CUSTOM
      menu_item_success++;
#endif
    }
#endif /* !defined(MENU_ENCODING_LIST) && MENU */

#ifdef MENU_CUSTOM
    if (!strncmp(menu_custom[j], "Input method", 13))
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
    if (!strncmp(menu_custom[j], "Submenu encoding list", 22))
#ifdef MENU_ENCODING_LIST
    {
      GtkWidget *subitem_enc = gtk_image_menu_item_new_with_label(LABEL_SUBMENU_ENCODING);
      GtkWidget *submenu_enc = gtk_menu_new();
      GtkWidget *image_submenu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), subitem_enc);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(subitem_enc), image_submenu_enc);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(subitem_enc), submenu_enc);
      GtkWidget *encoding_sub[MENU_ENCODING_LIST_SIZE];
      for (i = 0 ; i < MENU_ENCODING_LIST_SIZE ; i++) {
#if MENU_DEFAULT_ENCODING
        if (!strncmp(encoding[i], "Default Encoding", 17)) {
          encoding[i] = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term->vte));
          encoding_sub[i] = gtk_menu_item_new_with_label(LABEL_DEFAULT_ENCODING);
        } else
#endif
          encoding_sub[i] = gtk_menu_item_new_with_label(encoding[i]);
        gtk_menu_shell_append(GTK_MENU_SHELL(submenu_enc), encoding_sub[i]);
        g_signal_connect(encoding_sub[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
        menu_item_success++;
      }
    }
#endif
#if !defined(MENU_ENCODING_LIST) && MENU
    {
      GtkWidget *subitem_enc = gtk_image_menu_item_new_with_label(LABEL_SUBMENU_ENCODING);
      GtkWidget *submenu_enc = gtk_menu_new();
      GtkWidget *image_submenu_enc = gtk_image_new_from_stock(GTK_STOCK_CONVERT, GTK_ICON_SIZE_MENU);
      gtk_menu_shell_append(GTK_MENU_SHELL(menu), subitem_enc);
      gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(subitem_enc), image_submenu_enc);
      gtk_menu_item_set_submenu(GTK_MENU_ITEM(subitem_enc), submenu_enc);
      encoding_item = gtk_menu_item_new_with_label("UTF-8");
      gtk_menu_shell_append(GTK_MENU_SHELL(submenu_enc), encoding_item);
      g_signal_connect(encoding_item, "activate", G_CALLBACK(set_encoding), "UTF-8");
      menu_item_success++;
    }
#endif
#endif /* SUBMENU_ENCODING_LIST */

#if SUBMENU_INPUT_METHOD
    if (!strncmp(menu_custom[j], "Submenu input method", 21)) {
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
