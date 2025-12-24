#define BLOP_LOG_SHORT
#define BLOP_LOG_COLOURED
#include <blop/blop.h>

enum BLOP {
  hola = 2
};

void logctx(const char* buffer) {
  LOGCTX(buffer);
  LOG_DEBUGCTX(buffer);
  LOG_SUCCESSCTX(buffer);
  LOG_WARNINGCTX(buffer);
  LOG_ERRORCTX(buffer);
  LOG_FATALCTX(buffer);
}

int main(int argc, char **argv) {
  BLOP_ANSI_ENABLE();

  char buffer[256] = {0};
  snprintf(buffer, 256, "Path = %s", argv[0]);

  LOG(buffer);
  LOG_DEBUG(buffer);
  LOG_SUCCESS(buffer);
  LOG_WARNING(buffer);
  LOG_ERROR(buffer);
  LOG_FATAL(buffer);

  logctx(buffer);
      
  BLOP_ANSI_DISABLE();
  return 0;
}