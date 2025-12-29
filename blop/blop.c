#include <blop/blop.h>

/* --------------------------------------------------------------------------
 * ANSI SYSTEM
 * -------------------------------------------------------------------------- */

#ifdef BLOP_WINDOWS
  #include <windows.h>

  void blop_impl_ansi_enable() {
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    BLOP_INTERNAL_ASSERT_FORCED(err != INVALID_HANDLE_VALUE, "Failed to get stderr handle (windows.h)");
    BLOP_INTERNAL_ASSERT_FORCED(out != INVALID_HANDLE_VALUE, "Failed to get stdout handle (windows.h)");

    DWORD err_mode = 0;
    DWORD out_mode = 0;
    BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(err, &err_mode), "Failed to get console err_mode (windows.h)");
    BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(out, &out_mode), "Failed to get console out_mode (windows.h)");

    if (!(err_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
      DWORD err_nmode = err_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
      BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(err, err_nmode) != 0, "Failed to set console err_mode (windows.h)");
    }

    if (!(out_mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
      DWORD out_nmode = out_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
      BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(out, out_nmode) != 0, "Failed to set console out_mode (windows.h)");
    }
  }
  void blop_impl_ansi_disable() {
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    BLOP_INTERNAL_ASSERT_FORCED(err != INVALID_HANDLE_VALUE, "Failed to get std handle (windows.h)");
    BLOP_INTERNAL_ASSERT_FORCED(out != INVALID_HANDLE_VALUE, "Failed to get std handle (windows.h)");

    DWORD err_mode = 0;
    DWORD out_mode = 0;
    BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(err, &err_mode), "Failed to get console mode (windows.h)");
    BLOP_INTERNAL_ASSERT_FORCED(GetConsoleMode(out, &out_mode), "Failed to get console mode (windows.h)");

    DWORD err_nmode = err_mode & ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    DWORD out_nmode = out_mode & ~(ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    
    BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(err, err_nmode) != 0, "Failed to set console mode (windows.h)");
    BLOP_INTERNAL_ASSERT_FORCED(SetConsoleMode(out, out_nmode) != 0, "Failed to set console mode (windows.h)");
  }
#endif

char* blop_ansi_fg_256(char* buffer, uint8_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "38;5;%hhum", n);
  return buffer;
}
char* blop_ansi_bg_256(char* buffer, uint8_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "48;5;%hhum", n);
  return buffer;
}
char* blop_ansi_fg_rgb(char* buffer, uint8_t r, uint8_t g, uint8_t b) {
  sprintf(buffer, BLOP_ANSI_ESC "38;2;%hhu;%hhu;%hhum", r, g, b);
  return buffer;
}
char* blop_ansi_bg_rgb(char* buffer, uint8_t r, uint8_t g, uint8_t b) {
  sprintf(buffer, BLOP_ANSI_ESC "48;2;%hhu;%hhu;%hhum", r, g, b);
  return buffer;
}

char* blop_ansi_cursor_up(char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huA", n);
  return buffer;
}
char* blop_ansi_cursor_down(char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huB", n);
  return buffer;
}
char* blop_ansi_cursor_forward(char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huC", n);
  return buffer;
}
char* blop_ansi_cursor_back(char* buffer, uint16_t n) {
  sprintf(buffer, BLOP_ANSI_ESC "%huD", n);
  return buffer;
}
char* blop_ansi_cursor_pos(char* buffer, uint16_t x, uint16_t y) {
  sprintf(buffer, BLOP_ANSI_ESC "%hu;%huH", y, x);
  return buffer;
}