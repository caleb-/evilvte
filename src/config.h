/* Use // to disable options                                                  */

#define    BACKGROUND_IMAGE       ".config/evilvte/background.png"
// #define BACKGROUND_SATURATION  0.4
// #define BACKGROUND_SCROLLABLE  TRUE
// #define BACKGROUND_TINT_COLOR  "black"
// #define BACKGROUND_TRANSPARENT TRUE  /* Pseudo transparent background      */
// #define BACKGROUND_OPACITY     TRUE  /* True transparent background        */
// #define BACKSPACE_KEY /* Options: AUTO, BACKSPACE, DELETE, DELETE_SEQUENCE */
// #define DELETE_KEY    /* Options: AUTO, BACKSPACE, DELETE, DELETE_SEQUENCE */
// #define BELL_AUDIBLE           TRUE
// #define BELL_VISIBLE           TRUE
// #define BUTTON_ORDER_BY_RCFILE TRUE
// #define CLOSE_DIALOG           TRUE  /* Dialog to confirm close event      */
// #define CLOSE_SAFELY           TRUE  /* Do not kill running applications   */
// #define COLOR_BACKGROUND       "white"
// #define COLOR_FOREGROUND       "#000000"
// #define COLOR_STYLE   /* Options: VTE_FIXED, LINUX, RXVT, TANGO, XTERM     */
// #define COLOR_TEXT_BOLD        "red"
// #define COLOR_TEXT_DIM         "#FFFF00"
// #define COLOR_TEXT_HIGHLIGHTED "green"
// #define COMMAND_AT_ROOT_WINDOW TRUE  /* -r option, run in root window      */
#define    COMMAND_EXEC_PROGRAM   TRUE  /* -e option, execute program         */
// #define COMMAND_FULLSCREEN     TRUE  /* -f option, init in fullscreen mode */
// #define COMMAND_SET_TITLE      TRUE  /* -T or -title to set program title  */
// #define COMMAND_SHOW_HELP      TRUE  /* -h option, show help               */
#define    COMMAND_SHOW_OPTIONS   TRUE  /* -o option, show build-time options */
#define    COMMAND_SHOW_VERSION   TRUE  /* -v option, show program version    */
// #define COMMAND_TAB_NUMBERS    TRUE  /* -[2-9] options, to set tab numbers */
// #define CURSOR_BLINKS          TRUE
// #define CURSOR_COLOR           "blue"
// #define DEFAULT_ARGV           NULL
// #define DEFAULT_COLUMNS        80
// #define DEFAULT_ROWS           24
// #define DEFAULT_COMMAND        g_getenv("SHELL")
// #define DEFAULT_DIRECTORY      g_get_current_dir()
// #define DEFAULT_EMULATION_TYPE "xterm"
// #define DEFAULT_ENCODING       "UTF-8"
// #define DEFAULT_ENVV           NULL
// #define FONT                   "Monospace"
// #define FONT_SIZE              10
// #define FONT_ANTI_ALIAS        TRUE
// #define FONT_ENABLE_BOLD_TEXT  TRUE
// #define MOUSE_CTRL_SATURATION  TRUE  /* Scroll left or right to adjust it  */
// #define MOUSE_CURSOR_AUTOHIDE  TRUE
// #define PROGRAM_ALWAYS_ON_TOP  TRUE
// #define PROGRAM_NAME           "evilvte"
// #define PROGRAM_VERSION        "7.7.7"
// #define PROGRAM_WM_CLASS       TRUE
// #define RECORD_LASTLOG         TRUE
// #define RECORD_UTMP            TRUE
// #define RECORD_WTMP            TRUE
// #define SCROLL_LINES           1000
// #define SCROLL_ON_KEYSTROKE    TRUE
// #define SCROLL_ON_OUTPUT       TRUE
// #define SCROLLBAR_LEFT         TRUE
#define    SCROLLBAR_RIGHT        TRUE
// #define SHOW_WINDOW_BORDER     TRUE
// #define SHOW_WINDOW_DECORATED  TRUE
#define    SHOW_WINDOW_ICON       TRUE
#define    SHOW_WINDOW_TITLE      TRUE
#define    STATUS_BAR             TRUE
// #define VIRTUAL_KEYBOARD       TRUE
#define    WORD_CHARS             "-A-Za-z0-9_$.+!*(),;:@&=?/~#%"

#define    MENU                   TRUE
// #define MENU_ENCODING_LIST     "BIG-5", "Default Encoding", "GBK", "EUC-JP"
// #define MENU_MATCH_STRING_EXEC "firefox"
// #define      MATCH_STRING      "((f|F)(t|T)(p|P)|((h|H)(t|T)(t|T)(p|P)(s|S)*))://[-a-zA-Z0-9.?$%&/=_~#.,:;+]*"
// #define MENU_CUSTOM            "Copy", "Separator", "Paste", "Separator"
           /* Options: "Copy", "Paste", "Select_all", "Add tab", "Remove tab",
            *          "Edit label", "New window", "Quit", "Zoom in",
            *          "Zoom out", "Zoom default", "Reset terminal",
            *          "Reset and clear", "Select font", "Separator",
            *          "Encoding list", "Input method", "Submenu encoding list",
            *          "Submenu input method", "Toggle background",
            *          "Toggle scrollbar", "Toggle status bar",
            *          "Toggle tabbar", "Toggle window decorated",
            *          "Toggle anti-alias", "Toggle hotkeys locking",
            *          "Toggle always on top", "Toggle fullscreen",
            *          "Adjust saturation", "Background tint"                 */

#define    TAB                    TRUE
// #define TAB_BORDER             0
// #define TAB_BORDER_HORIZONTAL  0
// #define TAB_BORDER_VERTICAL    0
// #define TAB_CLOSE_BUTTON       TRUE
// #define TAB_EXPANDED_WIDTH     TRUE
// #define TAB_LABEL              "Page"
// #define TAB_LABEL_NUMBER       TRUE
// #define TAB_LABEL_POEM         TRUE
// #define TAB_LABEL_CUSTOM       "One", "Two", "Three", "Four", "Five", "Six"
// #define TAB_NEW_PATH_EQUAL_OLD TRUE
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
 * See /usr/include/gtk-2.0/gdk/gdkkeysyms.h for GDK_* definitions.
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
// #define HOTKEY_OPEN_NEW_WINDOW       CTRL GDK_N
// #define HOTKEY_RESET_TERMINAL        CTRL GDK_R
// #define HOTKEY_RESET_AND_CLEAR       CTRL GDK_A
// #define HOTKEY_SATURATION_DIALOG     CTRL GDK_X
// #define HOTKEY_SATURATION_MORE       CTRL GDK_KP_Multiply
// #define HOTKEY_SATURATION_LESS       CTRL GDK_KP_Divide
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
// #define        TOGGLE_BG_ORDER       "Image", "Transparent", "No background", "Opacity"

// #define LABEL_DEFAULT_ENCODING       "Default Encoding"
// #define LABEL_DIALOG_BACKGROUND_TINT "Background tint color"
// #define LABEL_DIALOG_CLOSE           "Do you really want to close it?"
// #define LABEL_DIALOG_SATURATION      "Saturation level"
// #define LABEL_MENU_SATURATION        "Adjust saturation"
// #define LABEL_MENU_TOGGLE_ANTI_ALIAS "Toggle anti-alias"
// #define LABEL_MENU_TOGGLE_BG         "Toggle background"
// #define LABEL_MENU_TOGGLE_DECORATED  "Toggle window decorated"
// #define LABEL_MENU_TOGGLE_FULLSCREEN "Toggle fullscreen"
// #define LABEL_MENU_TOGGLE_HOTKEYS    "Toggle hotkeys locking"
// #define LABEL_MENU_TOGGLE_ON_TOP     "Toggle always on top"
// #define LABEL_MENU_TOGGLE_SCROLLBAR  "Toggle scrollbar"
// #define LABEL_MENU_TOGGLE_STATUS_BAR "Toggle status bar"
// #define LABEL_MENU_TOGGLE_TABBAR     "Toggle tabbar"
// #define LABEL_SUBMENU_ENCODING       "Character Encoding"
// #define LABEL_SUBMENU_IME            "Input Methods"
// #define VIRTUAL_KEYBOARD_MENU_KEY    "Menu" /* " 目 "                    */
// #define VIRTUAL_KEYBOARD_WINDOW_KEY  "Win"  /* "ﾐ田"                     */
