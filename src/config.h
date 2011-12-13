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
// #define COMMAND_SET_TITLE      TRUE  /* -title option, set program title   */
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
 * (event->keyval == GDK_A || event->keyval == GDK_a)
 *                   GDK_A = "Shift + a", so hotkey will be "Ctrl + Shift + a"
 * See /usr/include/gtk-2.0/gdk/gdkkeysyms.h for GDK_* definitions.
 *
 * If you enable "CTRL_ALT", all hotkeys will become Ctrl + Alt + hotkey(s).  */

#define    CTRL                   TRUE
// #define CTRL_ALT               TRUE
// #define CTRL_COPY              (event->keyval == GDK_C)
// #define CTRL_PASTE             (event->keyval == GDK_V)
// #define CTRL_SELECT_ALL        (event->keyval == GDK_S)
// #define CTRL_COLOR_BACKGROUND  (event->keyval == GDK_B)
// #define CTRL_EDIT_ENCODING     (event->keyval == GDK_U)
// #define CTRL_FONT_BIGGER       (event->keyval == GDK_KP_Add)
// #define CTRL_FONT_SMALLER      (event->keyval == GDK_KP_Subtract)
// #define CTRL_FONT_DEFAULT_SIZE (event->keyval == GDK_KP_Insert)
// #define CTRL_FONT_SELECT       (event->keyval == GDK_F)
// #define CTRL_OPEN_NEW_WINDOW   (event->keyval == GDK_N)
// #define CTRL_RESET_TERMINAL    (event->keyval == GDK_R)
// #define CTRL_RESET_AND_CLEAR   (event->keyval == GDK_A)
// #define CTRL_SATURATION_DIALOG (event->keyval == GDK_X)
// #define CTRL_SATURATION_MORE   (event->keyval == GDK_KP_Multiply)
// #define CTRL_SATURATION_LESS   (event->keyval == GDK_KP_Divide)
#define    CTRL_TAB_ADD           (event->keyval == GDK_T)
#define    CTRL_TAB_REMOVE        (event->keyval == GDK_W)
#define    CTRL_TAB_PREVIOUS      (event->keyval == GDK_Page_Up)
#define    CTRL_TAB_NEXT          (event->keyval == GDK_Page_Down)
// #define CTRL_TAB_FIRST         (event->keyval == GDK_Home)
// #define CTRL_TAB_LAST          (event->keyval == GDK_End)
// #define CTRL_TAB_GO_TO_NUMBER  TRUE
// #define CTRL_TAB_EDIT_LABEL    (event->keyval == GDK_E)
// #define CTRL_TOGGLE_ANTI_ALIAS (event->keyval == GDK_L)
// #define CTRL_TOGGLE_DECORATED  (event->keyval == GDK_Left)
// #define CTRL_TOGGLE_FULLSCREEN (event->keyval == GDK_M)
// #define CTRL_TOGGLE_HOTKEYS    (event->keyval == GDK_quoteleft)
// #define CTRL_TOGGLE_ON_TOP     (event->keyval == GDK_O)
// #define CTRL_TOGGLE_SCROLLBAR  (event->keyval == GDK_Right)
// #define CTRL_TOGGLE_STATUS_BAR (event->keyval == GDK_Down)
// #define CTRL_TOGGLE_TABBAR     (event->keyval == GDK_Up)
// #define CTRL_TOGGLE_BACKGROUND (event->keyval == GDK_KP_Delete)
// #define      TOGGLE_BG_ORDER   "Image", "Transparent", "No background", "Opacity"

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
