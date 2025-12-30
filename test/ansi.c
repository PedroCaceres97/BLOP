#include <blop/blop.h>

int main(int argc, char **argv) {

    printf(ANSI_COLOR(ANSI_BOLD ANSI_FG_RED, "PATH: ") ANSI_BOLD ANSI_ITALIC ANSI_FG_GREEN "%s\n" ANSI_RESET, argv[0]);

    if (argc == 1 || argc > 5) {
        printf(ANSI_BOLD ANSI_UNDERLINE ANSI_FG_MAGENTA "No text or rgb colors provided.\n" ANSI_RESET);
        return 1;
    }

    int r = 255;
    int g = 0;
    int b = 0;
    sscanf(argv[2], "%i", &r);
    sscanf(argv[3], "%i", &g);
    sscanf(argv[4], "%i", &b);

    char buffer[256] = {0};
    printf("%s%s" ANSI_RESET "\n", ansi_fg_rgb(buffer, r, g, b), argv[1]);
        
    return 0;
}