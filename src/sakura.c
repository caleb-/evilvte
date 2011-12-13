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

/* Copyright (C) 2008  Wen-Yen Chuang <caleb@calno.com>
 *
 * Forked from sakura 2.0.1, http://www.pleyades.net/david/sakura.php
 */

#include <gdk/gdkkeysyms.h>
#include <vte/vte.h>
#include "config.h"

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

#ifdef BACKGROUND_IMAGE
#define BACKGROUND_EXIST TRUE
#endif

#ifdef BACKGROUND_TRANSPARENT
#undef BACKGROUND_EXIST /* undefine it here to prevent duplicated definition warning */
#define BACKGROUND_EXIST TRUE
#endif

#if TAB
#define ENABLE_KEY_PRESS TRUE
#endif

#if !TAB
#undef SHOW_WINDOW_BORDER
#undef TAB_AUTOHIDE
#undef TAB_BORDER
#undef TAB_BORDER_H
#undef TAB_BORDER_V
#undef TAB_INITIAL_NUMBER
#undef TAB_LABEL
#define SHOW_WINDOW_BORDER FALSE
#endif

#ifndef TAB_LABEL
#undef TAB_LABEL_NUMBER
#endif

#if DOUBLE_PRESS_HOTKEY
#ifndef DOUBLE_PRESS_TIME
#define DOUBLE_PRESS_TIME 300000
#endif
#endif

#if SHOW_WINDOW_VERSION
#ifndef SHOW_WINDOW_TITLE
#define SHOW_WINDOW_TITLE "evilvte"
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

#ifdef FONT_CHANGE_SIZE
#ifndef DEFAULT_FONT
#define DEFAULT_FONT "Monospace"
#endif
#undef DEFAULT_FONT_SIMPLE
#undef ENABLE_KEY_PRESS /* undefine it here to prevent duplicated definition warning */
#define ENABLE_KEY_PRESS TRUE
int width_f;
int height_f;
#endif /* FONT_CHANGE_SIZE */

#ifdef DEFAULT_FONT
#ifndef DEFAULT_FONT_SIZE
#define DEFAULT_FONT_SIZE 10
#endif
char font1str[32];
int font1_size = DEFAULT_FONT_SIZE;
#endif /* DEFAULT_FONT */

#ifdef ENCODING_LIST
#if ENCODING_LIST_DEFAULT
#define CHAR_ENCODING char
#else
#define CHAR_ENCODING static char
#endif
#define SHOW_MENU TRUE
CHAR_ENCODING *encoding[] = {
ENCODING_LIST
};
#endif /* ENCODING_LIST */

#if MENU_INPUT_METHOD
#undef SHOW_MENU /* undefine it here to prevent duplicated definition warning */
#define SHOW_MENU TRUE
#endif

#if TAB
GArray *terminals;
#endif

#if TAB_AUTOHIDE
int width;
int height;
#endif

#if SHOW_MENU
GtkWidget *menu;
#endif

#if STATUS_BAR
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

#if DOUBLE_PRESS_HOTKEY
#ifdef CTRL_NEW_TAB
gint64 now_time_1;
gint64 last_time_1 = 0;
#endif
#ifdef CTRL_REMOVE_TAB
gint64 now_time_2;
gint64 last_time_2 = 0;
#endif
#endif /* DOUBLE_PRESS_HOTKEY */

struct terminal {
  GtkWidget *vte;
#if CLOSE_SAFE
  int pid;
#endif
#ifdef TAB_LABEL
  GtkWidget *label;
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
void delete_event();
void sakura_add_tab();
void sakura_del_tab();
void set_encoding(GtkWidget *widget, void *data);

#if ENABLE_KEY_PRESS
gboolean sakura_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  if (event->state & GDK_CONTROL_MASK) {

#if FONT_CHANGE_SIZE
#ifdef CTRL_FONT_BIGGER
    if CTRL_FONT_BIGGER {
      font1_size++;
      goto font_size_changed;
    }
#endif /* CTRL_FONT_BIGGER */

#ifdef CTRL_FONT_SMALLER
    if CTRL_FONT_SMALLER {
      font1_size--;
      goto font_size_changed;
    }
#endif /* CTRL_FONT_SMALLER */

#ifdef CTRL_FONT_DEFAULT_SIZE
    if CTRL_FONT_DEFAULT_SIZE {
      font1_size = DEFAULT_FONT_SIZE;
      goto font_size_changed;
    }
#endif /* CTRL_FONT_DEFAULT_SIZE */
#endif /* FONT_CHANGE_SIZE */

#if TAB
#ifdef CTRL_PREVIOUS_TAB
    if CTRL_PREVIOUS_TAB {
      int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (npages)
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages - 1);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
      return TRUE;
#endif /* CTRL_PREVIOUS_TAB */
    }
#ifdef CTRL_NEXT_TAB
    if CTRL_NEXT_TAB {
      int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (npages == (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1))
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages + 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
      return TRUE;
#endif /* CTRL_NEXT_TAB */
    }
#ifdef CTRL_NEW_TAB
    if CTRL_NEW_TAB {
#if DOUBLE_PRESS_HOTKEY
      if (last_time_1 != 0) {
        now_time_1 = current_time();
        if (now_time_1 - last_time_1 < DOUBLE_PRESS_TIME) {
#endif
          sakura_add_tab();
#if STATUS_BAR
          change_statusbar_encoding();
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
#endif /* CTRL_NEW_TAB */
    }
#ifdef CTRL_REMOVE_TAB
    if CTRL_REMOVE_TAB {
#if DOUBLE_PRESS_HOTKEY
      if (last_time_2 != 0) {
        now_time_2 = current_time();
        if (now_time_2 - last_time_2 < DOUBLE_PRESS_TIME) {
#endif
          sakura_del_tab();
#if STATUS_BAR
          change_statusbar_encoding();
#endif

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
#endif /* CTRL_REMOVE_TAB */
    }
#endif /* TAB */

  }
  return FALSE;

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
    gtk_window_resize(GTK_WINDOW(main_window), width_f, height_f);
    return TRUE;
#endif /* FONT_CHANGE_SIZE */
}
#endif /* ENABLE_KEY_PRESS */

#if SHOW_MENU
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

#if CLOSE_SAFE
void delete_event()
{
  while (1)
    sakura_del_tab();
}
#endif

void sakura_add_tab()
{
#ifdef TAB_LABEL
#if TAB_LABEL_NUMBER
  term.label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) + 1)));
#else
  term.label = gtk_label_new(TAB_LABEL);
#endif
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

#if CLOSE_SAFE
  term.pid = vte_terminal_fork_command(VTE_TERMINAL(term.vte), DEFAULT_COMMAND, DEFAULT_ARGV, DEFAULT_ENVV, DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);
#else
  vte_terminal_fork_command(VTE_TERMINAL(term.vte), DEFAULT_COMMAND, DEFAULT_ARGV, DEFAULT_ENVV, DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);
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

#ifdef TAB_LABEL
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.hbox, term.label);
#else
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.vte, term.label);
#endif
#else /* TAB_LABEL */
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.hbox, NULL);
#else
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), term.vte, NULL);
#endif
#endif /* TAB_LABEL */

  g_signal_connect(term.vte, "child-exited", sakura_del_tab, NULL);

#if SHOW_MENU
  g_signal_connect(term.vte, "button-press-event", G_CALLBACK(sakura_popup), NULL);
#endif

#if TAB
  g_array_append_val(terminals, term);
#endif

#if TAB_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) {
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_window_get_size(GTK_WINDOW(main_window), &width, &height);
  } else
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), TRUE);
  gtk_window_resize(GTK_WINDOW(main_window), width, height);
#endif

  gtk_widget_show_all(notebook);

#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
#endif

  gtk_window_set_focus(GTK_WINDOW(main_window), term.vte);
}

void sakura_del_tab()
{
  int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));

#if CLOSE_SAFE
#if TAB
  term = g_array_index(terminals, struct terminal, npages);
#endif
  /* Dirty hack. Prevent background applications close with tab */
  char pidstr[32];
  sprintf(pidstr, "kill -KILL %d", term.pid);
  system(pidstr);
#endif /* CLOSE_SAFE */

#if TAB
  g_array_remove_index(terminals, npages);
#endif

  gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), npages);
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < 1)
    gtk_main_quit();

#if TAB
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages);
#endif

#if TAB_AUTOHIDE
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) == 1) {
    gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
    gtk_window_resize(GTK_WINDOW(main_window), width, height);
  }
#endif

#if TAB_LABEL_NUMBER
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    term = g_array_index(terminals, struct terminal, i);
    term.label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (i + 1)));
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), term.hbox, term.label);
  }
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
#if SHOW_WINDOW_VERSION
  if (argc == 2 && !strcmp(argv[1], "-v")) {
    printf("%s, version %s\n", SHOW_WINDOW_TITLE, EVILVTE_VERSION);
    return 0;
  }
#endif

#ifdef DEFAULT_FONT
  sprintf(font1str, "%s %d", DEFAULT_FONT, font1_size);
#endif

  gtk_init(NULL, NULL);

#if TAB
  terminals = g_array_new(FALSE, TRUE, sizeof(struct terminal));
#endif

  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

#ifdef SHOW_WINDOW_TITLE
  gtk_window_set_title(GTK_WINDOW(main_window), SHOW_WINDOW_TITLE);
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

#ifdef TAB_BORDER_V
  gtk_notebook_set_tab_hborder(GTK_NOTEBOOK(notebook), TAB_BORDER_V);
#endif

#ifdef TAB_BORDER_H
  gtk_notebook_set_tab_vborder(GTK_NOTEBOOK(notebook), TAB_BORDER_H);
#endif

#if TAB_INITIAL_NUMBER
  for (i = 0 ; i < TAB_INITIAL_NUMBER ; i++)
    sakura_add_tab();
#else
  sakura_add_tab();
#endif

#if STATUS_BAR
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, 1, 0);
  statusbar = gtk_statusbar_new();
  change_statusbar_encoding();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, TRUE, 1, 0);
#else
  gtk_container_add(GTK_CONTAINER(main_window), notebook);
#endif

#if CLOSE_SAFE
  g_signal_connect(main_window, "delete_event", delete_event, NULL);
#endif

#if ENABLE_KEY_PRESS
  g_signal_connect(main_window, "key-press-event", G_CALLBACK(sakura_key_press), NULL);
#endif

#if !TAB
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
#endif

#if FONT_CHANGE_SIZE
  gtk_window_get_size(GTK_WINDOW(main_window), &width_f, &height_f);
#endif

#if SHOW_MENU
  menu = gtk_menu_new();
#endif

#ifdef ENCODING_LIST
  int encoding_size = sizeof(encoding) / sizeof(encoding[0]);
  GtkWidget *encoding_item[encoding_size];
  int i = 0;
  for (i = 0 ; i < encoding_size ; i++) {
#if ENCODING_LIST_DEFAULT
    if (encoding[i] == "Default") {
      encoding[i] = (char*)vte_terminal_get_encoding(VTE_TERMINAL(term.vte));
      encoding_item[i] = gtk_menu_item_new_with_label("Default");
    } else
#endif
      encoding_item[i] = gtk_menu_item_new_with_label(encoding[i]);
    gtk_menu_append(GTK_MENU(menu), encoding_item[i]);
    g_signal_connect(encoding_item[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
  }
#endif /* ENCODING_LIST */

#if SHOW_MENU
  gtk_widget_show_all(menu);
#endif

  gtk_widget_show_all(main_window);

#if MENU_INPUT_METHOD
  vte_terminal_im_append_menuitems(VTE_TERMINAL(term.vte), GTK_MENU_SHELL(menu));
#endif

  gtk_main();
  return 0;
}
