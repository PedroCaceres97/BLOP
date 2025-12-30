#define LOG_COLOURED
#include <blop/blop.h>

int main(int argc, char **argv) {
  ANSI_ENABLE();

  char buffer[256] = {0};
  snprintf(buffer, 256, "Path = %s", argv[0]);

  LOG(buffer);
  LOG_DEBUG(buffer);
  LOG_SUCCESS(buffer);
  LOG_WARNING(buffer);
  LOG_ERROR(buffer);
  LOG_FATAL(buffer);

  LOGF("Pathetic: \"%s\"", buffer);
  LOG_DEBUGF("Pathetic: \"%s\"", buffer);
  LOG_SUCCESSF("Pathetic: \"%s\"", buffer);
  LOG_WARNINGF("Pathetic: \"%s\"", buffer);
  LOG_ERRORF("Pathetic: \"%s\"", buffer);
  LOG_FATALF("Pathetic: \"%s\"", buffer);
      
  ANSI_DISABLE();
  return 0;
}