/* Forked from gcin 1.4.5.pre2, http://cle.linux.org.tw/gcin
 * Copyright (C) 2008-2009  Edward Der-Hua Liu, Hsin-Chu, Taiwan
 * Copyright (C) 2009  Wen-Yen Chuang <caleb AT calno DOT com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <X11/keysym.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#if !EVILVTE_MINIMUM && !EVILVTE_MAXIMUM
#include "config.h"
#include "evilvte.h"
#endif

#if EVILVTE_MAXIMUM
#include "maximum.h"
#include "max.h"
#endif

#if EVILVTE_TESTING
#include "testing.h"
#include "test.h"
#endif

#ifndef VIRTUAL_KEYBOARD_MENU_KEY
#define VIRTUAL_KEYBOARD_MENU_KEY "Menu"
#endif

#ifndef VIRTUAL_KEYBOARD_WINDOW_KEY
#define VIRTUAL_KEYBOARD_WINDOW_KEY "Win"
#endif

#if VIRTUAL_KEYBOARD && XTST_DETECTED

extern GtkWidget *main_window;

#define K_FILL   1
#define K_HOLD   2
#define K_PRESS  4
#define K_AREA_R 8

#define MODIFY_FOREGROUND(lab,col) gtk_widget_modify_fg(lab, GTK_STATE_NORMAL, col); gtk_widget_modify_fg(lab, GTK_STATE_PRELIGHT, col);

static Display *dpy;
static const GdkColor red = { 0, 0xFFFF, 0x0000, 0x0000 };
static GtkRequisition kbm_size;
static GtkWidget *gwin_kbm;

typedef struct {
  const KeySym keysym;
  const char *enkey;
  char flag;
  GtkWidget *laben;
} KEY;

#include "layout_en.h"

static const int keysN = sizeof(keys) / sizeof(keys[0]);

static void kbm_click(GtkWidget *widget, KEY *k)
{
  const KeyCode kc = XKeysymToKeycode(dpy, k->keysym);

  if (k->flag & K_HOLD) {
    if (k->flag & K_PRESS) {
      XTestFakeKeyEvent(dpy, kc, 0, CurrentTime);
      k->flag &= ~K_PRESS;
      MODIFY_FOREGROUND(k->laben, NULL);
    } else {
      XTestFakeKeyEvent(dpy, kc, 1, CurrentTime);
      k->flag |= K_PRESS;
      MODIFY_FOREGROUND(k->laben, &red);
    }
  } else {
    XTestFakeKeyEvent(dpy, kc, 1, CurrentTime);
    XTestFakeKeyEvent(dpy, kc, 0, CurrentTime);
    int i;
    for(i = 0; i < keysN; i++) {
      int j;
      for(j = 0; keys[i][j].enkey; j++) {
        if (!(keys[i][j].flag & K_PRESS))
          continue;
        XTestFakeKeyEvent(dpy, XKeysymToKeycode(dpy, keys[i][j].keysym), 0, CurrentTime);
        keys[i][j].flag &= ~K_PRESS;
        MODIFY_FOREGROUND(keys[i][j].laben, NULL);
      }
    }
  }
}

void hide_kbm()
{
  gtk_widget_hide_all(gwin_kbm);
}

void move_kbm()
{
  GtkRequisition vte_size;
  gtk_widget_size_request(main_window, &vte_size);
  int win_x, win_y, final_x, final_y;
  gtk_window_get_position(GTK_WINDOW(main_window), &win_x, &win_y);
  int scr_x = gdk_screen_width() - kbm_size.width;
  int scr_y = gdk_screen_height() - kbm_size.height;
  final_x = (win_x > scr_x) ? scr_x : win_x;
  final_y = ((win_y + vte_size.height) > scr_y) ? scr_y : win_y + vte_size.height;
  gtk_widget_hide_all(gwin_kbm);
  gtk_widget_show_all(gwin_kbm);
  gtk_window_move(GTK_WINDOW(gwin_kbm), (final_x < 0) ? 0 : final_x, (final_y < 0) ? 0 : final_y);
}

void make_kbm()
{
  dpy = GDK_DISPLAY();
#if 0
  gdk_color_parse("red", &red);
#endif

  gwin_kbm = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  GtkWidget *hbox_top = gtk_hbox_new(0, 0);
  gtk_container_add(GTK_CONTAINER(gwin_kbm), hbox_top);

  GtkWidget *vbox_l = gtk_vbox_new(0, 0);
  gtk_box_pack_start(GTK_BOX(hbox_top), vbox_l, 0, 0, 0);
  GtkWidget *vbox_r = gtk_vbox_new(0, 0);
  gtk_box_pack_start(GTK_BOX(hbox_top), vbox_r, 0, 0, 0);

  int i;
  for(i = 0; i < keysN; i++) {
    GtkWidget *hboxl = gtk_hbox_new(0, 0);
    gtk_box_pack_start(GTK_BOX(vbox_l), hboxl, 0, 0, 0);
    GtkWidget *hboxr = gtk_hbox_new(0, 0);
    gtk_box_pack_start(GTK_BOX(vbox_r), hboxr, 0, 0, 0);
    KEY *pk = keys[i];

    int j;
    for(j = 0; pk[j].enkey; j++) {
      KEY *ppk = &pk[j];
      GtkWidget *but = gtk_button_new();
      g_signal_connect(but, "clicked", G_CALLBACK(kbm_click), ppk);

      if (ppk->flag & K_FILL) {
        if (ppk->flag & K_AREA_R)
          gtk_box_pack_start(GTK_BOX(hboxr), but, 1, 1, 0);
        else
          gtk_box_pack_start(GTK_BOX(hboxl), but, 1, 1, 0);
      } else {
        if (ppk->flag & K_AREA_R)
          gtk_box_pack_start(GTK_BOX(hboxr), but, 0, 0, 0);
        else
          gtk_box_pack_start(GTK_BOX(hboxl), but, 0, 0, 0);
      }

      ppk->laben = gtk_label_new(ppk->enkey);
      gtk_container_add(GTK_CONTAINER(but), ppk->laben);
    }
  }

  gtk_widget_realize(gwin_kbm);
  gdk_window_set_override_redirect(gwin_kbm->window, 1);
  gtk_widget_show_all(gwin_kbm);
  gtk_widget_size_request(gwin_kbm, &kbm_size);
}
#endif /* VIRTUAL_KEYBOARD && XTST_DETECTED */
