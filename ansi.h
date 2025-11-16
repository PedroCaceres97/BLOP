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
 * @author Pedro Caceres
 * @date 2025-11-16
 */

#ifndef __BLOP_ANSI_H__
#define __BLOP_ANSI_H__

/* ============================================================================
 *  Core escape prefix
 * ==========================================================================*/
/**
 * @defgroup ansi_core Core tokens
 * @brief Base constants used internally by other ANSI macros.
 * @{
 */

/**
 * @brief Control Sequence Introducer (CSI).  
 * Most ANSI sequences begin with this prefix.
 */
#define ANSI_ESC  "\x1b["

/** @} */

/* ============================================================================
 *  Colors (foreground/background)
 * ==========================================================================*/
/**
 * @defgroup ansi_colors Color and style helpers
 * @brief Predefined standard ANSI 16-color foreground/background macros.
 * @{
 */

/**
 * @brief Passthrough identity macro for color constants.
 * @param color One of the defined ANSI_* macros.
 */
#define ANSI_COLOR_SET(color) color

/**
 * @brief Expand to color + text + reset.
 * @param color Any ANSI color/style macro.
 * @param text Null-terminated string to colorize.
 */
#define ANSI_COLOR_TEXT(color, text)  color text ANSI_RESET

/* --- Standard 8-color foreground --- */
/** @brief Set foreground to black (30). */
#define ANSI_FG_BLACK           ANSI_ESC "30m"

/** @brief Set foreground to red (31). */
#define ANSI_FG_RED             ANSI_ESC "31m"

/** @brief Set foreground to green (32). */
#define ANSI_FG_GREEN           ANSI_ESC "32m"

/** @brief Set foreground to yellow (33). */
#define ANSI_FG_YELLOW          ANSI_ESC "33m"

/** @brief Set foreground to blue (34). */
#define ANSI_FG_BLUE            ANSI_ESC "34m"

/** @brief Set foreground to magenta (35). */
#define ANSI_FG_MAGENTA         ANSI_ESC "35m"

/** @brief Set foreground to cyan (36). */
#define ANSI_FG_CYAN            ANSI_ESC "36m"

/** @brief Set foreground to white (37). */
#define ANSI_FG_WHITE           ANSI_ESC "37m"


/* --- Standard 8-color background --- */
/** @brief Set background to black (40). */
#define ANSI_BG_BLACK           ANSI_ESC "40m"

/** @brief Set background to red (41). */
#define ANSI_BG_RED             ANSI_ESC "41m"

/** @brief Set background to green (42). */
#define ANSI_BG_GREEN           ANSI_ESC "42m"

/** @brief Set background to yellow (43). */
#define ANSI_BG_YELLOW          ANSI_ESC "43m"

/** @brief Set background to blue (44). */
#define ANSI_BG_BLUE            ANSI_ESC "44m"

/** @brief Set background to magenta (45). */
#define ANSI_BG_MAGENTA         ANSI_ESC "45m"

/** @brief Set background to cyan (46). */
#define ANSI_BG_CYAN            ANSI_ESC "46m"

/** @brief Set background to white (47). */
#define ANSI_BG_WHITE           ANSI_ESC "47m"


/** @} */

/* ============================================================================
 *  Styles and resets
 * ==========================================================================*/
/**
 * @defgroup ansi_styles Text styles and reset
 * @brief Text formatting escape sequences.
 * @{
 */

/** @brief Full device reset (ESC c). Resets terminal state. */
#define ANSI_RESET_FULL         ANSI_ESC "c"

/** @brief Reset all graphics attributes (SGR 0). */
#define ANSI_RESET              ANSI_ESC "0m"

/** @brief Bold text. */
#define ANSI_BOLD               ANSI_ESC "1m"

/** @brief Dim / faint text. */
#define ANSI_DIM                ANSI_ESC "2m"

/** @brief Italic text. */
#define ANSI_ITALIC             ANSI_ESC "3m"

/** @brief Underlined text. */
#define ANSI_UNDERLINE          ANSI_ESC "4m"

/** @brief Blink text (slow blink). */
#define ANSI_BLINK              ANSI_ESC "5m"

/** @brief Reverse foreground/background. */
#define ANSI_REVERSE            ANSI_ESC "7m"

/** @brief Hidden/obscured text. */
#define ANSI_HIDDEN             ANSI_ESC "8m"

/** @brief Strikethrough text. */
#define ANSI_STRIKETHROUGH      ANSI_ESC "9m"

/** @brief Double underline. */
#define ANSI_DOUBLE_UNDER       ANSI_ESC "21m"

/** @brief Overlined text. */
#define ANSI_OVERLINE           ANSI_ESC "53m"

/** @} */

/* ============================================================================
 *  Extended color macros (256-color and RGB)
 * ==========================================================================*/
/**
 * @defgroup ansi_extended Extended color macros
 * @brief 256-color and 24-bit RGB sequences.
 * @{
 */

/**
 * @brief Foreground 256-color mode.
 * @param n Color index (0–255). Must be a literal numeric token.
 */
#define ANSI_FG_256(n)          ANSI_ESC "38;5;" #n "m"

/**
 * @brief Background 256-color mode.
 * @param n Color index (0–255). Must be a literal numeric token.
 */
#define ANSI_BG_256(n)          ANSI_ESC "48;5;" #n "m"

/**
 * @brief True-color (RGB) foreground.
 * @param r Red   (0-255).
 * @param g Green (0-255).
 * @param b Blue  (0-255).
 */
#define ANSI_FG_RGB(r, g, b)    ANSI_ESC "38;2;" #r ";" #g ";" #b "m"

/**
 * @brief True-color (RGB) background.
 * @param r Red   (0-255).
 * @param g Green (0-255).
 * @param b Blue  (0-255).
 */
#define ANSI_BG_RGB(r, g, b)    ANSI_ESC "48;2;" #r ";" #g ";" #b "m"

/** @} */

/* ============================================================================
 *  Cursor movement
 * ==========================================================================*/
/**
 * @defgroup ansi_cursor Cursor movement & visibility
 * @brief Cursor-positioning ANSI sequences.
 * @{
 */

/**
 * @brief Move cursor up by n lines.
 * @param n Number of lines (literal).
 */
#define ANSI_CURSOR_UP(n)       ANSI_ESC #n "A"

/**
 * @brief Move cursor down by n lines.
 * @param n Number of lines (literal).
 */
#define ANSI_CURSOR_DOWN(n)     ANSI_ESC #n "B"

/**
 * @brief Move cursor forward by n columns.
 * @param n Number of columns (literal).
 */
#define ANSI_CURSOR_FORWARD(n)  ANSI_ESC #n "C"

/**
 * @brief Move cursor backward by n columns.
 * @param n Number of columns (literal).
 */
#define ANSI_CURSOR_BACK(n)     ANSI_ESC #n "D"

/**
 * @brief Absolute cursor positioning (x column, y row).
 * @param x Column number (1-based).
 * @param y Row number (1-based).
 */
#define ANSI_CURSOR_POS(x, y)   ANSI_ESC #y ";" #x "H"

/** @brief Save current cursor position. */
#define ANSI_CURSOR_SAVE        ANSI_ESC "s"

/** @brief Restore saved cursor position. */
#define ANSI_CURSOR_RESTORE     ANSI_ESC "u"

/** @brief Hide cursor. */
#define ANSI_CURSOR_HIDE        ANSI_ESC "?25l"

/** @brief Show cursor. */
#define ANSI_CURSOR_SHOW        ANSI_ESC "?25h"

/** @} */

/* ============================================================================
 *  Screen clearing
 * ==========================================================================*/
/**
 * @defgroup ansi_screen Screen/line clearing helpers
 * @brief Commands for clearing the terminal display.
 * @{
 */

/** @brief Clear entire screen (2J). */
#define ANSI_CLEAR_SCREEN       ANSI_ESC "2J"

/** @brief Clear entire current line (2K). */
#define ANSI_CLEAR_LINE         ANSI_ESC "2K"

/** @brief Clear screen from cursor to end (0J). */
#define ANSI_CLEAR_TO_END       ANSI_ESC "0J"

/** @brief Clear screen from cursor to beginning (1J). */
#define ANSI_CLEAR_TO_START     ANSI_ESC "1J"

/** @brief Clear line from cursor to end (0K). */
#define ANSI_CLEAR_LINE_END     ANSI_ESC "0K"

/** @brief Clear line from cursor to start (1K). */
#define ANSI_CLEAR_LINE_START   ANSI_ESC "1K"

/** @} */

#endif /* __BLOP_ANSI_H__ */