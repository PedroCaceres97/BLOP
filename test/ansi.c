#include <blop/ansi.h>

#include <stdio.h>

int main(int argc, char **argv) {
    printf(ANSI_COLOR_TEXT(ANSI_BOLD ANSI_FG_RED, "PATH: ") ANSI_COLOR_SET(ANSI_BOLD ANSI_ITALIC ANSI_FG_GREEN) "%s\n" ANSI_RESET, argv[0]);

    if (argc > 1)
        printf(ANSI_COLOR_SET(ANSI_BOLD ANSI_UNDERLINE ANSI_FG_MAGENTA) "%s\n" ANSI_RESET, argv[1]);
    else
        printf(ANSI_COLOR_SET(ANSI_BOLD ANSI_UNDERLINE ANSI_FG_MAGENTA) "No second argument provided.\n" ANSI_RESET);
    return 0;
}