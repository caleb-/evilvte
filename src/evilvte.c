/*
    Copyright (C) 2008 Wen-Yen Chuang <caleb@calno.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
    This file is written from scratch by Wen-Yen Chuang <caleb@calno.com>
 */

#include <vte/vte.h>

/* keep include config.h below other includes */
#include "config.h"

GtkWidget *main_window;
GtkWidget *vte_box;

int main()
{
  gtk_init(NULL, NULL);
  main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  vte_box = vte_terminal_new();
  gtk_container_add(GTK_CONTAINER(main_window), vte_box);
  g_signal_connect(main_window, "delete_event", gtk_main_quit, NULL);
  g_signal_connect(vte_box, "child_exited", gtk_main_quit, NULL);
  vte_terminal_fork_command(VTE_TERMINAL(vte_box), DEFAULT_COMMAND, DEFAULT_ARGV, DEFAULT_ENVV, DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);

#ifdef ALLOW_BOLD
  vte_terminal_set_allow_bold(VTE_TERMINAL(vte_box), ALLOW_BOLD);
#endif

#ifdef BELL_AUDIBLE
  vte_terminal_set_audible_bell(VTE_TERMINAL(vte_box), BELL_AUDIBLE);
#endif

#ifdef BELL_VISIBLE
  vte_terminal_set_visible_bell(VTE_TERMINAL(vte_box), BELL_VISIBLE);
#endif

#ifdef DEFAULT_ENCODING
  vte_terminal_set_encoding(VTE_TERMINAL(vte_box), DEFAULT_ENCODING);
#endif

#ifdef TERMINAL_FONT
#ifdef ANTI_ALIAS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(vte_box), TERMINAL_FONT, ANTI_ALIAS);
#else
  vte_terminal_set_font_from_string(VTE_TERMINAL(vte_box), TERMINAL_FONT);
#endif
#endif

#ifdef MOUSE_AUTOHIDE
  vte_terminal_set_mouse_autohide(VTE_TERMINAL(vte_box), MOUSE_AUTOHIDE);
#endif

#ifdef SCROLL_BACKGROUND
  vte_terminal_set_scroll_background(VTE_TERMINAL(vte_box), SCROLL_BACKGROUND);
#endif

#ifdef SCROLL_ON_KEYSTROKE
  vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(vte_box), SCROLL_ON_KEYSTROKE);
#endif

#ifdef SCROLL_ON_OUTPUT
  vte_terminal_set_scroll_on_output(VTE_TERMINAL(vte_box), SCROLL_ON_OUTPUT);
#endif

#ifdef SCROLLBACK_LINES
  vte_terminal_set_scrollback_lines(VTE_TERMINAL(vte_box), SCROLLBACK_LINES);
#endif

#if TERMINAL_COLS && TERMINAL_ROWS
  vte_terminal_set_size(VTE_TERMINAL(vte_box), TERMINAL_COLS, TERMINAL_ROWS);
#endif

#ifdef WORD_CHARS
  vte_terminal_set_word_chars(VTE_TERMINAL(vte_box), WORD_CHARS);
#endif

  gtk_widget_show_all(main_window);
  gtk_main();
  return 0;
}
