#include <ctype.h>
#include <dlfcn.h>
#include <glib.h>
#ifndef G_CONST_RETURN
#define G_CONST_RETURN const
#endif
#include <gtk/gtk.h>
#if GTK_CHECK_VERSION(2,90,7)
#include <gdk/gdkkeysyms-compat.h>
#endif
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <libintl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <vte/vte.h>
#include <vte/vteaccess.h>

#ifndef VTE_CHECK_VERSION
#define VTE_CHECK_VERSION(x,y,z) FALSE
#endif

#ifndef VTE_ERASE_TTY
#define VTE_ERASE_TTY VTE_ERASE_AUTO
#endif

#define AUTO            VTE_ERASE_AUTO
#define BACKSPACE       VTE_ERASE_ASCII_BACKSPACE
#define DELETE          VTE_ERASE_ASCII_DELETE
#define DELETE_SEQUENCE VTE_ERASE_DELETE_SEQUENCE
#define ERASE_TTY       VTE_ERASE_TTY

#define BLOCK     VTE_CURSOR_SHAPE_BLOCK
#define IBEAM     VTE_CURSOR_SHAPE_IBEAM
#define UNDERLINE VTE_CURSOR_SHAPE_UNDERLINE

#define LINUX            1
#define RXVT             2
#define TANGO            3
#define VTE_FIXED        4
#define XTERM            5
#define ZENBURN_DARK     6
#define ZENBURN          7
#define SOLARIZED_DARK   8
#define SOLARIZED_LIGHT  9
#define USER_CUSTOM     10
#define HOLO            11

#define LEFT   0
#define RIGHT  1
#define TOP    2
#define BOTTOM 3
#define OFF_L  4
#define OFF_R  5

#define ALT(x) (((event->state & GDK_MOD1_MASK) == GDK_MOD1_MASK) && (event->keyval == (x)))
#define ALT_SHIFT(x) (((event->state & (GDK_MOD1_MASK | GDK_SHIFT_MASK)) == (GDK_MOD1_MASK | GDK_SHIFT_MASK)) && (event->keyval == (x)))
#define CTRL(x) (((event->state & GDK_CONTROL_MASK) == GDK_CONTROL_MASK) && (event->keyval == (x)))
#define CTRL_ALT(x) (((event->state & (GDK_CONTROL_MASK | GDK_MOD1_MASK)) == (GDK_CONTROL_MASK | GDK_MOD1_MASK)) && (event->keyval == (x)))
#define CTRL_ALT_SHIFT(x) (((event->state & (GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_MOD1_MASK | GDK_SHIFT_MASK)) && (event->keyval == (x)))
#define CTRL_SHIFT(x) (((event->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) && (event->keyval == (x)))
#define SHIFT(x) (((event->state & GDK_SHIFT_MASK) == GDK_SHIFT_MASK) && (event->keyval == (x)))

#include "custom.h"
#include "evilvte.h"
