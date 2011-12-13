/* comment out options to reduce binary size and RAM consumption */

// #define ALLOW_BOLD          TRUE
// #define BACKGROUND_IMAGE    ".config/evilvte/background.png"
// #define BACKGROUND_TINT     "brown"
// #define BACKGROUND_SATURATE 0.5 /* Please assign it from 0 to 1 */
// #define BELL_AUDIBLE        TRUE
// #define BELL_VISIBLE        TRUE
// #define CLOSE_SAFE          TRUE /* Do not kill background applications */
// #define COLOR_STYLE_LINUX   TRUE
// #define COLOR_STYLE_RXVT    TRUE
// #define COLOR_STYLE_TANGO   TRUE
// #define COLOR_STYLE_XTERM   TRUE
// #define COLOR_BACKGROUND    "white" /* "#FFFFFF" */
// #define COLOR_BOLD          "red"
// #define COLOR_CURSOR        "blue"
// #define COLOR_DIM           "yellow"
// #define COLOR_FOREGROUND    "black"
// #define COLOR_HIGHLIGHT     "green"
// #define CURSOR_BLINKS       TRUE
// #define DEFAULT_COLUMNS     80
// #define DEFAULT_ROWS        24
// #define DEFAULT_ENCODING    "UTF-8"
// #define DEFAULT_FONT_1      "Monospace" /* Supports FONT_CHANGE_SIZE */
// #define DEFAULT_FONT_1_SIZE 10
// #define DEFAULT_FONT_SIMPLE "Monospace 10" /* Can not change font size */
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_USE_DEFAULT
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_FORCE_ENABLE
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_FORCE_DISABLE
// #define FONT_CHANGE_SIZE    TRUE /* Change font size with hotkey */
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
// #define SHOW_WINDOW_VERSION TRUE /* Command line -v option */
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

/* Do not comment out options below */

#define ENABLE_LASTLOG         TRUE
#define ENABLE_UTMP            TRUE
#define ENABLE_WTMP            TRUE
#ifdef SWITCH_ENCODING
// #define MENU_PARSE_DEFAULT  TRUE /* You can define "Default" below */
char *encoding[] = {           "UTF-8"           };
// char *encoding[] = {"BIG-5", "Default", "GBK", "EUC-JP", "SHIFT-JIS"};
#endif

/* Hotkey definitions. Please see /usr/include/gtk-2.0/gdk/gdkkeysyms.h
 * GDK_A = "Shift + a", so hotkey will be "Ctrl + Shift + a"
 *
 * You can use || to define multiple keys
 * Example: (event->keyval == GDK_a || event->keyval == GDK_A)
 */

#define CTRL_PREVIOUS_TAB      (event->keyval == GDK_Page_Up)
#define CTRL_NEXT_TAB          (event->keyval == GDK_Page_Down)
#define CTRL_NEW_TAB           (event->keyval == GDK_T) /* Ctrl + Shift + t */
#define CTRL_REMOVE_TAB        (event->keyval == GDK_W) /* Ctrl + Shift + w */
#define CTRL_FONT_BIGGER       (event->keyval == GDK_KP_Add)
#define CTRL_FONT_SMALLER      (event->keyval == GDK_KP_Subtract)
// #define CTRL_FONT_DEFAULT   (event->keyval == GDK_KP_Insert)

/* If you enable DOUBLE_PRESS_HOTKEY,
 * press CTRL_NEW_TAB or CTRL_REMOVE_TAB twice quickly to add or remove a tab
 */

// #define DOUBLE_PRESS_HOTKEY TRUE
// #define DOUBLE_PRESS_TIME   300000 /* 0.3 second = 300000 microseconds */

/* Do not edit options below unless you know what you are doing */

#define DEFAULT_COMMAND        g_getenv("SHELL")
#define DEFAULT_ARGV           NULL
#define DEFAULT_ENVV           NULL
#define DEFAULT_DIRECTORY      g_getenv("PWD")

/* uncomment options below to reduce binary size
 * WARNING: may cause compatibility problems
 */

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
