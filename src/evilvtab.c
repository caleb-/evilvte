/*
    This file is derived from termit 1.1.3, http://code.google.com/p/termit/
    Thanks to <ratnikov.ev@gmail.com>

    Original files of termit have no copyright information

    COPYING file of termit is GPL version 2
 */
#include <gdk/gdkkeysyms.h>
#include <vte/vte.h>

/* keep include config.h below other includes */
#include "config.h"

GArray *tabs;
GtkWidget *main_window;
GtkWidget *notebook;

#if MENU_POPUP
GtkWidget *menu;
#endif

#if STATUSBAR
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

struct Tab
{
  GtkWidget *vte_box;
#ifdef TAB_LABEL
  GtkWidget *label;
#endif
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  GtkWidget *hbox;
  GtkWidget *scrollbar;
#endif
} tab;

gboolean kbd_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data);
void add_tab();
void rm_tab();
void set_encoding(GtkWidget *widget, void *data);

#if MENU_POPUP
gboolean right_click_menu(GtkWidget *widget, GdkEvent *event);
#endif

#if STATUSBAR
void change_statusbar_encoding(int page);
void switch_page_routine(GtkNotebook *notebook, GtkNotebookPage *page, int page_num, gpointer user_data);
#endif

gboolean kbd_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  if (event->state & GDK_CONTROL_MASK) {
    if (event->keyval == GDK_Page_Up) {
      int current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (current_page)
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), current_page - 1);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1);
      return TRUE;
    }
    if (event->keyval == GDK_Page_Down) {
      int current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
      if (current_page == (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) - 1))
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
      else
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), current_page + 1);
      return TRUE;
    }
    if (event->keyval == GDK_t || event->keyval == GDK_T) {
      add_tab();
      return TRUE;
    }
    if (event->keyval == GDK_w || event->keyval == GDK_W) {
      rm_tab();
      return TRUE;
    }
  }
  return FALSE;
}

#if MENU_POPUP
gboolean right_click_menu(GtkWidget *widget, GdkEvent *event)
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

void add_tab()
{
#ifdef TAB_LABEL
#if TAB_LABEL_NUMBER
  tab.label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) + 1)));
#else
  tab.label = gtk_label_new(TAB_LABEL);
#endif
#endif /* TAB_LABEL */
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  tab.hbox = gtk_hbox_new(FALSE, 0);
#endif
  tab.vte_box = vte_terminal_new();
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  tab.scrollbar = gtk_vscrollbar_new(vte_terminal_get_adjustment(VTE_TERMINAL(tab.vte_box)));
#endif
#if SCROLLBAR_LEFT
  gtk_box_pack_start(GTK_BOX(tab.hbox), tab.scrollbar, FALSE, FALSE, 0);
#endif
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  gtk_box_pack_start(GTK_BOX(tab.hbox), tab.vte_box, TRUE, TRUE, 0);
#endif
#if SCROLLBAR_RIGHT && !SCROLLBAR_LEFT
  gtk_box_pack_start(GTK_BOX(tab.hbox), tab.scrollbar, FALSE, FALSE, 0);
#endif
  vte_terminal_fork_command(VTE_TERMINAL(tab.vte_box), DEFAULT_COMMAND, DEFAULT_ARGV, DEFAULT_ENVV, DEFAULT_DIRECTORY, ENABLE_LASTLOG, ENABLE_UTMP, ENABLE_WTMP);
#ifdef ALLOW_BOLD
  vte_terminal_set_allow_bold(VTE_TERMINAL(tab.vte_box), ALLOW_BOLD);
#endif
#ifdef BELL_AUDIBLE
  vte_terminal_set_audible_bell(VTE_TERMINAL(tab.vte_box), BELL_AUDIBLE);
#endif
#ifdef BELL_VISIBLE
  vte_terminal_set_visible_bell(VTE_TERMINAL(tab.vte_box), BELL_VISIBLE);
#endif
#ifdef DEFAULT_ENCODING
  vte_terminal_set_encoding(VTE_TERMINAL(tab.vte_box), DEFAULT_ENCODING);
#endif
#ifdef TERMINAL_FONT
#ifdef ANTI_ALIAS
  vte_terminal_set_font_from_string_full(VTE_TERMINAL(tab.vte_box), TERMINAL_FONT, ANTI_ALIAS);
#else
  vte_terminal_set_font_from_string(VTE_TERMINAL(tab.vte_box), TERMINAL_FONT);
#endif
#endif
#ifdef MOUSE_AUTOHIDE
  vte_terminal_set_mouse_autohide(VTE_TERMINAL(tab.vte_box), MOUSE_AUTOHIDE);
#endif
#ifdef SCROLL_BACKGROUND
  vte_terminal_set_scroll_background(VTE_TERMINAL(tab.vte_box), SCROLL_BACKGROUND);
#endif
#ifdef SCROLL_ON_KEYSTROKE
  vte_terminal_set_scroll_on_keystroke(VTE_TERMINAL(tab.vte_box), SCROLL_ON_KEYSTROKE);
#endif
#ifdef SCROLL_ON_OUTPUT
  vte_terminal_set_scroll_on_output(VTE_TERMINAL(tab.vte_box), SCROLL_ON_OUTPUT);
#endif
#if SCROLLBACK_LINES
  vte_terminal_set_scrollback_lines(VTE_TERMINAL(tab.vte_box), SCROLLBACK_LINES);
#endif
#if TERMINAL_COLS && TERMINAL_ROWS
  vte_terminal_set_size(VTE_TERMINAL(tab.vte_box), TERMINAL_COLS, TERMINAL_ROWS);
#endif
#ifdef WORD_CHARS
  vte_terminal_set_word_chars(VTE_TERMINAL(tab.vte_box), WORD_CHARS);
#endif
#ifdef TAB_LABEL
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab.hbox, tab.label);
#else
#if SCROLLBAR_LEFT || SCROLLBAR_RIGHT
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab.hbox, NULL);
#else
  int index = gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab.vte_box, NULL);
#endif
#endif /* TAB_LABEL */
  g_signal_connect(tab.vte_box, "child-exited", rm_tab, NULL);
#if MENU_POPUP
  g_signal_connect_swapped(tab.vte_box, "button-press-event", G_CALLBACK(right_click_menu), menu);
#endif
  g_array_append_val(tabs, tab);
  gtk_widget_show_all(notebook);
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
  gtk_window_set_focus(GTK_WINDOW(main_window), tab.vte_box);
}

void rm_tab()
{
  int current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
  g_array_remove_index(tabs, current_page);
  gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), current_page);
  if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) < 1)
    gtk_main_quit();
  gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), current_page);
#ifdef TAB_LABEL
#if TAB_LABEL_NUMBER
  int i;
  for (i = 0 ; i < gtk_notebook_get_n_pages(GTK_NOTEBOOK(notebook)) ; i++) {
    tab = g_array_index(tabs, struct Tab, i);
    tab.label = gtk_label_new(g_strdup_printf("%s %d", TAB_LABEL, (i + 1)));
    gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook), tab.hbox, tab.label);
  }
#endif
#endif /* TAB_LABEL */
}

void set_encoding(GtkWidget *widget, void *data)
{
  tab = g_array_index(tabs, struct Tab, gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook)));
  vte_terminal_set_encoding(VTE_TERMINAL(tab.vte_box), (char*)data);
#if STATUSBAR
  change_statusbar_encoding(-1);
#endif
}

#if STATUSBAR
void change_statusbar_encoding(int page)
{
    if (page < 0)
        page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    tab = g_array_index(tabs, struct Tab, page);
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, vte_terminal_get_encoding(VTE_TERMINAL(tab.vte_box)));
}
#endif

#if STATUSBAR
void switch_page_routine(GtkNotebook *notebook, GtkNotebookPage *page, int page_num, gpointer user_data)
{
  change_statusbar_encoding(page_num);
}
#endif

int main()
{
  gtk_init(NULL, NULL);
  tabs = g_array_new(FALSE, TRUE, sizeof(struct Tab));
#if MENU_POPUP
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
#if STATUSBAR
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(main_window), vbox);
#endif
  notebook = gtk_notebook_new();
#if INITIAL_TAB_NUMBER
  for (i = 0 ; i < INITIAL_TAB_NUMBER ; i++)
    add_tab();
#else
  add_tab();
#endif
#if STATUSBAR
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, 1, 0);
  statusbar = gtk_statusbar_new();
  change_statusbar_encoding(0);
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, TRUE, 1, 0);
  g_signal_connect(notebook, "switch-page", G_CALLBACK(switch_page_routine), NULL);
#else
  gtk_container_add(GTK_CONTAINER(main_window), notebook);
#endif
  g_signal_connect(main_window, "delete_event", gtk_main_quit, NULL);
  g_signal_connect(main_window, "key-press-event", G_CALLBACK(kbd_event), NULL);
  gtk_widget_show_all(main_window);
  gtk_main();
  return 0;
}
