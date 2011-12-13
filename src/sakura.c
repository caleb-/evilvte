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
#include <vte/vte.h>
#include "config.h"

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
#define DEFAULT_DIRECTORY g_getenv("PWD")
#endif

#ifndef ENABLE_LASTLOG
#define ENABLE_LASTLOG TRUE
#endif

#ifndef ENABLE_UTMP
#define ENABLE_UTMP TRUE
#endif

#ifndef ENABLE_WTMP
#define ENABLE_WTMP TRUE
#endif

#if SCROLLBAR_LEFT
#undef SCROLLBAR_RIGHT
#endif

#if CLOSE_SAFELY
#define GET_VTE_CHILD_PID TRUE
#endif

#if TAB_NEW_TAB_OLD_PATH
#undef GET_VTE_CHILD_PID /* prevent duplicated definitions */
#define GET_VTE_CHILD_PID TRUE
#define VTE_DEFAULT_DIRECTORY default_directory
#else
#define VTE_DEFAULT_DIRECTORY DEFAULT_DIRECTORY
#endif

#if BUTTON_ORDER_GTK_STYLE
#undef BUTTON_ORDER_QT_STYLE
#define BUTTON_ORDER FALSE
#endif

#if BUTTON_ORDER_QT_STYLE
#define BUTTON_ORDER TRUE
#endif

#ifdef CTRL_TOGGLE_BACKGROUND
#ifndef BACKGROUND_IMAGE
#define BACKGROUND_IMAGE ".config/evilvte/background.png"
#endif
#ifndef TOGGLE_BG_ORDER
#define TOGGLE_BG_ORDER "Image","Transparent","No background"
#endif
static char *background_order[] = {
TOGGLE_BG_ORDER
};
int background_order_size = sizeof(background_order) / sizeof(background_order[0]);
int background_status = 0;
#endif /* CTRL_TOGGLE_BACKGROUND */

#ifdef BACKGROUND_IMAGE
#define BACKGROUND_EXIST TRUE
#endif

#ifdef BACKGROUND_TRANSPARENT
#undef BACKGROUND_EXIST /* prevent duplicated definitions */
#define BACKGROUND_EXIST TRUE
#endif

#if !MENU
#undef MENU_COPY
#undef MENU_PASTE
#undef MENU_TAB_ADD_TAB
#undef MENU_TAB_REMOVE_TAB
#undef MENU_TAB_EDIT_LABEL
#undef MENU_NEW_WINDOW
#undef MENU_FONT_ZOOM_IN
#undef MENU_FONT_ZOOM_OUT
#undef MENU_FONT_ZOOM_100
#undef MENU_RESET
#undef MENU_RESET_AND_CLEAR
#undef MENU_QUIT
#undef MENU_INPUT_METHOD
#undef MENU_CUSTOM
#undef MENU_ENCODING_LIST
#endif

#if !TAB
#undef CTRL_TAB_OPEN_NEW_TAB
#undef CTRL_TAB_CLOSE_TAB
#undef CTRL_TAB_PREVIOUS_TAB
#undef CTRL_TAB_NEXT_TAB
#undef CTRL_TAB_FIRST_TAB
#undef CTRL_TAB_LAST_TAB
#undef CTRL_TAB_JUMP_NUMBER
#undef CTRL_TAB_EDIT_LABEL
#undef DOUBLE_PRESS_HOTKEY
#undef MENU_TAB_ADD_TAB
#undef MENU_TAB_REMOVE_TAB
#undef SHOW_WINDOW_BORDER
#undef TAB_AT_BOTTOM
#undef TAB_AT_LEFT
#undef TAB_AT_RIGHT
#undef TAB_AT_TOP
#undef TAB_BORDER
#undef TAB_BORDER_HORIZONTAL
#undef TAB_BORDER_VERTICAL
#undef TAB_EXPAND
#undef TAB_INFO_AT_TITLE
#undef TAB_INITIAL_NUMBER
#undef TAB_LABEL
#undef TAB_LABEL_STYLE_POEM
#undef TAB_LABEL_STYLE_CUSTOM
#undef TAB_NEW_TAB_OLD_PATH
#undef TABBAR
#undef TABBAR_AUTOHIDE
#undef TABBAR_MENU_SELECT_TAB
#undef TABBAR_MOUSE_SCROLLABLE
#define SHOW_WINDOW_BORDER FALSE
#endif

#ifdef TABBAR
#if !TABBAR
#undef SHOW_WINDOW_BORDER
#undef TAB_AT_BOTTOM
#undef TAB_AT_LEFT
#undef TAB_AT_RIGHT
#undef TAB_AT_TOP
#undef TAB_BORDER
#undef TAB_BORDER_HORIZONTAL
#undef TAB_BORDER_VERTICAL
#undef TAB_EXPAND
#undef TABBAR_AUTOHIDE
#undef TABBAR_MENU_SELECT_TAB
#undef TABBAR_MOUSE_SCROLLABLE
#define SHOW_WINDOW_BORDER FALSE
#endif
#endif /* TABBAR */

#if TABBAR_AUTOHIDE
#ifndef TABBAR
#define TABBAR TRUE
#endif
#endif

#if !CTRL
#undef CTRL_COPY_TO_CLIPBOARD
#undef CTRL_PASTE_FROM_CLIPBOARD
#undef CTRL_EDIT_ENCODING
#undef CTRL_FONT_BIGGER
#undef CTRL_FONT_SMALLER
#undef CTRL_FONT_DEFAULT_SIZE
#undef CTRL_OPEN_NEW_WINDOW
#undef CTRL_RESET_TERMINAL
#undef CTRL_RESET_AND_CLEAR_TERM
#undef CTRL_SATURATION_MORE
#undef CTRL_SATURATION_LESS
#undef CTRL_TAB_OPEN_NEW_TAB
#undef CTRL_TAB_CLOSE_TAB
#undef CTRL_TAB_PREVIOUS_TAB
#undef CTRL_TAB_NEXT_TAB
#undef CTRL_TAB_FIRST_TAB
#undef CTRL_TAB_LAST_TAB
#undef CTRL_TAB_JUMP_NUMBER
#undef CTRL_TAB_EDIT_LABEL
#undef CTRL_TOGGLE_BACKGROUND
#undef DOUBLE_PRESS_HOTKEY
#endif

#ifdef MENU_CUSTOM
#undef MENU_SEPARATOR_1
#undef MENU_SEPARATOR_2
#undef MENU_COPY
#undef MENU_PASTE
#undef MENU_TAB_ADD_TAB
#undef MENU_TAB_REMOVE_TAB
#undef MENU_TAB_EDIT_LABEL
#undef MENU_NEW_WINDOW
#undef MENU_FONT_ZOOM_IN
#undef MENU_FONT_ZOOM_OUT
#undef MENU_FONT_ZOOM_100
#undef MENU_RESET
#undef MENU_RESET_AND_CLEAR
#undef MENU_QUIT
#undef MENU_INPUT_METHOD
#define MENU_COPY TRUE
#define MENU_PASTE TRUE
#define MENU_TAB_ADD_TAB TRUE
#define MENU_TAB_REMOVE_TAB TRUE
#define MENU_TAB_EDIT_LABEL TRUE
#define MENU_NEW_WINDOW TRUE
#define MENU_FONT_ZOOM_IN TRUE
#define MENU_FONT_ZOOM_OUT TRUE
#define MENU_FONT_ZOOM_100 TRUE
#define MENU_RESET TRUE
#define MENU_RESET_AND_CLEAR TRUE
#define MENU_QUIT TRUE
#define MENU_INPUT_METHOD TRUE
#ifndef MENU_ENCODING_LIST
#define MENU_ENCODING_LIST "UTF-8"
#endif
static char *menu_custom[] = {
MENU_CUSTOM
};
int menu_custom_size = sizeof(menu_custom) / sizeof(menu_custom[0]);
#endif

#ifdef TAB_INFO_AT_TITLE
#undef SHOW_WINDOW_TITLE /* prevent duplicated definitions */
#define SHOW_WINDOW_TITLE TRUE
#endif

#if SHOW_WINDOW_TITLE
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#if DOUBLE_PRESS_HOTKEY
#ifndef DOUBLE_PRESS_TIME
#define DOUBLE_PRESS_TIME 300000
#endif
#endif

#if COMMAND_SHOW_VERSION
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#ifdef CTRL_OPEN_NEW_WINDOW
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#if MENU_NEW_WINDOW
#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#endif
#endif

#ifdef COLOR_BACKGROUND
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_BOLD
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_CURSOR
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_DIM
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_FOREGROUND
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#ifdef COLOR_HIGHLIGHT
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 1
#endif

#if COLOR_STYLE_LINUX
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 0
#endif

#if COLOR_STYLE_RXVT
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 0
#endif

#if COLOR_STYLE_TANGO
#undef SET_DEFAULT_COLORS
#define SET_DEFAULT_COLORS 0
#endif

#if COLOR_STYLE_XTERM
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

#ifdef TAB_LABEL_STYLE_CUSTOM
#undef TAB_LABEL
#undef TAB_LABEL_STYLE_POEM
#define TAB_LABEL_INIT TRUE
static char *label_style_custom[] = {
TAB_LABEL_STYLE_CUSTOM
};
int label_style_size = sizeof(label_style_custom) / sizeof(label_style_custom[0]);
#endif

#if TAB_LABEL_STYLE_POEM
#undef TAB_LABEL
#define TAB_LABEL_INIT TRUE
/* Taken from http://zh.wikipedia.org/wiki/%E5%8D%83%E5%AD%97%E6%96%87 */
static char *label_style_poem[] = {
"天","地","玄","黃","宇","宙","洪","荒",
"日","月","盈","昃","辰","宿","列","張",
"寒","來","暑","往","秋","收","冬","藏",
"閏","餘","成","歲","律","呂","調","陽"
};
int label_style_size = sizeof(label_style_poem) / sizeof(label_style_poem[0]);
#endif

#ifdef TAB_LABEL
#define TAB_LABEL_INIT TRUE
#endif

#ifndef TAB_LABEL
#undef TAB_LABEL_NUMBER
#endif

#if COLOR_STYLE_LINUX
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

#if COLOR_STYLE_RXVT
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

#if COLOR_STYLE_TANGO
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

#if COLOR_STYLE_XTERM
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

#ifdef DEFAULT_FONT_SIMPLE
#undef DEFAULT_FONT
#endif

#ifdef CTRL_FONT_BIGGER
#define FONT_CHANGE_SIZE TRUE
#endif

#ifdef CTRL_FONT_SMALLER
#undef FONT_CHANGE_SIZE /* prevent duplicated definitions */
#define FONT_CHANGE_SIZE TRUE
#endif

#if MENU_FONT_ZOOM_IN || MENU_FONT_ZOOM_OUT
#undef FONT_CHANGE_SIZE /* prevent duplicated definitions */
#define FONT_CHANGE_SIZE TRUE
#endif

#if FONT_CHANGE_SIZE
#ifndef DEFAULT_FONT
#define DEFAULT_FONT "Monospace"
#endif
#undef DEFAULT_FONT_SIMPLE
#endif

#ifdef DEFAULT_FONT
#ifndef DEFAULT_FONT_SIZE
#define DEFAULT_FONT_SIZE 10
#endif
char font1str[32];
int font1_size = DEFAULT_FONT_SIZE;
#endif

#ifdef MENU_ENCODING_LIST
#ifdef MENU_ENCODING_DEFAULT
#define CHAR_ENCODING char
#else
#define CHAR_ENCODING static char
#endif
CHAR_ENCODING *encoding[] = {
MENU_ENCODING_LIST
};
#endif

#if COMMAND_EXEC_COMMAND
#define VTE_DEFAULT_COMMAND default_command
#define VTE_DEFAULT_ARGV default_argv
char default_command[32];
char **default_argv = NULL;
#else
#define VTE_DEFAULT_COMMAND DEFAULT_COMMAND
#define VTE_DEFAULT_ARGV DEFAULT_ARGV
#endif

#ifdef CTRL_SATURATION_MORE
#define ADJUST_SATURATION TRUE
#endif

#ifdef CTRL_SATURATION_LESS
#undef ADJUST_SATURATION /* prevent duplicated definitions */
#define ADJUST_SATURATION TRUE
#endif

#if ADJUST_SATURATION
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.5
#endif
double saturation_level = BACKGROUND_SATURATION;
#endif

#if TAB
GArray *terminals;
#endif

#if MENU
GtkWidget *menu;
#endif

#if STATUS_BAR
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

#if DOUBLE_PRESS_HOTKEY
#ifdef CTRL_TAB_OPEN_NEW_TAB
gint64 now_time_1;
gint64 last_time_1 = 0;
#endif
#ifdef CTRL_TAB_CLOSE_TAB
gint64 now_time_2;
gint64 last_time_2 = 0;
#endif
#endif

struct terminal {
  GtkWidget *vte;
#if GET_VTE_CHILD_PID
  int pid;
#endif
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  GtkWidget *hbox;
  GtkWidget *scrollbar;
#endif
} term;

gboolean sakura_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
gboolean sakura_popup(GtkWidget *widget, GdkEvent *event);
gint64 current_time();
void change_statusbar_encoding();
void change_window_title();
void delete_event();
void do_clear();
void do_copy();
void do_edit_label();
void do_new_window();
void do_paste();
void do_reset();
void do_zoom_100();
void do_zoom_in();
void do_zoom_out();
void sakura_add_tab();
void sakura_del_tab();
void set_encoding(GtkWidget *widget, void *data);

#if CTRL
gboolean sakura_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  if (event->state & GDK_CONTROL_MASK) {

#ifdef CTRL_OPEN_NEW_WINDOW
    if CTRL_OPEN_NEW_WINDOW {
      char new_window_str[32];
      sprintf(new_window_str, "%s &", PROGRAM_NAME);
      system(new_window_str);
      return TRUE;
    }
#endif

#ifdef CTRL_EDIT_ENCODING
    if CTRL_EDIT_ENCODING {
      char *encoding_name = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term.vte));
      GtkWidget *encoding_entry = gtk_entry_new();
      GtkWidget *encoding_dialog;
#ifdef BUTTON_ORDER
      gboolean button_order = BUTTON_ORDER;
#if BUTTON_ORDER_GTK_STYLE
      g_object_get(gtk_settings_get_default(), "gtk-alternative-button-order", &button_order, NULL);
#endif
      if (button_order)
        encoding_dialog = gtk_dialog_new_with_buttons(encoding_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
      else
#endif /* BUTTON_ORDER */
        encoding_dialog = gtk_dialog_new_with_buttons(encoding_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
      gtk_dialog_set_default_response(GTK_DIALOG(encoding_dialog), GTK_RESPONSE_OK);
      gtk_entry_set_text(GTK_ENTRY(encoding_entry), encoding_name);
      gtk_entry_set_activates_default(GTK_ENTRY(encoding_entry), TRUE);
      gtk_container_add(GTK_CONTAINER(GTK_DIALOG(encoding_dialog)->vbox), encoding_entry);
      gtk_widget_show_all (encoding_dialog);
      if (gtk_dialog_run(GTK_DIALOG(encoding_dialog)) == GTK_RESPONSE_OK) {
#if TAB
        term = g_array_index(terminals, struct terminal, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
#endif
        vte_terminal_set_encoding(VTE_TERMINAL(term.vte), gtk_entry_get_text(GTK_ENTRY(encoding_entry)));
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
      char *label_name = (char*)gtk_label_get_text(GTK_LABEL(gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook))))));
      if (label_name == NULL)
        label_name = "";
      GtkWidget *entry = gtk_entry_new();
      GtkWidget *dialog;
#ifdef BUTTON_ORDER
      gboolean button_order = BUTTON_ORDER;
#if BUTTON_ORDER_GTK_STYLE
      g_object_get(gtk_settings_get_default(), "gtk-alternative-button-order", &button_order, NULL);
#endif
      if (button_order)
        dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
      else
#endif /* BUTTON_ORDER */
        dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
      gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
      gtk_entry_set_text(GTK_ENTRY(entry), label_name);
      gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
      gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), entry);
      gtk_widget_show_all (dialog);
      if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
        gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.hbox, gtk_label_new(gtk_entry_get_text(GTK_ENTRY(entry))));
#else
        gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.vte, gtk_label_new(gtk_entry_get_text(GTK_ENTRY(entry))));
#endif
      gtk_widget_destroy(dialog);
      return TRUE;
    }
#endif /* CTRL_TAB_EDIT_LABEL */

#if CTRL_TAB_JUMP_NUMBER
    if (event->keyval <= GDK_9 && event->keyval >= GDK_0) {
      int pages = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
      switch (event->keyval) {
        case GDK_1:
          gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
          break;
        case GDK_2:
          if (pages > 1)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
          else
            return FALSE;
          break;
        case GDK_3:
          if (pages > 2)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);
          else
            return FALSE;
          break;
        case GDK_4:
          if (pages > 3)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);
          else
            return FALSE;
          break;
        case GDK_5:
          if (pages > 4)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 4);
          else
            return FALSE;
          break;
        case GDK_6:
          if (pages > 5)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 5);
          else
            return FALSE;
          break;
        case GDK_7:
          if (pages > 6)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 6);
          else
            return FALSE;
          break;
        case GDK_8:
          if (pages > 7)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 7);
          else
            return FALSE;
          break;
        case GDK_9:
          if (pages > 8)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 8);
          else
            return FALSE;
          break;
        case GDK_0:
          if (pages > 9)
            gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 9);
          else
            return FALSE;
          break;
      }
#if STATUS_BAR
      change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
      change_window_title();
#endif
      return TRUE;
    }
#endif /* CTRL_TAB_JUMP_NUMBER */

#ifdef CTRL_TOGGLE_BACKGROUND
    if CTRL_TOGGLE_BACKGROUND {
      background_status++;
      if (background_status == background_order_size)
        background_status = 0;
      if (background_order[background_status] == "Transparent") {
        vte_terminal_set_background_transparent(VTE_TERMINAL(term.vte), TRUE);
        return TRUE;
      }
      if (background_order[background_status] == "Image") {
        char imgstr[64];
        sprintf(imgstr, "%s/%s", g_getenv("HOME"), BACKGROUND_IMAGE);
        vte_terminal_set_background_transparent(VTE_TERMINAL(term.vte), FALSE);
        vte_terminal_set_background_image_file(VTE_TERMINAL(term.vte), imgstr);
        return TRUE;
      }
      if (background_order[background_status] == "No background") {
        vte_terminal_set_background_transparent(VTE_TERMINAL(term.vte), FALSE);
        vte_terminal_set_background_image_file(VTE_TERMINAL(term.vte), "/dev/null");
        return TRUE;
      }
      return FALSE;
    }
#endif /* CTRL_TOGGLE_BACKGROUND */

#ifdef CTRL_SATURATION_MORE
    if CTRL_SATURATION_MORE {
      saturation_level += 0.1;
      if (saturation_level > 1)
        saturation_level = 1;
      vte_terminal_set_background_saturation(VTE_TERMINAL(term.vte), saturation_level);
      return TRUE;
    }
#endif

#ifdef CTRL_SATURATION_LESS
    if CTRL_SATURATION_LESS {
      saturation_level -= 0.1;
      if (saturation_level < 0)
        saturation_level = 0;
      vte_terminal_set_background_saturation(VTE_TERMINAL(term.vte), saturation_level);
      return TRUE;
    }
#endif

#ifdef CTRL_RESET_TERMINAL
    if CTRL_RESET_TERMINAL {
      vte_terminal_reset(VTE_TERMINAL(term.vte), 1,  0);
      return TRUE;
    }
#endif

#ifdef CTRL_RESET_AND_CLEAR_TERM
    if CTRL_RESET_AND_CLEAR_TERM {
      vte_terminal_reset(VTE_TERMINAL(term.vte), 1,  1);
      return TRUE;
    }
#endif

#ifdef CTRL_COPY_TO_CLIPBOARD
    if CTRL_COPY_TO_CLIPBOARD {
      vte_terminal_copy_clipboard(VTE_TERMINAL(term.vte));
      return TRUE;
    }
#endif

#ifdef CTRL_PASTE_FROM_CLIPBOARD
    if CTRL_PASTE_FROM_CLIPBOARD {
      vte_terminal_paste_clipboard(VTE_TERMINAL(term.vte));
      return TRUE;
    }
#endif

#ifdef CTRL_FONT_BIGGER
    if CTRL_FONT_BIGGER {
      font1_size++;
      goto font_size_changed;
    }
#endif

#ifdef CTRL_FONT_SMALLER
    if CTRL_FONT_SMALLER {
      font1_size--;
      goto font_size_changed;
    }
#endif

#ifdef CTRL_FONT_DEFAULT_SIZE
    if CTRL_FONT_DEFAULT_SIZE {
      font1_size = DEFAULT_FONT_SIZE;
      goto font_size_changed;
    }
#endif

#ifdef CTRL_TAB_FIRST_TAB
    if CTRL_TAB_FIRST_TAB {
      gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
      change_window_title();
#endif
      return TRUE;
    }
#endif /* CTRL_TAB_FIRST_TAB */

#ifdef CTRL_TAB_LAST_TAB
    if CTRL_TAB_LAST_TAB {
      gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
      change_window_title();
#endif
      return TRUE;
    }
#endif /* CTRL_TAB_LAST_TAB */

#ifdef CTRL_TAB_PREVIOUS_TAB
    if CTRL_TAB_PREVIOUS_TAB {
      int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (npages)
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages - 1);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
      change_window_title();
#endif
      return TRUE;
    }
#endif /* CTRL_TAB_PREVIOUS_TAB */

#ifdef CTRL_TAB_NEXT_TAB
    if CTRL_TAB_NEXT_TAB {
      int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (npages == (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1))
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages + 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
      change_window_title();
#endif
      return TRUE;
    }
#endif /* CTRL_TAB_NEXT_TAB */

#ifdef CTRL_TAB_OPEN_NEW_TAB
    if CTRL_TAB_OPEN_NEW_TAB {
#if DOUBLE_PRESS_HOTKEY
      if (last_time_1 != 0) {
        now_time_1 = current_time();
        if (now_time_1 - last_time_1 < DOUBLE_PRESS_TIME) {
#endif
          sakura_add_tab();

#if STATUS_BAR
          change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
          change_window_title();
#endif

#if DOUBLE_PRESS_HOTKEY
          last_time_1 = 0;
#endif

          return TRUE;

#if DOUBLE_PRESS_HOTKEY
        }
      }
      last_time_1 = current_time();
#endif

      return FALSE;
    }
#endif /* CTRL_TAB_OPEN_NEW_TAB */

#ifdef CTRL_TAB_CLOSE_TAB
    if CTRL_TAB_CLOSE_TAB {

#if DOUBLE_PRESS_HOTKEY
      if (last_time_2 != 0) {
        now_time_2 = current_time();
        if (now_time_2 - last_time_2 < DOUBLE_PRESS_TIME) {
#endif

          sakura_del_tab();

#if DOUBLE_PRESS_HOTKEY
          last_time_2 = 0;
#endif

          return TRUE;

#if DOUBLE_PRESS_HOTKEY
        }
      }
      last_time_2 = current_time();
#endif

      return FALSE;
    }
#endif /* CTRL_TAB_CLOSE_TAB */

  }
  return FALSE; /* (event->state & GDK_CONTROL_MASK) */

#if FONT_CHANGE_SIZE
font_size_changed:
  sprintf(font1str, "%s %d", DEFAULT_FONT, font1_size);
#if TAB
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), font1str);
#if TAB
  }
#endif
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
    return TRUE;
#endif /* FONT_CHANGE_SIZE */
}
#endif /* CTRL */

#if MENU
gboolean sakura_popup(GtkWidget *widget, GdkEvent *event)
{
  GdkEventButton *event_button;
  if (event->type == GDK_BUTTON_PRESS) {
    event_button = (GdkEventButton *) event;
    if (event_button->button == 3) {
      gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, event_button->button, event_button->time);
      return TRUE;
    }
  }
  return FALSE;
}
#endif

#if DOUBLE_PRESS_HOTKEY
gint64 current_time()
{
  struct timeval tval;
  gettimeofday(&tval, NULL);
  return (gint64)tval.tv_sec * 1000000 + tval.tv_usec;
}
#endif

#if STATUS_BAR
void change_statusbar_encoding()
{
#if TAB
  term = g_array_index(terminals, struct terminal, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
#endif
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, vte_terminal_get_encoding(VTE_TERMINAL(term.vte)));
}
#endif

#if TAB_INFO_AT_TITLE
void change_window_title()
{
  int index = gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook));
  if (index == 1)
    gtk_window_set_title(GTK_WINDOW(main_window), PROGRAM_NAME);
  else {
    char tabtitle[32];
    sprintf(tabtitle, "%s - tab %d of %d", PROGRAM_NAME, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)) + 1, index);
    gtk_window_set_title(GTK_WINDOW(main_window), tabtitle);
  }
}
#endif

#if CLOSE_SAFELY
void delete_event()
{
  while (1)
    sakura_del_tab();
}
#endif

#if MENU_COPY
void do_copy()
{
  vte_terminal_copy_clipboard(VTE_TERMINAL(term.vte));
}
#endif

#if MENU_RESET_AND_CLEAR
void do_clear()
{
  vte_terminal_reset(VTE_TERMINAL(term.vte), 1,  1);
}
#endif

#if MENU_TAB_EDIT_LABEL
void do_edit_label()
{
  char *label_name = (char*)gtk_label_get_text(GTK_LABEL(gtk_notebook_get_tab_label(GTK_NOTEBOOK(notebook), gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook))))));
  if (label_name == NULL)
    label_name = "";
  GtkWidget *entry = gtk_entry_new();
  GtkWidget *dialog;
#ifdef BUTTON_ORDER
  gboolean button_order = BUTTON_ORDER;
#if BUTTON_ORDER_GTK_STYLE
  g_object_get(gtk_settings_get_default(), "gtk-alternative-button-order", &button_order, NULL);
#endif
  if (button_order)
    dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);
  else
#endif /* BUTTON_ORDER */
    dialog = gtk_dialog_new_with_buttons(label_name, GTK_WINDOW(main_window), GTK_DIALOG_NO_SEPARATOR | GTK_DIALOG_DESTROY_WITH_PARENT, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
  gtk_entry_set_text(GTK_ENTRY(entry), label_name);
  gtk_entry_set_activates_default(GTK_ENTRY(entry), TRUE);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), entry);
  gtk_widget_show_all (dialog);
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK)
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.hbox, gtk_label_new(gtk_entry_get_text(GTK_ENTRY(entry))));
#else
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.vte, gtk_label_new(gtk_entry_get_text(GTK_ENTRY(entry))));
#endif
  gtk_widget_destroy(dialog);
}
#endif

#if MENU_NEW_WINDOW
void do_new_window()
{
  char new_window_str[32];
  sprintf(new_window_str, "%s &", PROGRAM_NAME);
  system(new_window_str);
}
#endif

#if MENU_PASTE
void do_paste()
{
  vte_terminal_paste_clipboard(VTE_TERMINAL(term.vte));
}
#endif

#if MENU_RESET
void do_reset()
{
  vte_terminal_reset(VTE_TERMINAL(term.vte), 1,  0);
}
#endif

#if MENU_FONT_ZOOM_100
void do_zoom_100()
{
  font1_size = DEFAULT_FONT_SIZE;
  sprintf(font1str, "%s %d", DEFAULT_FONT, font1_size);
#if TAB
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), font1str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_FONT_ZOOM_IN
void do_zoom_in()
{
  font1_size++;
  sprintf(font1str, "%s %d", DEFAULT_FONT, font1_size);
#if TAB
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), font1str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

#if MENU_FONT_ZOOM_OUT
void do_zoom_out()
{
  font1_size--;
  sprintf(font1str, "%s %d", DEFAULT_FONT, font1_size);
#if TAB
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
#endif
    vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), font1str);
#if TAB
  }
#endif
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
}
#endif

void sakura_add_tab()
{
#ifdef TAB_LABEL_INIT
  GtkWidget *label;
#endif
#ifdef TAB_LABEL
#if TAB_LABEL_NUMBER
  label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) + 1)));
#else
  label = gtk_label_new(TAB_LABEL);
#endif
#endif

#ifdef TAB_LABEL_STYLE_CUSTOM
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < label_style_size)
    label = gtk_label_new(g_strdup_printf("%s", label_style_custom[gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))]));
  else
    label = NULL;
#endif

#if TAB_LABEL_STYLE_POEM
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < label_style_size)
    label = gtk_label_new(g_strdup_printf("%s", label_style_poem[gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook))]));
  else
    label = NULL;
#endif

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  term.hbox = gtk_hbox_new(FALSE, 0);
#endif

  term.vte = vte_terminal_new();

#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  term.scrollbar = gtk_vscrollbar_new(vte_terminal_get_adjustment(VTE_TERMINAL(term.vte)));
#endif

#if SCROLLBAR_LEFT
  gtk_box_pack_start(GTK_BOX(term.hbox), term.scrollbar, FALSE, FALSE, 0);
#endif
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  gtk_box_pack_start(GTK_BOX(term.hbox), term.vte, TRUE, TRUE, 0);
#endif
#if SCROLLBAR_RIGHT
  gtk_box_pack_start(GTK_BOX(term.hbox), term.scrollbar, FALSE, FALSE, 0);
#endif

#if TAB_NEW_TAB_OLD_PATH
  char *default_directory = (char*)DEFAULT_DIRECTORY;
  if  (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) > 0) {
    struct terminal oldterm;
    oldterm = g_array_index(terminals, struct terminal, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
    default_directory = g_file_read_link(g_strdup_printf("/proc/%d/cwd", oldterm.pid), NULL);
  }
#endif

#if GET_VTE_CHILD_PID
  term.pid = vte_terminal_fork_command(VTE_TERMINAL(term.vte), VTE_DEFAULT_COMMAND, VTE_DEFAULT_ARGV, DEFAULT_ENVV, VTE_DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);
#else
  vte_terminal_fork_command(VTE_TERMINAL(term.vte), VTE_DEFAULT_COMMAND, VTE_DEFAULT_ARGV, DEFAULT_ENVV, VTE_DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);
#endif

#ifdef EMULATION_TYPE
  vte_terminal_set_emulation(VTE_TERMINAL(term.vte), EMULATION_TYPE);
#endif

#ifdef ALLOW_BOLD
  vte_terminal_set_allow_bold(VTE_TERMINAL(term.vte), ALLOW_BOLD);
#endif

#ifdef BELL_AUDIBLE
  vte_terminal_set_audible_bell(VTE_TERMINAL(term.vte), BELL_AUDIBLE);
#endif

#ifdef BELL_VISIBLE
  vte_terminal_set_visible_bell(VTE_TERMINAL(term.vte), BELL_VISIBLE);
#endif

#ifdef BACKGROUND_IMAGE
  char imgstr[64];
  sprintf(imgstr, "%s/%s", g_getenv("HOME"), BACKGROUND_IMAGE);
  vte_terminal_set_background_image_file(VTE_TERMINAL(term.vte), imgstr);
#endif

#ifdef BACKGROUND_TINT_COLOR
#if BACKGROUND_EXIST
  GdkColor color_tint;
  gdk_color_parse(BACKGROUND_TINT_COLOR, &color_tint);
  vte_terminal_set_background_tint_color(VTE_TERMINAL(term.vte), &color_tint);
#endif
#endif

#ifdef BACKGROUND_SATURATION
#if BACKGROUND_EXIST
  vte_terminal_set_background_saturation(VTE_TERMINAL(term.vte), BACKGROUND_SATURATION);
#endif
#endif

#ifdef BACKGROUND_TRANSPARENT
  vte_terminal_set_background_transparent(VTE_TERMINAL(term.vte), BACKGROUND_TRANSPARENT);
#endif

#ifdef BINDING_BACKSPACE_KEY
  vte_terminal_set_backspace_binding(VTE_TERMINAL(term.vte), BINDING_BACKSPACE_KEY);
#endif

#ifdef BINDING_DELETE_KEY
  vte_terminal_set_delete_binding(VTE_TERMINAL(term.vte), BINDING_DELETE_KEY);
#endif

#if COLOR_STYLE_LINUX
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_linux, 16);
#endif

#if COLOR_STYLE_RXVT
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_rxvt, 16);
#endif

#if COLOR_STYLE_TANGO
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_tango, 16);
#endif

#if COLOR_STYLE_XTERM
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_xterm, 16);
#endif

#if SET_DEFAULT_COLORS
  vte_terminal_set_default_colors(VTE_TERMINAL(term.vte));
#endif

#ifdef COLOR_BACKGROUND
  GdkColor color_bg;
  gdk_color_parse(COLOR_BACKGROUND, &color_bg);
  vte_terminal_set_color_background(VTE_TERMINAL(term.vte), &color_bg);
#endif

#ifdef COLOR_BOLD
  GdkColor color_bold;
  gdk_color_parse(COLOR_BOLD, &color_bold);
  vte_terminal_set_color_bold(VTE_TERMINAL(term.vte), &color_bold);
#endif

#ifdef COLOR_CURSOR
  GdkColor color_cursor;
  gdk_color_parse(COLOR_CURSOR, &color_cursor);
  vte_terminal_set_color_cursor(VTE_TERMINAL(term.vte), &color_cursor);
#endif

#ifdef COLOR_DIM
  GdkColor color_dim;
  gdk_color_parse(COLOR_DIM, &color_dim);
  vte_terminal_set_color_dim(VTE_TERMINAL(term.vte), &color_dim);
#endif

#ifdef COLOR_FOREGROUND
  GdkColor color_fg;
  gdk_color_parse(COLOR_FOREGROUND, &color_fg);
  vte_terminal_set_color_foreground(VTE_TERMINAL(term.vte), &color_fg);
#endif

#ifdef COLOR_HIGHLIGHT
  GdkColor color_highlight;
  gdk_color_parse(COLOR_HIGHLIGHT, &color_highlight);
  vte_terminal_set_color_highlight(VTE_TERMINAL(term.vte), &color_highlight);
#endif

#if CURSOR_BLINKS
  vte_terminal_set_cursor_blinks(VTE_TERMINAL(term.vte), CURSOR_BLINKS);
#endif

#ifdef DEFAULT_ENCODING
  vte_terminal_set_encoding(VTE_TERMINAL(term.vte), DEFAULT_ENCODING);
#endif

#ifdef DEFAULT_FONT_SIMPLE
#ifdef ANTI_ALIAS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(term.vte), DEFAULT_FONT_SIMPLE, ANTI_ALIAS);
#else
  vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), DEFAULT_FONT_SIMPLE);
#endif
#endif

#ifdef DEFAULT_FONT
#ifdef ANTI_ALIAS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(term.vte), font1str, ANTI_ALIAS);
#else
  vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), font1str);
#endif
#endif

#ifdef MOUSE_AUTOHIDE
  vte_terminal_set_mouse_autohide(VTE_TERMINAL(term.vte), MOUSE_AUTOHIDE);
#endif

#ifdef SCROLL_BACKGROUND
  vte_terminal_set_scroll_background(VTE_TERMINAL(term.vte), SCROLL_BACKGROUND);
#endif

#ifdef SCROLL_ON_KEYSTROKE
  vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(term.vte), SCROLL_ON_KEYSTROKE);
#endif

#ifdef SCROLL_ON_OUTPUT
  vte_terminal_set_scroll_on_output(VTE_TERMINAL(term.vte), SCROLL_ON_OUTPUT);
#endif

#ifdef SCROLL_LINES
  vte_terminal_set_scrollback_lines(VTE_TERMINAL(term.vte), SCROLL_LINES);
#endif

#if DEFAULT_COLUMNS && DEFAULT_ROWS
  vte_terminal_set_size(VTE_TERMINAL(term.vte), DEFAULT_COLUMNS, DEFAULT_ROWS);
#endif

#ifdef WORD_CHARS
  vte_terminal_set_word_chars(VTE_TERMINAL(term.vte), WORD_CHARS);
#endif

#ifdef TAB_LABEL_INIT
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.hbox, label);
#else
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.vte, label);
#endif
#else /* TAB_LABEL_INIT */
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.hbox, NULL);
#else
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.vte, NULL);
#endif
#endif /* TAB_LABEL_INIT */

  g_signal_connect(term.vte, "child-exited", sakura_del_tab, NULL);

#if MENU
  g_signal_connect(term.vte, "button-press-event", G_CALLBACK(sakura_popup), NULL);
#endif

#if TAB
  g_array_append_val(terminals, term);
#endif

#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) {
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
  } else
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), TABBAR);
  gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
#endif

  gtk_widget_show_all(notebook);

#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif

#if TAB_EXPAND
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  gtk_container_child_set(GTK_CONTAINER(notebook), term.hbox, "tab-expand", TRUE, NULL);
#else
  gtk_container_child_set(GTK_CONTAINER(notebook), term.vte, "tab-expand", TRUE, NULL);
#endif
#endif /* TAB_EXPAND */

  gtk_window_set_focus(GTK_WINDOW(main_window), term.vte);
}

void sakura_del_tab()
{
#ifdef TAB_LABEL_INIT
  GtkWidget *label;
#endif
  int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));

#if CLOSE_SAFELY
#if TAB
  term = g_array_index(terminals, struct terminal, npages);
#endif
  /* Dirty hack. Prevent background applications close with tab */
  char pidstr[32];
  sprintf(pidstr, "kill -KILL %d\0", term.pid);
  system(pidstr);
#endif /* CLOSE_SAFELY */

#if TAB
  g_array_remove_index(terminals, npages);
#endif

  gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), npages);
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < 1)
    gtk_main_quit();

#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages);
#endif

#if TABBAR_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) {
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_window_resize(GTK_WINDOW(main_window), 1, 1);
  }
#endif

#if TAB_LABEL_NUMBER
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
    label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (i + 1)));
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.hbox, label);
#else
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.vte, label);
#endif
  }
#endif /* TAB_LABEL_NUMBER */

#ifdef TAB_LABEL_STYLE_CUSTOM
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
    if (i < label_style_size) {
      label = gtk_label_new(g_strdup_printf("%s", label_style_custom[i]));
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
      gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.hbox, label);
#else
      gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.vte, label);
#endif
    }
  }
#endif

#if TAB_LABEL_STYLE_POEM
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
    if (i < label_style_size) {
      label = gtk_label_new(g_strdup_printf("%s", label_style_poem[i]));
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
      gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.hbox, label);
#else
      gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.vte, label);
#endif
    }
  }
#endif

#if STATUS_BAR
  change_statusbar_encoding();
#endif
#if TAB_INFO_AT_TITLE
  change_window_title();
#endif
}

void set_encoding(GtkWidget *widget, void *data)
{
#if TAB
  term = g_array_index(terminals, struct terminal, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
#endif
  vte_terminal_set_encoding(VTE_TERMINAL(term.vte), (char*)data);
#if STATUS_BAR
  change_statusbar_encoding();
#endif
}

int main(int argc, char **argv)
{
#if COMMAND_SHOW_VERSION
  if (argc == 2 && !strcmp(argv[1], "-v")) {
    printf("%s, version %s\n", PROGRAM_NAME, EVILVTE_VERSION);
    return 0;
  }
#endif

#if COMMAND_SHOW_CONFIG
  if (argc == 2 && !strcmp(argv[1], "-o")) {
    system("showvte");
    return 0;
  }
#endif

#if COMMAND_EXEC_COMMAND
  if (argc > 2 && !strcmp(argv[1], "-e")) {
    sprintf(default_command, "%s", argv[2]);
    int k;
    for (k = 0 ; k < argc ; k++)
    {
      if (!strcmp(argv[k], "-e")) {
        if (++k != argc)
          default_argv = &(argv[k]);
      }
    }
  }
  else
    sprintf(default_command, "%s", DEFAULT_COMMAND);
#endif

#ifdef DEFAULT_FONT
  sprintf(font1str, "%s %d", DEFAULT_FONT, font1_size);
#endif

  gtk_init(NULL, NULL);

#if TAB
  terminals = g_array_new(FALSE, TRUE, sizeof(struct terminal));
#endif

  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#if SHOW_WINDOW_TITLE
  gtk_window_set_title(GTK_WINDOW(main_window), PROGRAM_NAME);
#endif

#if SHOW_WINDOW_ICON
  gtk_window_set_icon_from_file(GTK_WINDOW(main_window), ICON_DIR"/evilvte.png", NULL);
#endif

#if STATUS_BAR
  vbox = gtk_vbox_new(FALSE, 0);
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
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), TABBAR);
#endif

#if TABBAR_MOUSE_SCROLLABLE
  gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TABBAR_MOUSE_SCROLLABLE);
#endif

#if TABBAR_MENU_SELECT_TAB
  gtk_notebook_popup_enable(GTK_NOTEBOOK(notebook));
#endif

#if TAB_AT_BOTTOM
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), 3);
#endif

#if TAB_AT_LEFT
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), 0);
#endif

#if TAB_AT_RIGHT
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), 1);
#endif

#if TAB_AT_TOP
  gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), 2);
#endif

#if !TAB
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
#endif

  sakura_add_tab();

#if COMMAND_EXEC_COMMAND && TAB
  sprintf(default_command, "%s", DEFAULT_COMMAND);
  default_argv = DEFAULT_ARGV;
#endif

#if TAB_INITIAL_NUMBER
  int j;
  for (j = 1 ; j < TAB_INITIAL_NUMBER ; j++)
    sakura_add_tab();
#endif

#if STATUS_BAR
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, 1, 0);
  statusbar = gtk_statusbar_new();
  change_statusbar_encoding();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, 0, 1, 0);
#else
  gtk_container_add(GTK_CONTAINER(main_window), notebook);
#endif

#if CLOSE_SAFELY
  g_signal_connect(main_window, "delete_event", delete_event, NULL);
#endif

#if CTRL
  g_signal_connect(main_window, "key-press-event", G_CALLBACK(sakura_key_press), NULL);
#endif

  gtk_widget_show_all(main_window);

#if MENU
  menu = gtk_menu_new();
#endif

#if MENU_SEPARATOR_1 || MENU_SEPARATOR_2
  GtkWidget *menu_item;
#endif

#ifdef MENU_CUSTOM
  GtkWidget *menu_item;
  int m;
  for (m = 0 ; m < menu_custom_size ; m++)
#endif

  { /* MENU_CUSTOM */
#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Copy")
#endif
#if MENU_COPY
    {
      GtkWidget *menu_copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, NULL);
      gtk_menu_append(menu, menu_copy);
      g_signal_connect(menu_copy, "activate", do_copy, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Paste")
#endif
#if MENU_PASTE
    {
      GtkWidget *menu_paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, NULL);
      gtk_menu_append(menu, menu_paste);
      g_signal_connect(menu_paste, "activate", do_paste, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Add tab")
#endif
#if MENU_TAB_ADD_TAB
    {
      GtkWidget *menu_add_tab = gtk_image_menu_item_new_from_stock(GTK_STOCK_ADD, NULL);
      gtk_menu_append(menu, menu_add_tab);
      g_signal_connect(menu_add_tab, "activate", sakura_add_tab, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Close tab")
#endif
#if MENU_TAB_REMOVE_TAB
    {
      GtkWidget *menu_close_tab = gtk_image_menu_item_new_from_stock(GTK_STOCK_REMOVE, NULL);
      gtk_menu_append(menu, menu_close_tab);
      g_signal_connect(menu_close_tab, "activate", sakura_del_tab, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Edit label")
#endif
#if MENU_TAB_EDIT_LABEL
    {
      GtkWidget *menu_edit_label = gtk_image_menu_item_new_from_stock(GTK_STOCK_EDIT, NULL);
      gtk_menu_append(menu, menu_edit_label);
      g_signal_connect(menu_edit_label, "activate", do_edit_label, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "New window")
#endif
#if MENU_NEW_WINDOW
    {
      GtkWidget *menu_new_window = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
      gtk_menu_append(menu, menu_new_window);
      g_signal_connect(menu_new_window, "activate", do_new_window, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Zoom in")
#endif
#if MENU_FONT_ZOOM_IN
    {
      GtkWidget *menu_zoom_in = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_IN, NULL);
      gtk_menu_append(menu, menu_zoom_in);
      g_signal_connect(menu_zoom_in, "activate", do_zoom_in, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Zoom out")
#endif
#if MENU_FONT_ZOOM_OUT
    {
      GtkWidget *menu_zoom_out = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_OUT, NULL);
      gtk_menu_append(menu, menu_zoom_out);
      g_signal_connect(menu_zoom_out, "activate", do_zoom_out, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Zoom 100")
#endif
#if MENU_FONT_ZOOM_100
    {
      GtkWidget *menu_zoom_100 = gtk_image_menu_item_new_from_stock(GTK_STOCK_ZOOM_100, NULL);
      gtk_menu_append(menu, menu_zoom_100);
      g_signal_connect(menu_zoom_100, "activate", do_zoom_100, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Reset")
#endif
#if MENU_RESET
    {
      GtkWidget *menu_reset = gtk_image_menu_item_new_from_stock(GTK_STOCK_REFRESH, NULL);
      gtk_menu_append(menu, menu_reset);
      g_signal_connect(menu_reset, "activate", do_reset, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Reset and clear")
#endif
#if MENU_RESET_AND_CLEAR
    {
      GtkWidget *menu_clear = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLEAR, NULL);
      gtk_menu_append(menu, menu_clear);
      g_signal_connect(menu_clear, "activate", do_clear, NULL);
    }
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Quit")
#endif
#if MENU_QUIT
    {
      GtkWidget *menu_quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
      gtk_menu_append(menu, menu_quit);
#if CLOSE_SAFELY
      g_signal_connect(menu_quit, "activate", delete_event, NULL);
#else
      g_signal_connect(menu_quit, "activate", gtk_main_quit, NULL);
#endif
    }
#endif /* MENU_QUIT */

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Separator") {
      menu_item = gtk_separator_menu_item_new();
      gtk_menu_append(menu, menu_item);
    }
#endif

#if MENU_SEPARATOR_1
  menu_item = gtk_separator_menu_item_new();
  gtk_menu_append(menu, menu_item);
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Encoding list")
#endif
#ifdef MENU_ENCODING_LIST
    {
      int encoding_size = sizeof(encoding) / sizeof(encoding[0]);
      GtkWidget *encoding_item[encoding_size];
      int i;
      for (i = 0 ; i < encoding_size ; i++) {
#ifdef MENU_ENCODING_DEFAULT
        if (encoding[i] == "Default") {
          encoding[i] = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term.vte));
          encoding_item[i] = gtk_menu_item_new_with_label(MENU_ENCODING_DEFAULT);
        } else
#endif
          encoding_item[i] = gtk_menu_item_new_with_label(encoding[i]);
        gtk_menu_append(menu, encoding_item[i]);
        g_signal_connect(encoding_item[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
      }
    }
#endif /* MENU_ENCODING_LIST */

#if MENU_SEPARATOR_2
  menu_item = gtk_separator_menu_item_new();
  gtk_menu_append(menu, menu_item);
#endif

#ifdef MENU_CUSTOM
    if (menu_custom[m] == "Input method")
#endif
#if MENU_INPUT_METHOD
    {
      vte_terminal_im_append_menuitems(VTE_TERMINAL(term.vte), GTK_MENU_SHELL(menu));
    }
#endif

  } /* MENU_CUSTOM */

#if MENU
  gtk_widget_show_all(menu);
#endif

  gtk_main();
  return 0;
}
