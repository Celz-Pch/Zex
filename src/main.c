/*
** Cz PROJECT, 2026
** main
** File description:
** main
*/

#include "zex.h"

static void setup_ncurses(void)
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    init_colors();
}

static editor_t *open_editor(char *path)
{
    struct stat st;

    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
        return init_editor(NULL, path);
    return init_editor(path, NULL);
}

int main(int argc, char **argv)
{
    editor_t *ed;

    if (argc != 2)
        return 84;
    setlocale(LC_ALL, "");
    setup_ncurses();
    ed = open_editor(argv[1]);
    if (!ed) {
        endwin();
        return 84;
    }
    main_loop(ed);
    free_editor(ed);
    endwin();
    return 0;
}