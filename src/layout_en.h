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

static KEY keys[][15] =
{
  {
    {XK_Escape, "Esc", K_FILL},
    {XK_F1,     "F1"         },
    {XK_F2,     "F2"         },
    {XK_F3,     "F3"         },
    {XK_F4,     "F4"         },
    {XK_F5,     "F5"         },
    {XK_F6,     "F6"         },
    {XK_F7,     "F7"         },
    {XK_F8,     "F8"         },
    {XK_F9,     "F9"         },
    {XK_F10,    "F10"        },
    {XK_F11,    "F11"        },
    {XK_F12,    "F12"        }
  },

  {
    {'`',          "`", K_FILL},
    {'1',          "1", K_FILL},
    {'2',          "2", K_FILL},
    {'3',          "3", K_FILL},
    {'4',          "4", K_FILL},
    {'5',          "5", K_FILL},
    {'6',          "6", K_FILL},
    {'7',          "7", K_FILL},
    {'8',          "8", K_FILL},
    {'9',          "9", K_FILL},
    {'0',          "0", K_FILL},
    {'-',          "-", K_FILL},
    {'=',          "=", K_FILL},
    {XK_BackSpace, "←", K_FILL}
  },

  {
    {XK_Tab, "Tab"       },
    {'q',    "q",  K_FILL},
    {'w',    "w",  K_FILL},
    {'e',    "e",  K_FILL},
    {'r',    "r",  K_FILL},
    {'t',    "t",  K_FILL},
    {'y',    "y",  K_FILL},
    {'u',    "u",  K_FILL},
    {'i',    "i",  K_FILL},
    {'o',    "o",  K_FILL},
    {'p',    "p",  K_FILL},
    {'[',    "[",  K_FILL},
    {']',    "]",  K_FILL},
    {'\\',   "\\", K_FILL}
  },

  {
    {XK_Caps_Lock, "Caps"         },
    {'a',          "a",     K_FILL},
    {'s',          "s",     K_FILL},
    {'d',          "d",     K_FILL},
    {'f',          "f",     K_FILL},
    {'g',          "g",     K_FILL},
    {'h',          "h",     K_FILL},
    {'j',          "j",     K_FILL},
    {'k',          "k",     K_FILL},
    {'l',          "l",     K_FILL},
    {';',          ";",     K_FILL},
    {'\'',         "'",     K_FILL},
    {XK_Return,    "Enter", K_FILL}
  },

  {
    {XK_Shift_L, "Shift", K_FILL|K_HOLD},
    {'z',        "z",     K_FILL       },
    {'x',        "x",     K_FILL       },
    {'c',        "c",     K_FILL       },
    {'v',        "v",     K_FILL       },
    {'b',        "b",     K_FILL       },
    {'n',        "n",     K_FILL       },
    {'m',        "m",     K_FILL       },
    {',',        ",",     K_FILL       },
    {'.',        ".",     K_FILL       },
    {'/',        "/",     K_FILL       },
    {XK_Shift_R, "Shift", K_FILL|K_HOLD}
  },

  {
    {XK_Control_L, "Ctrl",                      K_HOLD},
    {XK_Meta_L,    VIRTUAL_KEYBOARD_WINDOW_KEY, K_HOLD},
    {XK_Alt_L,     "Alt",                       K_HOLD},
    {' ',          "",                          K_FILL},
    {XK_Alt_R,     "Alt",                       K_HOLD},
    {XK_Meta_R,    VIRTUAL_KEYBOARD_WINDOW_KEY, K_HOLD},
    {XK_Menu,      VIRTUAL_KEYBOARD_MENU_KEY          },
    {XK_Control_R, "Ctrl",                      K_HOLD}
  },

  {
    {XK_Print,       "PrtSc", K_AREA_R|K_FILL},
    {XK_Scroll_Lock, "ScrLk", K_AREA_R|K_FILL},
    {XK_Pause,       "Pause", K_AREA_R|K_FILL}
  },

  {
    {XK_Insert, "Ins",  K_AREA_R|K_FILL},
    {XK_Home,   "Home", K_AREA_R|K_FILL},
    {XK_Prior,  "PgUp", K_AREA_R|K_FILL}
  },

  {
    {XK_Delete, "Del",  K_AREA_R|K_FILL},
    {XK_End,    "End",  K_AREA_R|K_FILL},
    {XK_Next,   "PgDn", K_AREA_R|K_FILL}
  },

  {
    {XK_Num_Lock,    "Num", K_AREA_R|K_FILL},
    {XK_KP_Divide,   "/",   K_AREA_R|K_FILL},
    {XK_KP_Multiply, "*",   K_AREA_R|K_FILL},
    {XK_KP_Subtract, "-",   K_AREA_R|K_FILL},
    {XK_KP_Add,      "+",   K_AREA_R|K_FILL}
  },

  {
    {XK_Up, "⇧", K_AREA_R|K_FILL}
  },

  {
    {XK_Left,  "⇦", K_AREA_R|K_FILL},
    {XK_Down,  "⇩", K_AREA_R|K_FILL},
    {XK_Right, "⇨", K_AREA_R|K_FILL}
  }
};
