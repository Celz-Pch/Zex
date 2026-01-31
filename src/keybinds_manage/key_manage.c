/*
** EPITECH PROJECT, 2026
** key_handlers
** File description:
** backspace and enter handlers
*/

#include "zex.h"

int manage_backspace(coords_t *stock)
{
    if (!(stock->ch == KEY_BACKSPACE || stock->ch == 263 || stock->ch == 127 || stock->ch == 8))
        return 0;
    if (stock->x > 0) {
        stock->x--;
        move(stock->y, stock->x);
        delch();
        refresh();
    } else if (stock->y > 0) {
        stock->y--;
        stock->x = COLS > 0 ? COLS - 1 : 0;
        move(stock->y, stock->x);
        delch();
        refresh();
    }
    return 1;
}

int manage_enter(coords_t *stock)
{
    if (!(stock->ch == 10 || stock->ch == KEY_ENTER))
        return 0;
    stock->y++;
    stock->x = 0;
    move(stock->y, stock->x);
    refresh();
    return 1;
}
