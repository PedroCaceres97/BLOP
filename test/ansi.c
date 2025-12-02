#include <blop/ansi.h>

#include <stdio.h>

int main(int argc, char **argv) {

    printf(ANSI_COLOR(ANSI_BOLD ANSI_FG_RED, "PATH: ") ANSI_BOLD ANSI_ITALIC ANSI_FG_GREEN "%s\n" ANSI_RESET, argv[0]);

    if (argc == 1 || argc > 5) {
        printf(ANSI_BOLD ANSI_UNDERLINE ANSI_FG_MAGENTA "No second argument provided.\n" ANSI_RESET);
    } else if (argc == 2) {
        printf(ANSI_COLOR(ANSI_FG_RGB(255, 120, 200), "%s\n"), argv[1]);
    }
        
    return 0;
}