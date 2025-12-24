#define BLOP_ANSI_SHORT
#include <blop/blop.h>

#include <stdio.h>

int main(int argc, char **argv) {

    printf(ANSI_COLOR(ANSI_BOLD ANSI_FG_RED, "PATH: ") ANSI_BOLD ANSI_ITALIC ANSI_FG_GREEN "%s\n" ANSI_RESET, argv[0]);

    if (argc == 1 || argc > 5) {
        printf(ANSI_BOLD ANSI_UNDERLINE ANSI_FG_MAGENTA "No second argument provided.\n" ANSI_RESET);
    } else if (argc == 2) {
        printf(ANSI_COLOR(ANSI_FG_RGB(0,    200,    255),   "%s\n"), argv[1]);
        printf(ANSI_COLOR(ANSI_FG_RGB(50,   100,    200),   "%s\n"), argv[1]);
        printf(ANSI_COLOR(ANSI_FG_RGB(100,  0,      150),   "%s\n"), argv[1]);
        printf(ANSI_COLOR(ANSI_FG_RGB(150,  0,      100),   "%s\n"), argv[1]);
        printf(ANSI_COLOR(ANSI_FG_RGB(200,  100,    50),    "%s\n"), argv[1]);
        printf(ANSI_COLOR(ANSI_FG_RGB(255,  200,    0),     "%s\n"), argv[1]);
    }
        
    return 0;
}