/* comment out options to reduce binary size and RAM consumption */

// #define ALLOW_BOLD          TRUE
// #define BELL_AUDIBLE        TRUE
// #define BELL_VISIBLE        TRUE
// #define COLOR_STYLE_LINUX   TRUE
// #define COLOR_STYLE_RXVT    TRUE
// #define COLOR_STYLE_TANGO   TRUE
// #define COLOR_STYLE_XTERM   TRUE
// #define COLOR_BACKGROUND    "white" /* "#FFFFFF" */
// #define COLOR_FOREGROUND    "black"
// #define DEFAULT_COLUMNS     80
// #define DEFAULT_ROWS        24
// #define DEFAULT_ENCODING    "UTF-8"
// #define DEFAULT_FONT        "Monospace 10"
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_USE_DEFAULT
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_FORCE_ENABLE
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_FORCE_DISABLE
// #define MOUSE_AUTOHIDE      TRUE
// #define SCROLL_BACKGROUND   TRUE
// #define SCROLL_LINES        60
// #define SCROLL_ON_KEYSTROKE TRUE
// #define SCROLL_ON_OUTPUT    TRUE
// #define SCROLLBAR_LEFT      TRUE
#define SCROLLBAR_RIGHT        TRUE
// #define SHOW_WINDOW_BORDER  TRUE
// #define SHOW_WINDOW_ICON    TRUE
// #define SHOW_WINDOW_TITLE   "evilvte" /* Change window name as you like */
#define STATUS_BAR             TRUE
#define SWITCH_ENCODING        TRUE /* Right click to switch encoding */
#define TAB                    TRUE /* Yon can disable TAB if you want */
// #define TAB_AUTOHIDE        TRUE
// #define TAB_BORDER          0
// #define TAB_BORDER_H        0
// #define TAB_BORDER_V        0
// #define TAB_INITIAL_NUMBER  1
// #define TAB_LABEL           "Page"
// #define TAB_LABEL_NUMBER    TRUE /* This option depends on TAB_LABEL */
// #define WORD_CHARS          "-A-Za-z0-9_$.+!*(),;:@&=?/~#%"

/* Some applications use Ctrl + T/W as hotkey.
 * Enable DOUBLE_PRESS_HOTKEY makes them still useful.
 * Press Ctrl + T/W twice quickly to add or remove tabs.
 */

// #define DOUBLE_PRESS_HOTKEY TRUE
// #define DOUBLE_PRESS_TIME   300000 /* 0.3 second = 300000 microseconds */

/* Do not comment out options below */

#define ENABLE_LASTLOG         TRUE
#define ENABLE_UTMP            TRUE
#define ENABLE_WTMP            TRUE
#ifdef SWITCH_ENCODING
static char *encoding[] = {"UTF-8"}; /* Add encoding options as you like */
// static char *encoding[] = {"UTF-8", "BIG-5", "GBK", "EUC-JP", "SHIFT-JIS"};
#endif

/* Hotkey definitions. Please see /usr/include/gtk-2.0/gdk/gdkkeysyms.h */
/* GDK_A = "Shift + a", so hotkey will be "Ctrl + Shift + a" */

#define CTRL_PREVIOUS_TAB (event->keyval == GDK_Page_Up) /* Ctrl + Page Up */
#define CTRL_NEXT_TAB     (event->keyval == GDK_Page_Down)
#define CTRL_NEW_TAB      (event->keyval == GDK_t || event->keyval == GDK_T)
#define CTRL_REMOVE_TAB   (event->keyval == GDK_w || event->keyval == GDK_W)

/* Do not edit options below unless you know what you are doing */

#define DEFAULT_COMMAND        g_getenv("SHELL")
#define DEFAULT_ARGV           NULL
#define DEFAULT_ENVV           NULL
#define DEFAULT_DIRECTORY      g_getenv("PWD")

/* uncomment those options to reduce binary size */

/*
#undef GTK_BOX
#undef GTK_CONTAINER
#undef GTK_NOTEBOOK
#undef GTK_WINDOW
#undef VTE_TERMINAL
#define GTK_BOX
#define GTK_CONTAINER
#define GTK_NOTEBOOK
#define GTK_WINDOW
#define VTE_TERMINAL
*/

/* G_CALLBACK GTK_MENU GTK_STATUSBAR */
