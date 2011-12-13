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

#define K_FILL 1
#define K_HOLD 2
#define K_PRESS 4
#define K_AREA_R 8

#define MODIFY_FOREGROUND(lab,col) \
  gtk_widget_modify_fg(lab, GTK_STATE_NORMAL, col); \
  gtk_widget_modify_fg(lab, GTK_STATE_PRELIGHT, col);

static Display *dpy;
static GdkColor red;
static GtkRequisition kbm_size;
static GtkWidget *gwin_kbm;

typedef struct {
  KeySym keysym;
  char *enkey;
  char flag;
  GtkWidget *laben;
} KEY;

static KEY keys[][15] = {
{{XK_Escape,"Esc",K_FILL},{XK_F1,"F1"},{XK_F2,"F2"},{XK_F3,"F3"},{XK_F4,"F4"},{XK_F5,"F5"},{XK_F6,"F6"},{XK_F7,"F7"},{XK_F8,"F8"},{XK_F9,"F9"},{XK_F10,"F10"},{XK_F11,"F11"},{XK_F12,"F12"}},
{{'`',"`",K_FILL},{'1',"1",K_FILL},{'2',"2",K_FILL},{'3',"3",K_FILL},{'4',"4",K_FILL},{'5',"5",K_FILL},{'6',"6",K_FILL},{'7',"7",K_FILL},{'8',"8",K_FILL},{'9',"9",K_FILL},{'0',"0",K_FILL},{'-',"-",K_FILL},{'=',"=",K_FILL},{XK_BackSpace,"←",K_FILL}},
{{XK_Tab,"Tab"},{'q',"q",K_FILL},{'w',"w",K_FILL},{'e',"e",K_FILL},{'r',"r",K_FILL},{'t',"t",K_FILL},{'y',"y",K_FILL},{'u',"u",K_FILL},{'i',"i",K_FILL},{'o',"o",K_FILL},{'p',"p",K_FILL},{'[',"[",K_FILL},{']',"]",K_FILL},{'\\',"\\",K_FILL}},
{{XK_Caps_Lock,"Caps"},{'a',"a",K_FILL},{'s',"s",K_FILL},{'d',"d",K_FILL},{'f',"f",K_FILL},{'g',"g",K_FILL},{'h',"h",K_FILL},{'j',"j",K_FILL},{'k',"k",K_FILL},{'l',"l",K_FILL},{';',";",K_FILL},{'\'',"'",K_FILL},{XK_Return,"Enter",K_FILL}},
{{XK_Shift_L,"Shift",K_HOLD|K_FILL},{'z',"z",K_FILL},{'x',"x",K_FILL},{'c',"c",K_FILL},{'v',"v",K_FILL},{'b',"b",K_FILL},{'n',"n",K_FILL},{'m',"m",K_FILL},{',',",",K_FILL},{'.',".",K_FILL},{'/',"/",K_FILL},{XK_Shift_R,"Shift",K_HOLD|K_FILL}},
{{XK_Control_L,"Ctrl",K_HOLD},{XK_Meta_L,VIRTUAL_KEYBOARD_WINDOW_KEY,K_HOLD},{XK_Alt_L,"Alt",K_HOLD},{' ',"",K_FILL},{XK_Alt_R,"Alt",K_HOLD},{XK_Meta_R,VIRTUAL_KEYBOARD_WINDOW_KEY,K_HOLD},{XK_Menu,VIRTUAL_KEYBOARD_MENU_KEY},{XK_Control_R,"Ctrl",K_HOLD}},
{{XK_Print,"PrtSc",K_AREA_R|K_FILL},{XK_Scroll_Lock,"ScrLk",K_AREA_R|K_FILL},{XK_Pause,"Pause",K_AREA_R|K_FILL}},
{{XK_Insert,"Ins",K_AREA_R|K_FILL},{XK_Home,"Home",K_AREA_R|K_FILL},{XK_Prior,"PgUp",K_AREA_R|K_FILL}},
{{XK_Delete,"Del",K_AREA_R|K_FILL},{XK_End,"End",K_AREA_R|K_FILL},{XK_Next,"PgDn",K_AREA_R|K_FILL}},
{{XK_Num_Lock,"Num",K_AREA_R|K_FILL},{XK_KP_Divide,"/",K_AREA_R|K_FILL},{XK_KP_Multiply,"*",K_AREA_R|K_FILL},{XK_KP_Subtract,"-",K_AREA_R|K_FILL},{XK_KP_Add,"+",K_AREA_R|K_FILL}},
{{XK_Up,"⇧",K_AREA_R|K_FILL}},
{{XK_Left,"⇦",K_AREA_R|K_FILL},{XK_Down,"⇩",K_AREA_R|K_FILL},{XK_Right,"⇨",K_AREA_R|K_FILL}}
};

static int keysN = sizeof(keys) / sizeof(keys[0]);

static void kbm_click(GtkWidget *widget, KEY *k)
{
  KeyCode kc = XKeysymToKeycode(dpy, k->keysym);

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
  gdk_color_parse("red", &red);

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
