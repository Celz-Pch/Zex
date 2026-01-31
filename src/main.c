/*
** EPITECH PROJECT, 2026
** main
** File description:
** main
*/

#include "zex.h"

int main(int argc, char **argv)
{
    if (argc != 2)
        return 84;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    main_loop();
    endwin();
    return 0;
}
