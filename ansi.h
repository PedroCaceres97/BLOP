/**
 * @file ansi.h
 * @brief ANSI escape sequence helper macros.
 *
 * A lightweight, header-only collection of macros generating ANSI/VT100 escape
 * sequences for terminal text styling, coloring, cursor control and screen
 * manipulation. All macros expand into constant string literals that may be
 * printed directly to any ANSI-compatible terminal.
 *
 * Usage examples:
 * @code
 *     printf(ANSI_FG_RED "Error!" ANSI_RESET);
 *     printf(ANSI_FG_256(42) "Hello" ANSI_RESET);
 *     printf(ANSI_CURSOR_POS(10, 5));
 * @endcode
 *
 * Important:
 * - Many macros use the # operator and require compile-time literal numbers.
 * - These macros produce raw escape sequences (no validation is performed).
 *
 * @version 1.0.0
 * @author Pedro Caceres
 * @date November 2025
 */

#ifndef __BLOP_ANSI_H__
#define __BLOP_ANSI_H__

/** @defgroup blop_ansi Ansi*/

/**
 * @defgroup ansi_core Core
 * @ingroup blop_ansi
 * @brief Core components of ansi.h
 * @{
 */

/**
 * @name Core
 * @brief Core components of ansi.h
 * @{
 */

/**
 * @def ANSI_ESC
 * @brief Control Sequence Introducer (CSI).  
 * Most ANSI sequences begin with this prefix.
 */

/**
 * @def ANSI_COLOR(color, text)
 * @brief Expand to color + text + reset.
 * @param color Any combination of ANSI color/style macro, may be standard or extended colors.
 * @param text Null-terminated string to colorize.
 * @warning Some consoles/terminals may need to enable color features (Such as Windows cmd)
 */

/** @} */ /* end of ansi_core (name) */

/** @} */ /* end of ansi_core */

/**
 * @defgroup ansi_colors Standard Colors
 * @ingroup blop_ansi
 * @brief Predefined standard ANSI 16-color foreground/background macros.
 * @{
 */

/**
 * @name Standard Colors
 * @brief Predefined standard ANSI 16-color foreground/background macros.
 * @{
 */

/** 
 * @def ANSI_FG_BLACK
 * @brief Set foreground to black (30). 
 */

/**
 * @def ANSI_FG_RED
 * @brief Set foreground to red (31). 
 */

/**
 * @def ANSI_FG_GREEN
 * @brief Set foreground to green (32). 
 */

/**
 * @def ANSI_FG_YELLOW
 * @brief Set foreground to yellow (33). 
 */

/**
 * @def ANSI_FG_BLUE
 * @brief Set foreground to blue (34). 
 */

/**
 * @def ANSI_FG_MAGENTA
 * @brief Set foreground to magenta (35). 
 */

/**
 * @def ANSI_FG_CYAN
 * @brief Set foreground to cyan (36). 
 */

/**
 * @def ANSI_FG_WHITE
 * @brief Set foreground to white (37). 
 */

/** 
 * @def ANSI_BG_BLACK
 * @brief Set background to black (40). 
 */

/** 
 * @def ANSI_BG_RED
 * @brief Set background to red (41). 
 */

/** 
 * @def ANSI_BG_GREEN
 * @brief Set background to green (42). 
 */

/** 
 * @def ANSI_BG_YELLOW
 * @brief Set background to yellow (43). 
 */

/** 
 * @def ANSI_BG_BLUE
 * @brief Set background to blue (44). 
 */

/** 
 * @def ANSI_BG_MAGENTA
 * @brief Set background to magenta (45). 
 */

/** 
 * @def ANSI_BG_CYAN
 * @brief Set background to cyan (46). 
 */

/** 
 * @def ANSI_BG_WHITE
 * @brief Set background to white (47). 
 */


/** @} */ /* end of ansi_colors (name) */

/** @} */ /* end of ansi_colors */

/**
 * @defgroup ansi_styles Text Styles
 * @ingroup blop_ansi
 * @brief Text formatting escape sequences.
 * @{
 */

/**
 * @name Text Styles
 * @brief Text formatting escape sequences. 
 * @{
 */

/**
 * @def ANSI_RESET_FULL
 * @brief Full device reset (ESC c). Resets terminal state.
 */

/**
 * @def ANSI_RESET
 * @brief Reset all graphics attributes (SGR 0).
 */

/**
 * @def ANSI_BOLD
 * @brief Bold text.
 */

/**
 * @def ANSI_DIM
 * @brief Dim / faint text.
 */

/**
 * @def ANSI_ITALIC
 * @brief Italic text.
 */

/**
 * @def ANSI_UNDERLINE
 * @brief Underlined text.
 */

/**
 * @def ANSI_BLINK
 * @brief Blink text (slow blink).
 */

/**
 * @def ANSI_REVERSE
 * @brief Reverse foreground/background.
 */

/**
 * @def ANSI_HIDDEN
 * @brief Hidden/obscured text.
 */

/**
 * @def ANSI_STRIKETHROUGH
 * @brief Strikethrough text.
 */

/**
 * @def ANSI_DOUBLE_UNDER
 * @brief Double underline.
 */

/**
 * @def ANSI_OVERLINE
 * @brief Overlined text.
 */

/** @} */ /* end of ansi_styles (name) */

/** @} */ /* end of ansi_styles */

/**
 * @defgroup ansi_extended Extended colors
 * @ingroup blop_ansi
 * @brief 256-color and 24-bit RGB sequences.
 * @{
 */

/**
 * @name Extended colors
 * @brief 256-color and 24-bit RGB sequences.
 * @{
 */

/**
 * @def ANSI_FG_256(n)
 * @brief Foreground 256-color mode.
 * @param n Color index (0–255). Must be a literal numeric token.
 */

/**
 * ANSI_BG_256(n)
 * @brief Background 256-color mode.
 * @param n Color index (0–255). Must be a literal numeric token.
 */

/**
 * @def ANSI_FG_RGB(r, g, b)
 * @brief True-color (RGB) foreground.
 * @param r Red   (0-255).
 * @param g Green (0-255).
 * @param b Blue  (0-255).
 */

/**
 * @def ANSI_BG_RGB(r, g, b)
 * @brief True-color (RGB) background.
 * @param r Red   (0-255).
 * @param g Green (0-255).
 * @param b Blue  (0-255).
 */

/** @} */ /* end of ansi_extend (name) */

/** @} */ /* end of ansi_extend */

/**
 * @defgroup ansi_cursor Cursor
 * @ingroup blop_ansi
 * @brief Cursor positioning and visibility ANSI sequences.
 * @{
 */

/**
 * @name Cursor
 * @brief Cursor positioning and visibility ANSI sequences.
 * @{
 */

/**
 * @def ANSI_CURSOR_UP(n)
 * @brief Move cursor up by n lines.
 * @param n Number of lines (literal).
 */

/**
 * @def ANSI_CURSOR_DOWN(n)
 * @brief Move cursor down by n lines.
 * @param n Number of lines (literal).
 */

/**
 * @def ANSI_CURSOR_FORWARD(n)
 * @brief Move cursor forward by n columns.
 * @param n Number of columns (literal).
 */

/**
 * @def ANSI_CURSOR_BACK(n)
 * @brief Move cursor backward by n columns.
 * @param n Number of columns (literal).
 */

/**
 * @def ANSI_CURSOR_POS(x, y)
 * @brief Absolute cursor positioning (x column, y row).
 * @param x Column number (1-based).
 * @param y Row number (1-based).
 */

/**
 * @def ANSI_CURSOR_SAVE
 * @brief Save current cursor position.
 */

/**
 * @def ANSI_CURSOR_RESTORE
 * @brief Restore saved cursor position.
 */

/**
 * @def ANSI_CURSOR_HIDE
 * @brief Hide cursor.
 */

/**
 * @def ANSI_CURSOR_SHOW
 * @brief Show cursor.
 */

/** @} */ /* end of ansi_cursor (name) */

/** @} */ /* end of ansi_cursor */

/**
 * @defgroup ansi_screen Screen/line clearing
 * @ingroup blop_ansi
 * @brief Commands for clearing the terminal display.
 * @{
 */

/**
 * @name Screen/line clearing
 * @brief Commands for clearing the terminal display.
 * @{
 */

/** 
 * @def ANSI_CLEAR_SCREEN
 * @brief Clear entire screen (2J). 
 */

/** 
 * @def ANSI_CLEAR_LINE
 * @brief Clear entire current line (2K). 
 */

/** 
 * @def ANSI_CLEAR_TO_END
 * @brief Clear screen from cursor to end (0J). 
 */

/** 
 * @def ANSI_CLEAR_TO_START
 * @brief Clear screen from cursor to beginning (1J). 
 */

/** 
 * @def ANSI_CLEAR_LINE_END
 * @brief Clear line from cursor to end (0K). 
 */

/** 
 * @def ANSI_CLEAR_LINE_START
 * @brief Clear line from cursor to start (1K). 
 */
 
/** @} */ /* end of ansi_screen (name) */

/** @} */ /* end of ansi_screen */

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