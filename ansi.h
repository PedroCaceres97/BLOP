#ifndef __BLOP_ANSI_H__
#define __BLOP_ANSI_H__

#define ANSI_ESC  "\x1b["

#define ANSI_COLOR(color, text)  color text ANSI_RESET

#define ANSI_FG_BLACK           ANSI_ESC "30m"
#define ANSI_FG_RED             ANSI_ESC "31m"
#define ANSI_FG_GREEN           ANSI_ESC "32m"
#define ANSI_FG_YELLOW          ANSI_ESC "33m"
#define ANSI_FG_BLUE            ANSI_ESC "34m"
#define ANSI_FG_MAGENTA         ANSI_ESC "35m"
#define ANSI_FG_CYAN            ANSI_ESC "36m"
#define ANSI_FG_WHITE           ANSI_ESC "37m"

#define ANSI_BG_BLACK           ANSI_ESC "40m"
#define ANSI_BG_RED             ANSI_ESC "41m"
#define ANSI_BG_GREEN           ANSI_ESC "42m"
#define ANSI_BG_YELLOW          ANSI_ESC "43m"
#define ANSI_BG_BLUE            ANSI_ESC "44m"
#define ANSI_BG_MAGENTA         ANSI_ESC "45m"
#define ANSI_BG_CYAN            ANSI_ESC "46m"
#define ANSI_BG_WHITE           ANSI_ESC "47m"

#define ANSI_RESET_FULL         ANSI_ESC "c"
#define ANSI_RESET              ANSI_ESC "0m"
#define ANSI_BOLD               ANSI_ESC "1m"
#define ANSI_DIM                ANSI_ESC "2m"
#define ANSI_ITALIC             ANSI_ESC "3m"
#define ANSI_UNDERLINE          ANSI_ESC "4m"
#define ANSI_BLINK              ANSI_ESC "5m"
#define ANSI_REVERSE            ANSI_ESC "7m"
#define ANSI_HIDDEN             ANSI_ESC "8m"
#define ANSI_STRIKETHROUGH      ANSI_ESC "9m"
#define ANSI_DOUBLE_UNDER       ANSI_ESC "21m"
#define ANSI_OVERLINE           ANSI_ESC "53m"

#define ANSI_FG_256(n)          ANSI_ESC "38;5;" #n "m"
#define ANSI_BG_256(n)          ANSI_ESC "48;5;" #n "m"
#define ANSI_FG_RGB(r, g, b)    ANSI_ESC "38;2;" #r ";" #g ";" #b "m"
#define ANSI_BG_RGB(r, g, b)    ANSI_ESC "48;2;" #r ";" #g ";" #b "m"

#define ANSI_CURSOR_UP(n)       ANSI_ESC #n "A"
#define ANSI_CURSOR_DOWN(n)     ANSI_ESC #n "B"
#define ANSI_CURSOR_FORWARD(n)  ANSI_ESC #n "C"
#define ANSI_CURSOR_BACK(n)     ANSI_ESC #n "D"
#define ANSI_CURSOR_POS(x, y)   ANSI_ESC #y ";" #x "H"

#define ANSI_CURSOR_SAVE        ANSI_ESC "s"
#define ANSI_CURSOR_RESTORE     ANSI_ESC "u"
#define ANSI_CURSOR_HIDE        ANSI_ESC "?25l"
#define ANSI_CURSOR_SHOW        ANSI_ESC "?25h"

#define ANSI_CLEAR_SCREEN       ANSI_ESC "2J"
#define ANSI_CLEAR_LINE         ANSI_ESC "2K"
#define ANSI_CLEAR_TO_END       ANSI_ESC "0J"
#define ANSI_CLEAR_TO_START     ANSI_ESC "1J"
#define ANSI_CLEAR_LINE_END     ANSI_ESC "0K"
#define ANSI_CLEAR_LINE_START   ANSI_ESC "1K"

#endif /* __BLOP_ANSI_H__ */