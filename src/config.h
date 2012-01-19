/* Use // to disable options                                                  */

#define    BACKGROUND_IMAGE       ".config/evilvte/background.png"
// #define BACKGROUND_SATURATION  0.4
// #define BACKGROUND_SCROLLABLE  TRUE
// #define BACKGROUND_TINT_COLOR  "black"
// #define BACKGROUND_TRANSPARENT TRUE  /* Pseudo transparent background      */
// #define BACKGROUND_OPACITY     TRUE  /* True transparent background        */
// #define BACKSPACE_KEY /* Options: AUTO, BACKSPACE, DELETE, ERASE_TTY,      */
                         /*          DELETE_SEQUENCE                          */
// #define DELETE_KEY    /* Options: AUTO, BACKSPACE, DELETE, ERASE_TTY,      */
                         /*          DELETE_SEQUENCE                          */
// #define BELL_AUDIBLE           TRUE
// #define BELL_VISIBLE           TRUE
// #define BELL_URGENT            TRUE
// #define BUTTON_ORDER_BY_RCFILE TRUE
// #define CLOSE_DIALOG           TRUE  /* Dialog to confirm close event      */
// #define CLOSE_SAFELY           TRUE  /* Do not kill running applications   */
// #define COLOR_BACKGROUND       "white"
// #define COLOR_FOREGROUND       "#000000"
// #define COLOR_STYLE   /* Options: LINUX, RXVT, TANGO, XTERM, ZENBURN,      */
                         /*          ZENBURN_DARK                             */
// #define COLOR_TEXT_BOLD        "red"
// #define COLOR_TEXT_DIM         "#FFFF00"
// #define COLOR_TEXT_HIGHLIGHTED "green"
// #define COMMAND_AT_ROOT_WINDOW TRUE  /* -r option, run in root window      */
// #define COMMAND_DOCK_MODE      TRUE  /* -d option, run as a dock           */
#define    COMMAND_EXEC_PROGRAM   TRUE  /* -e option, execute program         */
// #define COMMAND_FULLSCREEN     TRUE  /* -f option, init in fullscreen mode */
// #define COMMAND_FONT           TRUE  /* -fn option, to assign font & size  */
// #define COMMAND_GEOMETRY       TRUE  /* -g +X+Y option, to assign geometry */
// #define COMMAND_LOGIN_SHELL    TRUE  /* -ls option, to enable login shell  */
// #define COMMAND_SET_TITLE      TRUE  /* -T or -title to set program title  */
// #define COMMAND_SHOW_HELP      TRUE  /* -h option, show help               */
#define    COMMAND_SHOW_OPTIONS   TRUE  /* -o option, show build-time options */
#define    COMMAND_SHOW_VERSION   TRUE  /* -v option, show program version    */
// #define COMMAND_TAB_NUMBERS    TRUE  /* -[2-9] options, to set tab numbers */
// #define CURSOR_BLINKS          TRUE
// #define CURSOR_COLOR           "blue"
// #define CURSOR_SHAPE  /* Options: BLOCK, IBEAM, UNDERLINE                  */
// #define DEFAULT_COMMAND        g_getenv("SHELL")
// #define DEFAULT_DIRECTORY      g_get_current_dir()
// #define DEFAULT_ENCODING       "UTF-8"
// #define DEFAULT_TERMINAL_SIZE  80x24
// #define FONT                   "Monospace 10"
// #define FONT_ANTI_ALIAS        TRUE
// #define FONT_ENABLE_BOLD_TEXT  TRUE
// #define MOUSE_CTRL_SATURATION  TRUE  /* Scroll left or right to adjust it  */
// #define MOUSE_CURSOR_AUTOHIDE  TRUE
// #define PROGRAM_ALWAYS_ON_TOP  TRUE
// #define PROGRAM_ICON           ".config/evilvte/icon.png"
// #define PROGRAM_NAME           "evilvte"
// #define PROGRAM_VERSION        "7.7.7"
// #define PROGRAM_WM_CLASS       TRUE
// #define RECORD_LASTLOG         TRUE
// #define RECORD_UTMP            TRUE
// #define RECORD_WTMP            TRUE
// #define SCROLL_LINES           1000  /* Negative value means unlimited     */
// #define SCROLL_ON_KEYSTROKE    TRUE
// #define SCROLL_ON_OUTPUT       TRUE
#define    SCROLLBAR              RIGHT /* Options: LEFT, RIGHT, OFF_L, OFF_R */
// #define SHOW_WINDOW_BORDER     TRUE
// #define SHOW_WINDOW_DECORATED  TRUE
#define    SHOW_WINDOW_ICON       TRUE
#define    STATUS_BAR             TRUE
// #define WINDOW_TITLE_DYNAMIC   TRUE  /* for xterm escape sequences         */
#define    WORD_CHARS             "-A-Za-z0-9_$.+!*(),;:@&=?/~#%"

#define    MENU                   TRUE
// #define MENU_ENCODING_LIST     "BIG-5", "Default Encoding", "GBK", "EUC-JP"
// #define MENU_MATCH_STRING_EXEC "firefox"
// #define      MATCH_STRING_L    "firefox" /* left click to open without menu*/
// #define      MATCH_STRING_M    "firefox" /* middle click to open           */
// #define      MATCH_STRING_HTTP TRUE      /* Detect http(s) and ftp(s)      */
// #define      MATCH_STRING_MAIL TRUE      /* Detect mailto:                 */
// #define      MATCH_STRING_FILE TRUE      /* Detect file:///                */
// #define MENU_CUSTOM            "Copy", "Separator", "Paste", "Separator"
           /* Options: "Copy",         "Zoom in",       "Add tab",
            *          "Paste",        "Zoom out",      "Remove tab",
            *          "Select all",   "Zoom default",  "New window",
            *          "Edit label",   "Select font",
            *          "Input method",                  "Encoding list",
            *          "Submenu input method",          "Submenu encoding list",
            *          "Toggle tabbar",
            *          "Toggle scrollbar",              "Background tint",
            *          "Toggle status bar",             "Adjust saturation",
            *          "Toggle anti-alias",
            *          "Toggle background",             "Separator",
            *          "Toggle fullscreen",             "Quit",
            *          "Toggle always on top",
            *          "Toggle hotkeys locking",        "Reset terminal",
            *          "Toggle window decorated",       "Reset and clear",
            *
            *          "Auto-click" (when there is only one effective menu item)
            */

#define    TAB                    TRUE
// #define TAB_BORDER             0
// #define TAB_CLOSE_BUTTON       TRUE
// #define TAB_EXPANDED_WIDTH     TRUE
// #define TAB_LABEL              "Page %u" /* %u will be replaced by numbers */
// #define TAB_LABEL_DYNAMIC      TRUE      /* for xterm escape sequences     */
// #define TAB_LABEL_CUSTOM       "One", "Two", "Three", "Four", "Five", "Six"
// #define TAB_NEW_PATH_EQUAL_OLD TRUE
// #define TAB_NEW_TAB_AT_TAB_ONE TRUE   /* New tab will always the first tab */
// #define TAB_REORDERABLE        TRUE
// #define TAB_SHOW_INFO_AT_TITLE TRUE
// #define TABBAR                 TRUE
// #define TABBAR_PLACE           TOP    /* Options: TOP, BOTTOM, LEFT, RIGHT */
// #define TABBAR_AUTOHIDE        TRUE
// #define TABBAR_SCROLLABLE      TRUE
// #define TABBAR_MENU_SELECT_TAB TRUE

/* Use || for multiple hotkeys.
 *
 * CTRL GDK_A || CTRL GDK_a || CTRL GDK_b
 *
 * CTRL GDK_a = "Ctrl + a"
 * CTRL GDK_A = "Ctrl + Shift + a"
 *
 * See /usr/include/gtk-?.0/gdk/gdkkeysyms.h for GDK_* definitions.
 *
 * If you enable "CTRL_ALT", all hotkeys will become Ctrl + Alt + hotkey(s).  */

#define    HOTKEY                       TRUE
// #define CTRL_ALT                     TRUE
// #define HOTKEY_COPY                  CTRL GDK_C
// #define HOTKEY_PASTE                 CTRL GDK_V
// #define HOTKEY_SELECT_ALL            CTRL GDK_S
// #define HOTKEY_COLOR_BACKGROUND      CTRL GDK_B
// #define HOTKEY_EDIT_ENCODING         CTRL GDK_U
// #define HOTKEY_FONT_BIGGER           CTRL GDK_KP_Add
// #define HOTKEY_FONT_SMALLER          CTRL GDK_KP_Subtract
// #define HOTKEY_FONT_DEFAULT_SIZE     CTRL GDK_KP_Insert
// #define HOTKEY_FONT_SELECT           CTRL GDK_F
// #define HOTKEY_MIMIC_SCROLL_UP       CTRL GDK_K
// #define HOTKEY_MIMIC_SCROLL_DOWN     CTRL GDK_J
// #define HOTKEY_OPEN_NEW_WINDOW       CTRL GDK_N
// #define HOTKEY_RESET_TERMINAL        CTRL GDK_R
// #define HOTKEY_RESET_AND_CLEAR       CTRL GDK_A
// #define HOTKEY_SATURATION_DIALOG     CTRL GDK_X
// #define HOTKEY_SATURATION_MORE       CTRL GDK_KP_Multiply
// #define HOTKEY_SATURATION_LESS       CTRL GDK_KP_Divide
// #define HOTKEY_SCROLL_ONE_PAGE_UP    CTRL GDK_k
// #define HOTKEY_SCROLL_ONE_PAGE_DOWN  CTRL GDK_j
// #define HOTKEY_SEARCH_STRING         CTRL GDK_Q
// #define HOTKEY_SEARCH_PREVIOUS       CTRL GDK_G
// #define HOTKEY_SEARCH_NEXT           CTRL GDK_g
// #define        SEARCH_CASE_SENSITIVE TRUE
#define    HOTKEY_TAB_ADD               CTRL GDK_T
#define    HOTKEY_TAB_REMOVE            CTRL GDK_W
#define    HOTKEY_TAB_PREVIOUS          CTRL GDK_Page_Up
#define    HOTKEY_TAB_NEXT              CTRL GDK_Page_Down
// #define HOTKEY_TAB_FIRST             CTRL GDK_Home
// #define HOTKEY_TAB_LAST              CTRL GDK_End
// #define HOTKEY_TAB_GO_TO_NUMBER      TRUE
// #define HOTKEY_TAB_EDIT_LABEL        CTRL GDK_E
// #define HOTKEY_TOGGLE_ANTI_ALIAS     CTRL GDK_L
// #define HOTKEY_TOGGLE_DECORATED      CTRL GDK_Left
// #define HOTKEY_TOGGLE_FULLSCREEN     CTRL GDK_M
// #define HOTKEY_TOGGLE_HOTKEYS        CTRL GDK_quoteleft
// #define HOTKEY_TOGGLE_ON_TOP         CTRL GDK_O
// #define HOTKEY_TOGGLE_SCROLLBAR      CTRL GDK_Right
// #define HOTKEY_TOGGLE_STATUS_BAR     CTRL GDK_Down
// #define HOTKEY_TOGGLE_TABBAR         CTRL GDK_Up
// #define HOTKEY_TOGGLE_BACKGROUND     CTRL GDK_KP_Delete
// #define TOGGLE_BG_ORDER    "Image", "Transparent", "No background", "Opacity"

// #define LABEL_DEFAULT_ENCODING       "_Default Encoding"
// #define LABEL_DIALOG_BACKGROUND_TINT "_Background tint color"
// #define LABEL_DIALOG_CLOSE           "Do you really want to close it?"
// #define LABEL_DIALOG_SEARCH          "Find"
// #define LABEL_MENU_SATURATION        "_Adjust saturation"
// #define LABEL_MENU_TOGGLE_ANTI_ALIAS "_Toggle anti-alias"
// #define LABEL_MENU_TOGGLE_BG         "_Toggle background"
// #define LABEL_MENU_TOGGLE_DECORATED  "_Toggle window decorated"
// #define LABEL_MENU_TOGGLE_FULLSCREEN "_Toggle fullscreen"
// #define LABEL_MENU_TOGGLE_HOTKEYS    "_Toggle hotkeys locking"
// #define LABEL_MENU_TOGGLE_ON_TOP     "_Toggle always on top"
// #define LABEL_MENU_TOGGLE_SCROLLBAR  "_Toggle scrollbar"
// #define LABEL_MENU_TOGGLE_STATUS_BAR "_Toggle status bar"
// #define LABEL_MENU_TOGGLE_TABBAR     "_Toggle tabbar"
// #define LABEL_SUBMENU_ENCODING       "_Character Encoding"
// #define LABEL_SUBMENU_IME            "_Input Methods"
// #define GTK3_CSS                     "GtkNotebook GtkHBox GtkButton { -GtkWidget-focus-line-width: 0; } GtkNotebook GtkButton { border-width: 0; padding: 0; -GtkButton-inner-border: 0; } GtkWindow GtkNotebook { padding: 0; }"
