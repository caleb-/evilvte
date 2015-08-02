#ifndef INNER_BORDER_H
#define INNER_BORDER_H 2
#endif

#ifndef INNER_BORDER_W
#define INNER_BORDER_W 2
#endif

#if defined(COLOR_STYLE) && (COLOR_STYLE == VTE_FIXED)
#undef COLOR_STYLE
#endif

#ifndef VTE_FORK_CMD_OLD
#define VTE_FORK_CMD_OLD TRUE
#endif

#if VTE_CHECK_VERSION(0,25,1) && defined(VTE_DISABLE_DEPRECATED)
#undef VTE_FORK_CMD_OLD
#define VTE_FORK_CMD_OLD FALSE
#endif

#ifndef VTE_COLUMNS
#define VTE_COLUMNS 0
#endif

#ifndef VTE_ROWS
#define VTE_ROWS 0
#endif

#define GET_CURRENT_TAB(x) term = (struct terminal*)g_object_get_data(G_OBJECT(gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook), (x))), "current_tab")

#ifndef DEFAULT_COMMAND
#define DEFAULT_COMMAND g_getenv("SHELL")
#endif

#ifndef RECORD_LASTLOG
#define RECORD_LASTLOG TRUE
#endif
#define EVILVTE_PTY_NO_LASTLOG 0
#if !RECORD_LASTLOG
#undef EVILVTE_PTY_NO_LASTLOG
#define EVILVTE_PTY_NO_LASTLOG VTE_PTY_NO_LASTLOG
#endif

#ifndef RECORD_UTMP
#define RECORD_UTMP TRUE
#endif
#define EVILVTE_PTY_NO_UTMP 0
#if !RECORD_UTMP
#undef EVILVTE_PTY_NO_UTMP
#define EVILVTE_PTY_NO_UTMP VTE_PTY_NO_UTMP
#endif

#ifndef RECORD_WTMP
#define RECORD_WTMP TRUE
#endif
#define EVILVTE_PTY_NO_WTMP 0
#if !RECORD_WTMP
#undef EVILVTE_PTY_NO_WTMP
#define EVILVTE_PTY_NO_WTMP VTE_PTY_NO_WTMP
#endif

#define EVILVTE_PTY_NO_HELPER 0
#if !RECORD_LASTLOG && !RECORD_UTMP && !RECORD_WTMP
#undef EVILVTE_PTY_NO_HELPER
#define EVILVTE_PTY_NO_HELPER VTE_PTY_NO_HELPER
#endif

#ifndef LABEL_DEFAULT_ENCODING
#define LABEL_DEFAULT_ENCODING "_Default Encoding"
#endif

#ifndef LABEL_DIALOG_BACKGROUND_TINT
#define LABEL_DIALOG_BACKGROUND_TINT "_Background tint color"
#endif

#ifndef LABEL_DIALOG_CLOSE
#define LABEL_DIALOG_CLOSE "Do you really want to close it?"
#endif

#ifndef LABEL_DIALOG_SEARCH
#define LABEL_DIALOG_SEARCH "Find"
#endif

#ifndef LABEL_MENU_SATURATION
#define LABEL_MENU_SATURATION "_Adjust saturation"
#endif

#ifndef LABEL_MENU_TOGGLE_ANTI_ALIAS
#define LABEL_MENU_TOGGLE_ANTI_ALIAS "_Toggle anti-alias"
#endif

#ifndef LABEL_MENU_TOGGLE_BG
#define LABEL_MENU_TOGGLE_BG "_Toggle background"
#endif

#ifndef LABEL_MENU_TOGGLE_DECORATED
#define LABEL_MENU_TOGGLE_DECORATED "_Toggle window decorated"
#endif

#ifndef LABEL_MENU_TOGGLE_FULLSCREEN
#define LABEL_MENU_TOGGLE_FULLSCREEN "_Toggle fullscreen"
#endif

#ifndef LABEL_MENU_TOGGLE_HOTKEYS
#define LABEL_MENU_TOGGLE_HOTKEYS "_Toggle hotkeys locking"
#endif

#ifndef LABEL_MENU_TOGGLE_ON_TOP
#define LABEL_MENU_TOGGLE_ON_TOP "_Toggle always on top"
#endif

#ifndef LABEL_MENU_TOGGLE_SCROLLBAR
#define LABEL_MENU_TOGGLE_SCROLLBAR "_Toggle scrollbar"
#endif

#ifndef LABEL_MENU_TOGGLE_STATUS_BAR
#define LABEL_MENU_TOGGLE_STATUS_BAR "_Toggle status bar"
#endif

#ifndef LABEL_MENU_TOGGLE_TABBAR
#define LABEL_MENU_TOGGLE_TABBAR "_Toggle tabbar"
#endif

#ifndef LABEL_SUBMENU_ENCODING
#define LABEL_SUBMENU_ENCODING "_Character Encoding"
#endif

#ifndef LABEL_SUBMENU_IME
#define LABEL_SUBMENU_IME "_Input Methods"
#endif

#define MATCH_HTTP_DATA "((f|F)|(h|H)(t|T))(t|T)(p|P)(s|S)?://(([^|.< \t\r\n\\\"]*([.][^|< \t\r\n\\\"])?[^|.< \t\r\n\\\"]*)*[^< \t\r\n,;|\\\"]*[^|.< \t\r\n\\\"])?/*"
#define MATCH_FILE_DATA "(f|F)(i|I)(l|L)(e|E):///(([^|.< \t\r\n\\\"]*([.][^|< \t\r\n\\\"])?[^|.< \t\r\n\\\"]*)*[^< \t\r\n,;|\\\"]*[^|.< \t\r\n\\\"])?/*"
#define MATCH_MAIL_DATA "(m|M)(a|A)(i|I)(l|L)(t|T)(o|O):(([^|.< \t\r\n\\\"]*([.][^|< \t\r\n\\\"])?[^|.< \t\r\n\\\"]*)*@[^< \t\r\n,;|\\\"]*[^|.< \t\r\n\\\"])?/*"

#ifndef CLOSE_DIALOG
#define CLOSE_DIALOG FALSE
#endif

#if CLOSE_DIALOG
#define DEL_TAB del_tab
#define DEL_TAB_DECL del_tab
#endif
#if !CLOSE_DIALOG
#define DEL_TAB(x,y) del_tab()
#define DEL_TAB_DECL(x,y) del_tab(void)
#endif

#ifndef TAB
#define TAB 0
#endif

#if !TAB
#undef COMMAND_TAB_NUMBERS
#undef SHOW_WINDOW_BORDER
#undef TAB_BORDER
#undef TAB_BORDER_VERTICAL
#undef TAB_BORDER_HORIZONTAL
#undef TAB_CLOSE_BUTTON
#undef TAB_EXPANDED_WIDTH
#undef TAB_LABEL
#undef TAB_LABEL_DYNAMIC
#undef TAB_LABEL_CUSTOM
#undef TAB_NEW_PATH_EQUAL_OLD
#undef TAB_REORDERABLE
#undef TAB_SHOW_INFO_AT_TITLE
#undef TABBAR
#undef TABBAR_PLACE
#undef TABBAR_AUTOHIDE
#undef TABBAR_SCROLLABLE
#undef TABBAR_MENU_SELECT_TAB
#define SHOW_WINDOW_BORDER FALSE
#define TABBAR FALSE
#endif

#ifndef SHOW_WINDOW_ICON
#define SHOW_WINDOW_ICON 0
#endif
#ifndef COMMAND_EXEC_PROGRAM
#define COMMAND_EXEC_PROGRAM 0
#endif
#ifndef COMMAND_SHOW_OPTIONS
#define COMMAND_SHOW_OPTIONS 0
#endif
#ifndef COMMAND_SHOW_VERSION
#define COMMAND_SHOW_VERSION 0
#endif
#ifndef CLOSE_SAFELY
#define CLOSE_SAFELY 0
#endif
#ifndef EXPORT_WINDOWID
#define EXPORT_WINDOWID 0
#endif
#ifndef TAB_NEW_PATH_EQUAL_OLD
#define TAB_NEW_PATH_EQUAL_OLD 0
#endif
#ifndef MATCH_STRING_HTTP
#define MATCH_STRING_HTTP 0
#endif
#ifndef MATCH_STRING_MAIL
#define MATCH_STRING_MAIL 0
#endif
#ifndef MATCH_STRING_FILE
#define MATCH_STRING_FILE 0
#endif
#ifndef TAB_NEW_TAB_AT_TAB_ONE
#define TAB_NEW_TAB_AT_TAB_ONE 0
#endif
#ifndef TAB_LABEL_DYNAMIC
#define TAB_LABEL_DYNAMIC 0
#endif
#ifndef MOUSE_CTRL_SATURATION
#define MOUSE_CTRL_SATURATION 0
#endif
#ifndef TABBAR_AUTOHIDE
#define TABBAR_AUTOHIDE 0
#endif
#ifndef TAB_EXPANDED_WIDTH
#define TAB_EXPANDED_WIDTH 0
#endif
#ifndef TAB_SHOW_INFO_AT_TITLE
#define TAB_SHOW_INFO_AT_TITLE 0
#endif
#ifndef WINDOW_TITLE_DYNAMIC
#define WINDOW_TITLE_DYNAMIC 0
#endif
#ifndef TAB_CLOSE_BUTTON
#define TAB_CLOSE_BUTTON 0
#endif
#ifndef TABBAR_SCROLLABLE
#define TABBAR_SCROLLABLE 0
#endif
#ifndef TABBAR_MENU_SELECT_TAB
#define TABBAR_MENU_SELECT_TAB 0
#endif
#ifndef BELL_URGENT
#define BELL_URGENT 0
#endif
#ifndef TAB_REORDERABLE
#define TAB_REORDERABLE 0
#endif
#ifndef COMMAND_TAB_NUMBERS
#define COMMAND_TAB_NUMBERS 0
#endif
#ifndef COMMAND_FULLSCREEN
#define COMMAND_FULLSCREEN 0
#endif
#ifndef COMMAND_GEOMETRY
#define COMMAND_GEOMETRY 0
#endif
#ifndef COMMAND_DOCK_MODE
#define COMMAND_DOCK_MODE 0
#endif
#ifndef COMMAND_AT_ROOT_WINDOW
#define COMMAND_AT_ROOT_WINDOW 0
#endif
#ifndef COMMAND_LOGIN_SHELL
#define COMMAND_LOGIN_SHELL 0
#endif
#ifndef BACKGROUND_OPACITY
#define BACKGROUND_OPACITY 0
#endif
#ifndef COMMAND_SET_TITLE
#define COMMAND_SET_TITLE 0
#endif
#ifndef PROGRAM_WM_CLASS
#define PROGRAM_WM_CLASS 0
#endif
#ifndef BUTTON_ORDER_BY_RCFILE
#define BUTTON_ORDER_BY_RCFILE 0
#endif
#ifndef COMMAND_SHOW_HELP
#define COMMAND_SHOW_HELP 0
#endif
#ifndef COMMAND_FONT
#define COMMAND_FONT 0
#endif
#ifndef COMMAND_COLOR_FG
#define COMMAND_COLOR_FG 0
#endif
#ifndef COMMAND_COLOR_BG
#define COMMAND_COLOR_BG 0
#endif
#ifndef COMMAND_SATURATION
#define COMMAND_SATURATION 0
#endif

#define GET_VTE_CHILD_PID NULL
#if VTE_FORK_CMD_OLD
#undef GET_VTE_CHILD_PID
#define GET_VTE_CHILD_PID
#endif

#if TAB_NEW_PATH_EQUAL_OLD || CLOSE_DIALOG || CLOSE_SAFELY
#undef GET_VTE_CHILD_PID
#define GET_VTE_CHILD_PID &(term->pid)
#if VTE_FORK_CMD_OLD
#undef GET_VTE_CHILD_PID
#define GET_VTE_CHILD_PID term->pid =
#endif
#endif

#if TAB_NEW_PATH_EQUAL_OLD
#ifndef DEFAULT_DIRECTORY
#define DEFAULT_DIRECTORY g_get_current_dir()
#endif
#define VTE_DEFAULT_DIRECTORY default_directory
char *default_directory;
#endif

#if !TAB_NEW_PATH_EQUAL_OLD
#ifndef DEFAULT_DIRECTORY
#define DEFAULT_DIRECTORY NULL
#endif
#define VTE_DEFAULT_DIRECTORY DEFAULT_DIRECTORY
#endif

#ifndef HOTKEY_HAS_DEFINE
#undef HOTKEY
#endif

#ifndef HOTKEY
#define HOTKEY 0
#endif

#if !HOTKEY
#undef MENU_TOGGLE_HOTKEYS
#undef HOTKEY_COLOR_BACKGROUND
#undef HOTKEY_FONT_BIGGER
#undef HOTKEY_FONT_SMALLER
#undef HOTKEY_FONT_DEFAULT_SIZE
#undef HOTKEY_FONT_SELECT
#undef HOTKEY_HAS_DEFINE
#undef HOTKEY_MIMIC_SCROLL_UP
#undef HOTKEY_MIMIC_SCROLL_DOWN
#undef HOTKEY_SATURATION_DIALOG
#undef HOTKEY_SATURATION_MORE
#undef HOTKEY_SATURATION_LESS
#undef HOTKEY_SEARCH_STRING
#undef HOTKEY_SEARCH_PREVIOUS
#undef HOTKEY_SEARCH_NEXT
#undef HOTKEY_TOGGLE_ANTI_ALIAS
#undef HOTKEY_TOGGLE_DECORATED
#undef HOTKEY_TOGGLE_FULLSCREEN
#undef HOTKEY_TOGGLE_HOTKEYS
#undef HOTKEY_TOGGLE_ON_TOP
#undef HOTKEY_TOGGLE_SCROLLBAR
#undef HOTKEY_TOGGLE_STATUS_BAR
#undef HOTKEY_TOGGLE_BACKGROUND
#endif

#if defined(HOTKEY_TAB_GO_TO_NUMBER) && !defined(CTRL_NUMBER_GO_TO_TAB_NUMBER)
#define CTRL_NUMBER_GO_TO_TAB_NUMBER HOTKEY_TAB_GO_TO_NUMBER
#endif

#if !TAB || !HOTKEY
#undef HOTKEY_TAB_ADD
#undef HOTKEY_TAB_REMOVE
#undef HOTKEY_TAB_PREVIOUS
#undef HOTKEY_TAB_NEXT
#undef HOTKEY_TAB_FIRST
#undef HOTKEY_TAB_LAST
#undef ALT_NUMBER_GO_TO_TAB_NUMBER
#undef CTRL_NUMBER_GO_TO_TAB_NUMBER
#undef HOTKEY_TAB_EDIT_LABEL
#undef HOTKEY_TOGGLE_TABBAR
#endif

#ifndef ALT_NUMBER_GO_TO_TAB_NUMBER
#define ALT_NUMBER_GO_TO_TAB_NUMBER 0
#endif
#ifndef CTRL_NUMBER_GO_TO_TAB_NUMBER
#define CTRL_NUMBER_GO_TO_TAB_NUMBER 0
#endif

#if BUTTON_ORDER_BY_RCFILE
bool button_order = FALSE;
#endif

#ifndef MENU
#define MENU 0
#endif

#if !MENU
#undef MENU_ENCODING_LIST
#undef MENU_MATCH_STRING_EXEC
#undef MENU_CUSTOM
#endif

#ifndef MENU_CUSTOM_SIZE
#undef MENU_CUSTOM
#endif

#ifndef MENU_ENCODING_LIST_SIZE
#undef MENU_ENCODING_LIST
#endif

#ifndef TOGGLE_BG_ORDER_SIZE
#undef TOGGLE_BG_ORDER
#endif

#ifndef MENU_CUSTOM
#undef MENU_COPY
#undef MENU_PASTE
#undef MENU_SELECT_ALL
#undef MENU_COLOR_BACKGROUND
#undef MENU_OPEN_NEW_WINDOW
#undef MENU_QUIT
#undef MENU_FONT_BIGGER
#undef MENU_FONT_SMALLER
#undef MENU_FONT_DEFAULT_SIZE
#undef MENU_RESET_TERMINAL
#undef MENU_RESET_AND_CLEAR
#undef MENU_FONT_SELECT
#undef MENU_SEPARATOR
#undef SUBMENU_ENCODING_LIST
#undef SUBMENU_INPUT_METHOD
#undef MENU_TOGGLE_BACKGROUND
#undef MENU_TOGGLE_HOTKEYS
#undef MENU_TOGGLE_ON_TOP
#undef MENU_TOGGLE_SCROLLBAR
#undef MENU_TOGGLE_STATUS_BAR
#undef MENU_TOGGLE_DECORATED
#undef MENU_TOGGLE_FULLSCREEN
#undef MENU_TOGGLE_ANTI_ALIAS
#undef MENU_CHANGE_SATURATION
#undef ONLY_ONE_MENU_ITEM
#endif

#ifndef STATUS_BAR
#define STATUS_BAR 0
#endif
#ifndef SCROLLBAR
#define SCROLLBAR OFF_R
#endif

#if !TAB || !defined(MENU_CUSTOM)
#undef MENU_TAB_ADD
#undef MENU_TAB_REMOVE
#undef MENU_TAB_EDIT_LABEL
#undef MENU_TOGGLE_TABBAR
#endif

#if defined(HOTKEY_TOGGLE_ON_TOP) || defined(MENU_TOGGLE_ON_TOP)
#ifndef PROGRAM_ALWAYS_ON_TOP
#define PROGRAM_ALWAYS_ON_TOP FALSE
#endif
#endif

#if defined(HOTKEY_SATURATION_DIALOG) || defined(MENU_CHANGE_SATURATION)
GtkWidget *adjustment;
#endif

#if defined(HOTKEY_TOGGLE_HOTKEYS) || defined(MENU_TOGGLE_HOTKEYS)
bool hotkey_status = TRUE;
#endif

#ifdef MENU_ENCODING_LIST
char *encoding[] = { MENU_ENCODING_LIST };
#endif

#if !defined(MENU_ENCODING_LIST) && MENU
GtkWidget *encoding_item;
#endif

#if defined(TOGGLE_BG_ORDER_SIZE) && (TOGGLE_BG_ORDER_SIZE == 1)
#undef HOTKEY_TOGGLE_BACKGROUND
#undef MENU_TOGGLE_BACKGROUND
#endif

#if defined(HOTKEY_TOGGLE_BACKGROUND) || defined(MENU_TOGGLE_BACKGROUND)
#ifndef BACKGROUND_IMAGE
#define BACKGROUND_IMAGE ".config/evilvte/background.png"
#endif
#ifndef TOGGLE_BG_ORDER
#define TOGGLE_BG_ORDER "Image", "Transparent", "No background", "Opacity"
#undef TOGGLE_BG_ORDER_SIZE
#define TOGGLE_BG_ORDER_SIZE 4
#define TOGGLE_BG_OPACITY
#define TOGGLE_BG_IMAGE
#define TOGGLE_BG_TRANSPARENT
#define TOGGLE_BG_NO_BACKGROUND
#endif
const char *background_order[] = { TOGGLE_BG_ORDER };
unsigned short background_status = 0;
#endif

#ifdef BACKGROUND_IMAGE
char imgstr[sizeof(BACKGROUND_IMAGE) + 64];
#endif

#ifdef PROGRAM_ICON
char iconstr[sizeof(PROGRAM_ICON) + 64];
#endif

#if defined(BACKGROUND_IMAGE) || (defined(BACKGROUND_TRANSPARENT) && BACKGROUND_TRANSPARENT) || defined(TOGGLE_BG_TRANSPARENT)
#define BACKGROUND_EXIST
#endif

#if defined(HOTKEY_COLOR_BACKGROUND) || defined(MENU_COLOR_BACKGROUND)
#ifndef BACKGROUND_TINT_COLOR
#define BACKGROUND_TINT_COLOR "black"
#endif
#endif

#if defined(BACKGROUND_TINT_COLOR) && defined(BACKGROUND_EXIST)
GdkColor color_tint;
#endif

#if defined(HOTKEY_TOGGLE_SCROLLBAR) || defined(MENU_TOGGLE_SCROLLBAR)
#ifndef SCROLLBAR
#define SCROLLBAR RIGHT
#endif
bool scrollbar_status = (SCROLLBAR < 3);
#endif

#if !defined(HOTKEY_TOGGLE_SCROLLBAR) && !defined(MENU_TOGGLE_SCROLLBAR) && (SCROLLBAR > 3)
#undef SCROLLBAR
#endif

#ifdef SCROLLBAR
#define VTE_HBOX term->hbox
#endif
#ifndef SCROLLBAR
#define VTE_HBOX term->vte
#endif

#if defined(HOTKEY_TOGGLE_STATUS_BAR) || defined(MENU_TOGGLE_STATUS_BAR)
bool status_bar_status = STATUS_BAR;
bool status_bar_resize_grip = FALSE;
#endif

#if defined(HOTKEY_TOGGLE_DECORATED) || defined(MENU_TOGGLE_DECORATED)
#ifdef SHOW_WINDOW_DECORATED
bool window_decorated_status = SHOW_WINDOW_DECORATED;
#endif
#ifndef SHOW_WINDOW_DECORATED
bool window_decorated_status = TRUE;
#endif
#endif

#if defined(HOTKEY_TOGGLE_FULLSCREEN) || defined(MENU_TOGGLE_FULLSCREEN) || defined(VTE_FUNNY)
bool window_fullscreen_status = FALSE;
#endif

#if VTE_CHECK_VERSION(0,19,1) && defined(VTE_DISABLE_DEPRECATED)
#undef FONT_ANTI_ALIAS
#undef HOTKEY_TOGGLE_ANTI_ALIAS
#undef MENU_TOGGLE_ANTI_ALIAS
#endif

#if defined(HOTKEY_TOGGLE_ANTI_ALIAS) || defined(MENU_TOGGLE_ANTI_ALIAS)
#if defined(FONT_ANTI_ALIAS) && !FONT_ANTI_ALIAS
unsigned short antialias_status = VTE_ANTI_ALIAS_FORCE_DISABLE;
#endif
#if !defined(FONT_ANTI_ALIAS) || FONT_ANTI_ALIAS
unsigned short antialias_status = VTE_ANTI_ALIAS_FORCE_ENABLE;
#endif
#endif

#define ONLY_ONE_MENU_HANDLE
#ifdef ONLY_ONE_MENU_ITEM
#undef ONLY_ONE_MENU_HANDLE
#define ONLY_ONE_MENU_HANDLE || (menu_item_success == 1)
#endif

#ifdef MENU_CUSTOM
const char *menu_custom[] = { MENU_CUSTOM };
unsigned short menu_item_success = 0;
#endif

#if TABBAR_AUTOHIDE && !defined(TABBAR)
#define TABBAR TRUE
#endif

#if defined(HOTKEY_TOGGLE_TABBAR) || defined(MENU_TOGGLE_TABBAR)
#ifdef TABBAR
bool tabbar_status = TABBAR;
#endif
#ifndef TABBAR
bool tabbar_status = TRUE;
#endif
#define VTE_TABBAR tabbar_status
#endif

#if !defined(HOTKEY_TOGGLE_TABBAR) && !defined(MENU_TOGGLE_TABBAR)
#define VTE_TABBAR TABBAR
#endif

#if defined(MENU_MATCH_STRING_EXEC) || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
#if !MATCH_STRING_HTTP && !MATCH_STRING_MAIL && !MATCH_STRING_FILE && !defined(MATCH_STRING)
#undef MATCH_STRING_HTTP
#define MATCH_STRING_HTTP TRUE
#endif
#endif

#ifdef FONT_ANTI_ALIAS
#define VTE_ANTI_ALIAS VTE_ANTI_ALIAS_FORCE_ENABLE
#if !FONT_ANTI_ALIAS
#undef VTE_ANTI_ALIAS
#define VTE_ANTI_ALIAS VTE_ANTI_ALIAS_FORCE_DISABLE
#endif
#endif

#ifndef FONT_ANTI_ALIAS
#define VTE_ANTI_ALIAS VTE_ANTI_ALIAS_USE_DEFAULT
#endif

#define EVILVTE_SET_FONT(x,y,z) vte_terminal_set_font_from_string(x,y)
#if defined(FONT_ANTI_ALIAS) || defined(MENU_TOGGLE_ANTI_ALIAS) || defined(HOTKEY_TOGGLE_ANTI_ALIAS)
#undef EVILVTE_SET_FONT
#define EVILVTE_SET_FONT vte_terminal_set_font_from_string_full
#endif

#ifndef PROGRAM_NAME
#define PROGRAM_NAME "evilvte"
#define UPPER_PROGRAM_NAME "Evilvte"
#endif

#if PROGRAM_WM_CLASS
char *wm_class_name = PROGRAM_NAME;
char *wm_class_class = UPPER_PROGRAM_NAME;
#endif

#define VTE_REGEX 0
#if defined(SEARCH_CASE_SENSITIVE) && !SEARCH_CASE_SENSITIVE
#undef VTE_REGEX
#define VTE_REGEX G_REGEX_CASELESS
#endif

#define VTE_PROGRAM_NAME PROGRAM_NAME
#if COMMAND_SET_TITLE
#undef VTE_PROGRAM_NAME
#define VTE_PROGRAM_NAME program_name
char *program_name = PROGRAM_NAME;
#endif

#if COMMAND_FONT
char *command_font = NULL;
#endif

#if COMMAND_COLOR_BG
char *command_color_bg = NULL;
#endif

#if COMMAND_COLOR_FG
char *command_color_fg = NULL;
#endif

#if COMMAND_GEOMETRY
char *command_geometry = NULL;
#endif

GtkWidget *main_window;
GtkWidget *notebook;

#define VTE_LABEL NULL
#if defined(TAB_LABEL) || defined(TAB_LABEL_CUSTOM)
#undef VTE_LABEL
#define VTE_LABEL label
#endif

#if defined(GTK3_CSS_USE_BOX) && defined(USE_GTK_GRID)
#if CLOSE_DIALOG || TAB_CLOSE_BUTTON || defined(SCROLLBAR) || STATUS_BAR || defined(HOTKEY_TOGGLE_STATUS_BAR) || defined(MENU_TOGGLE_STATUS_BAR)
#warning "You are using GtkGrid but your GTK3_CSS defined GtkHBox or GtkVBox."
#endif
#endif

#if defined(DEF_TAB_LABEL) || TAB_CLOSE_BUTTON
#ifndef TAB_LABEL
#define TAB_LABEL "Page %u"
#endif
#endif

#if TAB_CLOSE_BUTTON
#undef VTE_LABEL
#define VTE_LABEL term->label
#endif

#if TAB_CLOSE_BUTTON
#ifndef GTK3_CSS
#ifdef USE_GTK_GRID
#define GTK3_CSS "GtkNotebook GtkGrid GtkButton { -GtkWidget-focus-line-width: 0; } GtkNotebook GtkButton { border-width: 0; padding: 0; -GtkButton-inner-border: 0; }"
#endif
#ifndef USE_GTK_GRID
#define GTK3_CSS "GtkNotebook GtkHBox GtkButton { -GtkWidget-focus-line-width: 0; } GtkNotebook GtkButton { border-width: 0; padding: 0; -GtkButton-inner-border: 0; }"
#endif
#endif
#endif

#ifdef TAB_LABEL_CUSTOM
#undef TAB_LABEL
const char *label_style_custom[] = { TAB_LABEL_CUSTOM };
const int label_style_size = sizeof(label_style_custom) / sizeof(label_style_custom[0]);
#endif

#ifdef COLOR_STYLE
const GdkColor color_style[16];
#if COLOR_STYLE != USER_CUSTOM
  #undef USER_COLOR_00
  #undef USER_COLOR_01
  #undef USER_COLOR_02
  #undef USER_COLOR_03
  #undef USER_COLOR_04
  #undef USER_COLOR_05
  #undef USER_COLOR_06
  #undef USER_COLOR_07
  #undef USER_COLOR_08
  #undef USER_COLOR_09
  #undef USER_COLOR_10
  #undef USER_COLOR_11
  #undef USER_COLOR_12
  #undef USER_COLOR_13
  #undef USER_COLOR_14
  #undef USER_COLOR_15
#endif
#if COLOR_STYLE == LINUX
  #define USER_COLOR_00 "#000000"
  #define USER_COLOR_01 "#a80000"
  #define USER_COLOR_02 "#00a800"
  #define USER_COLOR_03 "#a85700"
  #define USER_COLOR_04 "#0000a8"
  #define USER_COLOR_05 "#a800a8"
  #define USER_COLOR_06 "#00a8a8"
  #define USER_COLOR_07 "#a8a8a8"
  #define USER_COLOR_08 "#575757"
  #define USER_COLOR_09 "#ff5757"
  #define USER_COLOR_10 "#57ff57"
  #define USER_COLOR_11 "#ffff57"
  #define USER_COLOR_12 "#5757ff"
  #define USER_COLOR_13 "#ff57ff"
  #define USER_COLOR_14 "#57ffff"
  #define USER_COLOR_15 "#ffffff"
#endif
#if COLOR_STYLE == RXVT
  #define USER_COLOR_00 "#000000"
  #define USER_COLOR_01 "#cd0000"
  #define USER_COLOR_02 "#00cd00"
  #define USER_COLOR_03 "#cdcd00"
  #define USER_COLOR_04 "#0000cd"
  #define USER_COLOR_05 "#cd00cd"
  #define USER_COLOR_06 "#00cdcd"
  #define USER_COLOR_07 "#faebd7"
  #define USER_COLOR_08 "#404040"
  #define USER_COLOR_09 "#ff0000"
  #define USER_COLOR_10 "#00ff00"
  #define USER_COLOR_11 "#ffff00"
  #define USER_COLOR_12 "#0000ff"
  #define USER_COLOR_13 "#ff00ff"
  #define USER_COLOR_14 "#00ffff"
  #define USER_COLOR_15 "#ffffff"
#endif
#if COLOR_STYLE == TANGO
  #define USER_COLOR_00 "#2e3436"
  #define USER_COLOR_01 "#cc0000"
  #define USER_COLOR_02 "#4e9a06"
  #define USER_COLOR_03 "#c4a000"
  #define USER_COLOR_04 "#3465a4"
  #define USER_COLOR_05 "#75507b"
  #define USER_COLOR_06 "#06989a"
  #define USER_COLOR_07 "#d3d7cf"
  #define USER_COLOR_08 "#555753"
  #define USER_COLOR_09 "#ef2929"
  #define USER_COLOR_10 "#8ae234"
  #define USER_COLOR_11 "#fce94f"
  #define USER_COLOR_12 "#729fcf"
  #define USER_COLOR_13 "#ad7fa8"
  #define USER_COLOR_14 "#34e2e2"
  #define USER_COLOR_15 "#eeeeec"
#endif
#if COLOR_STYLE == XTERM
  #define USER_COLOR_00 "#000000"
  #define USER_COLOR_01 "#cd0000"
  #define USER_COLOR_02 "#00cd00"
  #define USER_COLOR_03 "#cdcd00"
  #define USER_COLOR_04 "#0000ee"
  #define USER_COLOR_05 "#cd00cd"
  #define USER_COLOR_06 "#00cdcd"
  #define USER_COLOR_07 "#e5e5e5"
  #define USER_COLOR_08 "#7f7f7f"
  #define USER_COLOR_09 "#ff0000"
  #define USER_COLOR_10 "#00ff00"
  #define USER_COLOR_11 "#ffff00"
  #define USER_COLOR_12 "#5c5cff"
  #define USER_COLOR_13 "#ff00ff"
  #define USER_COLOR_14 "#00ffff"
  #define USER_COLOR_15 "#ffffff"
#endif
#if COLOR_STYLE == ZENBURN_DARK
  #define USER_COLOR_00 "#000000"
  #define USER_COLOR_01 "#9e1828"
  #define USER_COLOR_02 "#aece92"
  #define USER_COLOR_03 "#968a38"
  #define USER_COLOR_04 "#414171"
  #define USER_COLOR_05 "#963c59"
  #define USER_COLOR_06 "#418179"
  #define USER_COLOR_07 "#bebebe"
  #define USER_COLOR_08 "#666666"
  #define USER_COLOR_09 "#cf6171"
  #define USER_COLOR_10 "#c5f779"
  #define USER_COLOR_11 "#fff796"
  #define USER_COLOR_12 "#4186be"
  #define USER_COLOR_13 "#cf9ebe"
  #define USER_COLOR_14 "#71bebe"
  #define USER_COLOR_15 "#ffffff"
#endif
#if COLOR_STYLE == ZENBURN
  #define USER_COLOR_00 "#3f3f3f"
  #define USER_COLOR_01 "#705050"
  #define USER_COLOR_02 "#60b48a"
  #define USER_COLOR_03 "#dfaf8f"
  #define USER_COLOR_04 "#506070"
  #define USER_COLOR_05 "#dc8cc3"
  #define USER_COLOR_06 "#8cd0d3"
  #define USER_COLOR_07 "#dcdccc"
  #define USER_COLOR_08 "#709080"
  #define USER_COLOR_09 "#dca3a3"
  #define USER_COLOR_10 "#c3bf9f"
  #define USER_COLOR_11 "#f0dfaf"
  #define USER_COLOR_12 "#94bff3"
  #define USER_COLOR_13 "#ec93d3"
  #define USER_COLOR_14 "#93e0e3"
  #define USER_COLOR_15 "#ffffff"
#endif
#if COLOR_STYLE == SOLARIZED_DARK
  #define USER_COLOR_00 "#073642"
  #define USER_COLOR_01 "#dc322f"
  #define USER_COLOR_02 "#859900"
  #define USER_COLOR_03 "#b58900"
  #define USER_COLOR_04 "#268bd2"
  #define USER_COLOR_05 "#d33682"
  #define USER_COLOR_06 "#2aa198"
  #define USER_COLOR_07 "#eee8d5"
  #define USER_COLOR_08 "#002b36"
  #define USER_COLOR_09 "#cb4b16"
  #define USER_COLOR_10 "#586e75"
  #define USER_COLOR_11 "#657b83"
  #define USER_COLOR_12 "#839496"
  #define USER_COLOR_13 "#6c71c4"
  #define USER_COLOR_14 "#93a1a1"
  #define USER_COLOR_15 "#fdf6e3"
#endif
#if COLOR_STYLE == SOLARIZED_LIGHT
  #define USER_COLOR_00 "#eee8d5"
  #define USER_COLOR_01 "#dc322f"
  #define USER_COLOR_02 "#859900"
  #define USER_COLOR_03 "#b58900"
  #define USER_COLOR_04 "#268bd2"
  #define USER_COLOR_05 "#d33682"
  #define USER_COLOR_06 "#2aa198"
  #define USER_COLOR_07 "#073642"
  #define USER_COLOR_08 "#fdf6e3"
  #define USER_COLOR_09 "#cb4b16"
  #define USER_COLOR_10 "#93a1a1"
  #define USER_COLOR_11 "#839496"
  #define USER_COLOR_12 "#657b83"
  #define USER_COLOR_13 "#6c71c4"
  #define USER_COLOR_14 "#586e75"
  #define USER_COLOR_15 "#002b36"
#endif
#if COLOR_STYLE == HOLO
  #define USER_COLOR_00 "#000000"
  #define USER_COLOR_01 "#cc0000"
  #define USER_COLOR_02 "#669900"
  #define USER_COLOR_03 "#ff8800"
  #define USER_COLOR_04 "#0099cc"
  #define USER_COLOR_05 "#9933cc"
  #define USER_COLOR_06 "#0099cc"
  #define USER_COLOR_07 "#a8a8a8"
  #define USER_COLOR_08 "#575757"
  #define USER_COLOR_09 "#ff4444"
  #define USER_COLOR_10 "#99cc00"
  #define USER_COLOR_11 "#ffbb33"
  #define USER_COLOR_12 "#33b5e5"
  #define USER_COLOR_13 "#aa66cc"
  #define USER_COLOR_14 "#33b5e5"
  #define USER_COLOR_15 "#ffffff"
#endif
#endif

#if defined(FONT_ANTI_ALIAS) || defined(MENU_FONT_BIGGER) || defined(MENU_FONT_SMALLER) || defined(MENU_FONT_SELECT) || defined(MENU_TOGGLE_ANTI_ALIAS) || defined(HOTKEY_TOGGLE_ANTI_ALIAS) || defined(HOTKEY_FONT_BIGGER) || defined(HOTKEY_FONT_SMALLER) || defined(HOTKEY_FONT_SELECT) || COMMAND_FONT
#ifndef FONT
#define FONT "Monospace 10"
#endif
#endif

#if !defined(MENU_FONT_BIGGER) && !defined(MENU_FONT_SMALLER) && !defined(MENU_FONT_SELECT) && !defined(HOTKEY_FONT_BIGGER) && !defined(HOTKEY_FONT_SMALLER) && !defined(HOTKEY_FONT_SELECT)
#undef MENU_FONT_DEFAULT_SIZE
#undef HOTKEY_FONT_DEFAULT_SIZE
#endif

#ifdef FONT
char font_name[125];
char font_str[128];
double font_size;
#if defined(HOTKEY_FONT_DEFAULT_SIZE) || defined(MENU_FONT_DEFAULT_SIZE)
double font_size_default;
#endif
#endif

#if COMMAND_EXEC_PROGRAM
#define VTE_DEFAULT_COMMAND default_command
char default_command[256];
char **default_argv = NULL;
#endif

#if !COMMAND_EXEC_PROGRAM
#define VTE_DEFAULT_COMMAND DEFAULT_COMMAND
#define default_argv NULL
#endif

#if COMMAND_LOGIN_SHELL && VTE_FORK_CMD_OLD
char *login_shell[] = { "-", NULL };
#endif

#if COMMAND_LOGIN_SHELL || (!VTE_FORK_CMD_OLD && COMMAND_EXEC_PROGRAM)
unsigned short login_shell_flag = 0;
#endif

#if defined(HOTKEY_SATURATION_MORE) || defined(HOTKEY_SATURATION_LESS) || MOUSE_CTRL_SATURATION || BACKGROUND_OPACITY || defined(HOTKEY_SATURATION_DIALOG) || defined(MENU_CHANGE_SATURATION) || defined(HOTKEY_TOGGLE_BACKGROUND) || defined(MENU_TOGGLE_BACKGROUND) || COMMAND_SATURATION
#ifndef BACKGROUND_SATURATION
#define BACKGROUND_SATURATION 0.4
#endif
#endif

#ifdef PROGRAM_ALWAYS_ON_TOP
bool always_on_top = PROGRAM_ALWAYS_ON_TOP;
#endif

#ifdef BACKGROUND_SATURATION
double saturation_level = BACKGROUND_SATURATION;
#endif

#if COMMAND_AT_ROOT_WINDOW
bool at_root_window = FALSE;
#endif

#if MENU
GtkWidget *menu;
#endif

#ifdef MENU_COPY
GtkWidget *menu_copy;
#endif

#ifdef MENU_PASTE
GtkWidget *menu_paste;
#endif

#ifdef MENU_FONT_DEFAULT_SIZE
GtkWidget *menu_zoom_100;
#endif

#ifdef MENU_MATCH_STRING_EXEC
GtkWidget *match_open;
GtkWidget *match_copy;
GtkWidget *match_item;
#endif

#if defined(MENU_MATCH_STRING_EXEC) || defined(MATCH_STRING_L) || defined(MATCH_STRING_M)
char *matched_url = NULL;
#endif

#if STATUS_BAR || defined(HOTKEY_TOGGLE_STATUS_BAR) || defined(MENU_TOGGLE_STATUS_BAR)
GtkWidget *statusbar;
GtkWidget *vbox;
#endif

#if defined(HOTKEY_SEARCH_STRING) || defined(HOTKEY_SEARCH_PREVIOUS) || defined(HOTKEY_SEARCH_NEXT)
  GtkWidget *global_search_string;
#endif

struct terminal {
  GtkWidget *vte;
#if CLOSE_DIALOG || CLOSE_SAFELY || TAB_NEW_PATH_EQUAL_OLD
  int pid;
#endif
#if TAB_CLOSE_BUTTON
  GtkWidget *button;
  GtkWidget *label;
  GtkWidget *label_edit;
#endif
#ifdef SCROLLBAR
  GtkWidget *hbox;
  GtkWidget *scrollbar;
#endif
#if defined(HOTKEY_TAB_EDIT_LABEL) || defined(MENU_TAB_EDIT_LABEL)
  bool label_exist;
#endif
#if defined(HOTKEY_SEARCH_STRING) || defined(HOTKEY_SEARCH_PREVIOUS) || defined(HOTKEY_SEARCH_NEXT)
  GtkWidget *search_string;
  bool global_string;
#endif
};

struct terminal *term;
