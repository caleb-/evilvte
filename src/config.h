/* Comment out options to reduce binary size and consumption of resources. */

// #define ALLOW_BOLD             TRUE
#define    BACKGROUND_IMAGE       ".config/evilvte/background.png"
// #define BACKGROUND_SATURATION  0.5     /* Between 0 and 1 */
// #define BACKGROUND_TINT_COLOR  "brown"
// #define BACKGROUND_TRANSPARENT TRUE
// #define BELL_AUDIBLE           TRUE
// #define BELL_VISIBLE           TRUE
// #define BINDING_BACKSPACE_KEY  VTE_ERASE_AUTO
// #define BINDING_BACKSPACE_KEY  VTE_ERASE_ASCII_BACKSPACE
// #define BINDING_BACKSPACE_KEY  VTE_ERASE_ASCII_DELETE
// #define BINDING_BACKSPACE_KEY  VTE_ERASE_DELETE_SEQUENCE
// #define BINDING_DELETE_KEY     VTE_ERASE_AUTO
// #define BINDING_DELETE_KEY     VTE_ERASE_ASCII_BACKSPACE
// #define BINDING_DELETE_KEY     VTE_ERASE_ASCII_DELETE
// #define BINDING_DELETE_KEY     VTE_ERASE_DELETE_SEQUENCE
// #define BUTTON_ORDER_GTK_STYLE TRUE /* Accept gtk-alternative-button-order */
// #define BUTTON_ORDER_QT_STYLE  TRUE
// #define CLOSE_SAFELY           TRUE /* Do not kill background applications */
// #define COLOR_BACKGROUND       "white"  /* "#FFFFFF" */
// #define COLOR_FOREGROUND       "black"
// #define COLOR_BOLD             "red"
// #define COLOR_CURSOR           "blue"
// #define COLOR_DIM              "yellow"
// #define COLOR_HIGHLIGHT        "green"
// #define COLOR_STYLE_LINUX      TRUE
// #define COLOR_STYLE_RXVT       TRUE
// #define COLOR_STYLE_TANGO      TRUE
// #define COLOR_STYLE_XTERM      TRUE
#define    COMMAND_EXEC_COMMAND   TRUE /* -e option, execute program */
#define    COMMAND_SHOW_CONFIG    TRUE /* -o option, show build-time options */
#define    COMMAND_SHOW_VERSION   TRUE /* -v option, show program version */
// #define CURSOR_BLINKS          TRUE
// #define DEFAULT_COLUMNS        80
// #define DEFAULT_ROWS           24
// #define DEFAULT_COMMAND        g_getenv("SHELL")
// #define DEFAULT_ARGV           NULL
// #define DEFAULT_ENVV           NULL
// #define DEFAULT_DIRECTORY      g_getenv("PWD")
// #define DEFAULT_ENCODING       "UTF-8"
// #define DEFAULT_FONT           "Monospace"
// #define DEFAULT_FONT_SIZE      10
// #define DEFAULT_FONT_SIMPLE    "Monospace 10" /* Can not change font size */
// #define ANTI_ALIAS             VTE_ANTI_ALIAS_USE_DEFAULT
// #define ANTI_ALIAS             VTE_ANTI_ALIAS_FORCE_ENABLE
// #define ANTI_ALIAS             VTE_ANTI_ALIAS_FORCE_DISABLE
// #define EMULATION_TYPE         "xterm"
// #define ENABLE_LASTLOG         TRUE
// #define ENABLE_UTMP            TRUE
// #define ENABLE_WTMP            TRUE
// #define MOUSE_AUTOHIDE         TRUE
#define    PROGRAM_NAME           "evilvte"
// #define SCROLL_BACKGROUND      TRUE
// #define SCROLL_LINES           60
// #define SCROLL_ON_KEYSTROKE    TRUE
// #define SCROLL_ON_OUTPUT       TRUE
// #define SCROLLBAR_LEFT         TRUE
#define    SCROLLBAR_RIGHT        TRUE
// #define SHOW_WINDOW_BORDER     TRUE
#define    SHOW_WINDOW_ICON       TRUE
#define    SHOW_WINDOW_TITLE      TRUE
#define    STATUS_BAR             TRUE
#define    WORD_CHARS             "-A-Za-z0-9_$.+!*(),;:@&=?/~#%"



/* Right click menu
 *
 * Turn off MENU will disable all, except TABBAR_MENU_SELECT_TAB.
 * It is controlled by TAB and TABBAR.
 */

#define    MENU                   TRUE
// #define MENU_COPY              TRUE
// #define MENU_PASTE             TRUE
// #define MENU_TAB_ADD_TAB       TRUE
// #define MENU_TAB_REMOVE_TAB    TRUE
// #define MENU_TAB_EDIT_LABEL    TRUE
// #define MENU_NEW_WINDOW        TRUE
// #define MENU_FONT_ZOOM_IN      TRUE
// #define MENU_FONT_ZOOM_OUT     TRUE
// #define MENU_FONT_ZOOM_100     TRUE /* Default font size */
// #define MENU_RESET             TRUE
// #define MENU_RESET_AND_CLEAR   TRUE
// #define MENU_QUIT              TRUE
// #define MENU_SEPARATOR_1       TRUE /* Separator above MENU_ENCODING_LIST */
// #define MENU_SEPARATOR_2       TRUE /* Separator below MENU_ENCODING_LIST */
#define    MENU_INPUT_METHOD      TRUE
// #define MENU_CUSTOM            "Copy","Paste","Add tab","Close tab"
// #define MENU_CUSTOM            "Edit label","New window","Zoom in","Zoom out"
// #define MENU_CUSTOM            "Zoom 100","Reset","Reset and clear","Quit"
// #define MENU_CUSTOM            "Separator","Encoding list","Input method"
#define    MENU_ENCODING_LIST     "UTF-8"
// #define MENU_ENCODING_LIST     "BIG-5","Default","GBK","EUC-JP","SHIFT-JIS"
// #define MENU_ENCODING_DEFAULT  "Default Encoding"
                                  /* If you define MENU_ENCODING_DEFAULT,
                                   * you can define a "Default" item in
                                   * MENU_ENCODING_LIST. It will be the initial
                                   * encoding when program starts.
                                   *
                                   * You can put "Default" anywhere in the
                                   * MENU_ENCODING_LIST.
                                   */
/* Tab
 *
 * Turn off TAB will disable all related options automatically,
 * including hotkey definitions.
 */

#define    TAB                     TRUE
// #define TAB_AT_BOTTOM           TRUE
// #define TAB_AT_LEFT             TRUE
// #define TAB_AT_RIGHT            TRUE
// #define TAB_AT_TOP              TRUE
// #define TAB_BORDER              0
// #define TAB_BORDER_HORIZONTAL   0
// #define TAB_BORDER_VERTICAL     0
// #define TAB_EXPAND              TRUE /* Dynamic tab width */
// #define TAB_INFO_AT_TITLE       TRUE /* Show tab info at window title */
// #define TAB_INITIAL_NUMBER      1
// #define TAB_LABEL               "Page"
// #define TAB_LABEL_NUMBER        TRUE   /* Depends on TAB_LABEL */
// #define TAB_LABEL_STYLE_POEM    TRUE
// #define TAB_LABEL_STYLE_CUSTOM  "One","Two","Three","Four","Five","Six"
// #define TAB_NEW_TAB_OLD_PATH    TRUE /* New tab with current working path */
// #define TABBAR                  TRUE
// #define TABBAR_AUTOHIDE         TRUE
// #define TABBAR_MENU_SELECT_TAB  TRUE /* Right click tabbar menu */
// #define TABBAR_MOUSE_SCROLLABLE TRUE



/* Hotkey definitions
 *
 * Please see /usr/include/gtk-2.0/gdk/gdkkeysyms.h
 *
 * GDK_A = "Shift + a", so hotkey will be "Ctrl + Shift + a"
 *
 * You may use || to define multiple keys
 * For example: (event->keyval == GDK_a || event->keyval == GDK_A)
 *
 * Turn off CTRL will disable all hotkeys automatically.
 */

#define    CTRL                      TRUE
// #define CTRL_COPY_TO_CLIPBOARD    (event->keyval == GDK_C)
// #define CTRL_PASTE_FROM_CLIPBOARD (event->keyval == GDK_V)
// #define CTRL_EDIT_ENCODING        (event->keyval == GDK_U)
// #define CTRL_FONT_BIGGER          (event->keyval == GDK_KP_Add)
// #define CTRL_FONT_SMALLER         (event->keyval == GDK_KP_Subtract)
// #define CTRL_FONT_DEFAULT_SIZE    (event->keyval == GDK_KP_Insert)
// #define CTRL_OPEN_NEW_WINDOW      (event->keyval == GDK_N)
// #define CTRL_RESET_TERMINAL       (event->keyval == GDK_R)
// #define CTRL_RESET_AND_CLEAR_TERM (event->keyval == GDK_A)
// #define CTRL_SATURATION_MORE      (event->keyval == GDK_KP_Multiply)
// #define CTRL_SATURATION_LESS      (event->keyval == GDK_KP_Divide)
#define    CTRL_TAB_OPEN_NEW_TAB     (event->keyval == GDK_T)
#define    CTRL_TAB_CLOSE_TAB        (event->keyval == GDK_W)
#define    CTRL_TAB_PREVIOUS_TAB     (event->keyval == GDK_Page_Up)
#define    CTRL_TAB_NEXT_TAB         (event->keyval == GDK_Page_Down)
// #define CTRL_TAB_FIRST_TAB        (event->keyval == GDK_Home)
// #define CTRL_TAB_LAST_TAB         (event->keyval == GDK_End)
// #define CTRL_TAB_JUMP_NUMBER      TRUE /* Ctrl + number -> goto tab number */
// #define CTRL_TAB_EDIT_LABEL       (event->keyval == GDK_E)
// #define CTRL_TOGGLE_BACKGROUND    (event->keyval == GDK_KP_Delete)
// #define      TOGGLE_BG_ORDER      "Image","Transparent","No background"
// #define      TOGGLE_BG_ORDER      "Image"
                                     /* Define only one "Image" means reload
                                      * image.
                                      *
                                      * Notice: Background image can be changed
                                      *         only when there is only one tab.
                                      *
                                      *         When multiple tab is opened,
                                      *         background image is not
                                      *         changeable.
                                      *
                                      *         It is an VTE problem,
                                      *         not my fault.
                                      */

/* If you enable DOUBLE_PRESS_HOTKEY,
 * press CTRL_TAB_OPEN_NEW_TAB or CTRL_TAB_CLOSE_TAB twice quickly to add or remove a tab
 */

// #define DOUBLE_PRESS_HOTKEY    TRUE
// #define DOUBLE_PRESS_TIME      300000 /* 0.3 second = 300000 microseconds */

/* uncomment options below to reduce binary size
 * WARNING: may cause compatibility problems
 */

// #undef G_CALLBACK
// #undef GTK_BOX
// #undef GTK_CONTAINER
// #undef GTK_ENTRY
// #undef GTK_MENU
// #undef GTK_MENU_SHELL
// #undef GTK_NOTEBOOK
// #undef GTK_STATUSBAR
// #undef GTK_WINDOW
// #undef VTE_TERMINAL
// #define G_CALLBACK
// #define GTK_BOX
// #define GTK_CONTAINER
// #define GTK_ENTRY
// #define GTK_MENU
// #define GTK_MENU_SHELL
// #define GTK_NOTEBOOK
// #define GTK_STATUSBAR
// #define GTK_WINDOW
// #define VTE_TERMINAL
