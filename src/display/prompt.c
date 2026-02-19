/*
** EPITECH PROJECT, 2026
** ~/epitech/free-project/Zex/src
** File description:
** prompt
*/

#include "zex.h"

void handle_choice(editor_t *ed, int *choice)
{
    int ch = 0;

    while (1) {
        ch = getch();
        if (ch == 'y' || ch == 'Y' || ch == KEY_ENTER || ch == 10) {
            *choice = 1;
            break;
        } else if (ch == 'n' || ch == 'N' || ch == 27) {
            *choice = 0;
            break;
        }
    }
}

int prompt(const char *message, editor_t *ed)
{
    int width = strlen(message) + 4;
    int startx = (COLS - width) / 2;
    int starty = LINES / 2;
    WINDOW *win = newwin(3, width, starty, startx);
    int choice = 0;

    box(win, 0, 0);
    keypad(win, TRUE);
    mvwprintw(win, 1, 2, "%s", message);
    wrefresh(win);
    handle_choice(ed, &choice);
    delwin(win);
    display_all(ed);
    return choice;
}
