/*
** EPITECH PROJECT, 2026
** main_loop
** File description:
** main
*/

#include "zex.h"

coords_t *init_struct(void)
{
    coords_t *stock = malloc(sizeof(coords_t));

    stock->x = 0;
    stock->y = 0;
    stock->ch = 0;
    return stock;
}

void main_loop(void)
{
    coords_t *stock = init_struct();

    while ((stock->ch = getch()) != 'q') {
        if (handle_backspace(stock))
            continue;
        if (handle_enter(stock))
            continue;
        if (arrow_manager(stock))
            continue;

        move(stock->y, stock->x);
        addch(stock->ch);
        refresh();
        stock->x++;
        if (stock->x >= COLS) {
            stock->x = 0;
            stock->y++;
        }
    }
}
