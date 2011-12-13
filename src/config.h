/* You can reduce binary size and RAM consumption by comment out options */

// #define ALLOW_BOLD          TRUE
// #define BELL_AUDIBLE        TRUE
// #define BELL_VISIBLE        TRUE
// #define DEFAULT_ENCODING    "UTF-8"

/* If you encounter focus problem, try edit this option */
// #define FOCUS_BUG           TRUE

// #define INITIAL_TAB_NUMBER  1
// #define TAB_LABEL           "Page"

/* You must also define TAB_LABEL before setting TAB_LABEL_NUMBER */
// #define TAB_LABEL_NUMBER    TRUE

#define MENU_POPUP          TRUE
// #define MOUSE_AUTOHIDE      TRUE
// #define SCROLL_BACKGROUND   TRUE
// #define SCROLL_ON_KEYSTROKE TRUE
// #define SCROLL_ON_OUTPUT    TRUE
// #define SCROLLBACK_LINES    60

/* Enable SCROLLBAR_LEFT will disable SCROLLBAR_RIGHT automatically */
// #define SCROLLBAR_LEFT      TRUE
#define SCROLLBAR_RIGHT     TRUE

#define STATUSBAR           TRUE
// #define TERMINAL_FONT       "Monospace 10"
#define ANTI_ALIAS          VTE_ANTI_ALIAS_USE_DEFAULT
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_FORCE_ENABLE
// #define ANTI_ALIAS          VTE_ANTI_ALIAS_FORCE_DISABLE
// #define TERMINAL_COLS       80
// #define TERMINAL_ROWS       24
// #define WORD_CHARS          "-A-Za-z0-9_$.+!*(),;:@&=?/~#%"

/* Do not comment out options below */

#define ENABLE_LASTLOG      TRUE
#define ENABLE_UTMP         TRUE
#define ENABLE_WTMP         TRUE
#ifdef MENU_POPUP
static char *encoding[] = {"UTF-8", "KOI8-R", "CP866", "CP1251"};
#endif

/* Do not edit options below unless you know what you are doing */

#define DEFAULT_COMMAND     g_getenv("SHELL")
#define DEFAULT_ARGV        NULL
#define DEFAULT_ENVV        NULL
#define DEFAULT_DIRECTORY   g_getenv("PWD")

/* You can reduce binary size by uncomment those options */

// #define GTK_BOX
// #define GTK_CONTAINER
// #define GTK_NOTEBOOK
// #define GTK_WINDOW
// #define VTE_TERMINAL

/* G_CALLBACK GTK_MENU GTK_STATUSBAR */
