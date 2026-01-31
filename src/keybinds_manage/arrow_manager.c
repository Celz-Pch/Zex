/*
** EPITECH PROJECT, 2026
** key_manager
** File description:
** key_manager
*/

#include "zex.h"

int arrow_manager(coords_t *stock)
{
    if (stock->ch == KEY_LEFT) {
        if (stock->x > 0) stock->x--;
        move(stock->y, stock->x);
        refresh();
        return 1;
    } else if (stock->ch == KEY_RIGHT) {
        if (stock->x < COLS - 1) stock->x++;
        move(stock->y, stock->x);
        refresh();
        return 1;
    } else if (stock->ch == KEY_UP) {
        if (stock->y > 0) stock->y--;
        move(stock->y, stock->x);
        refresh();
        return 1;
    } else if (stock->ch == KEY_DOWN) {
        stock->y++;
        move(stock->y, stock->x);
        refresh();
        return 1;
    }
    return 0;
}
