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

#if SCROLLBAR_LEFT
#undef SCROLLBAR_RIGHT
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

GtkWidget *main_window;
GtkWidget *notebook;

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

#if TAB
GArray *terminals;
#endif

#if TAB_AUTOHIDE
int width;
int height;
#endif

#if SWITCH_ENCODING
GtkWidget *menu;
#endif

#if STATUS_BAR
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

#if DOUBLE_PRESS_HOTKEY
gint64 now_time_1;
gint64 last_time_1 = 0;
gint64 now_time_2;
gint64 last_time_2 = 0;
#endif

struct terminal {
  GtkWidget *vte;
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
void sakura_add_tab();
void sakura_del_tab();
void set_encoding(GtkWidget *widget, void *data);

#if TAB
gboolean sakura_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  if (event->state & GDK_CONTROL_MASK) {
    if (event->keyval == GDK_Page_Up) {
      int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (npages)
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages - 1);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
      return TRUE;
    }
    if (event->keyval == GDK_Page_Down) {
      int npages = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (npages == (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1))
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), npages + 1);
#if STATUS_BAR
      change_statusbar_encoding();
#endif
      return TRUE;
    }
    if (event->keyval == GDK_t || event->keyval == GDK_T) {
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
    }
    if (event->keyval == GDK_w || event->keyval == GDK_W) {
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
    }
  }
  return FALSE;
}
#endif /* TAB */

#if SWITCH_ENCODING
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

  vte_terminal_fork_command(VTE_TERMINAL(term.vte), DEFAULT_COMMAND, DEFAULT_ARGV, DEFAULT_ENVV, DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);

#ifdef ALLOW_BOLD
  vte_terminal_set_allow_bold(VTE_TERMINAL(term.vte), ALLOW_BOLD);
#endif

#ifdef BELL_AUDIBLE
  vte_terminal_set_audible_bell(VTE_TERMINAL(term.vte), BELL_AUDIBLE);
#endif

#ifdef BELL_VISIBLE
  vte_terminal_set_visible_bell(VTE_TERMINAL(term.vte), BELL_VISIBLE);
#endif

#if COLOR_STYLE_TANGO
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_tango, 16);
#endif

#if COLOR_STYLE_LINUX
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_linux, 16);
#endif

#if COLOR_STYLE_XTERM
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_xterm, 16);
#endif

#if COLOR_STYLE_RXVT
  vte_terminal_set_colors(VTE_TERMINAL(term.vte), NULL, NULL, color_rxvt, 16);
#endif

#ifdef DEFAULT_ENCODING
  vte_terminal_set_encoding(VTE_TERMINAL(term.vte), DEFAULT_ENCODING);
#endif

#ifdef DEFAULT_FONT
#ifdef ANTI_ALIAS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(term.vte), DEFAULT_FONT, ANTI_ALIAS);
#else
  vte_terminal_set_font_from_string(VTE_TERMINAL(term.vte), DEFAULT_FONT);
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

#if SWITCH_ENCODING
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

int main()
{
  gtk_init(NULL, NULL);

#if TAB
  terminals = g_array_new(FALSE, TRUE, sizeof(struct terminal));
#endif

#if SWITCH_ENCODING
  menu = gtk_menu_new();
  int encoding_size = sizeof(encoding) / sizeof(encoding[0]);
  GtkWidget *encoding_item[encoding_size];
  int i = 0;
  for (i = 0 ; i < encoding_size ; i++) {
    encoding_item[i] = gtk_menu_item_new_with_label(encoding[i]);
    gtk_menu_append(GTK_MENU(menu), encoding_item[i]);
    g_signal_connect(encoding_item[i], "activate", G_CALLBACK(set_encoding), encoding[i]);
  }
  gtk_widget_show_all(menu);
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

#if TAB
  g_signal_connect(main_window, "key-press-event", G_CALLBACK(sakura_key_press), NULL);
#endif

#if !TAB
  gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
#endif

  gtk_widget_show_all(main_window);
  gtk_main();
  return 0;
}
