/*
** EPITECH PROJECT, 2026
** zex
** File description:
** zex
*/

#include <ncurses.h>
#include <stdlib.h>

#ifndef ZEX_H
    #define ZEX_H

typedef struct coords_s {
    int x;
    int y;
    int ch;
} coords_t;

int arrow_manager(coords_t *stock);
int manage_backspace(coords_t *stock);
int manage_enter(coords_t *stock);
void main_loop(void);
#endif
