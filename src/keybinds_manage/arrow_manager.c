/*
** Cz PROJECT, 2026
** arrow_manager
** File description:
** arrow key dispatcher
*/

#include "zex.h"

int arrow_manager(editor_t *ed)
{
    if (ed->ch == KEY_LEFT)
        return arrow_left(ed);
    if (ed->ch == KEY_RIGHT)
        return arrow_right(ed);
    if (ed->ch == KEY_UP)
        return arrow_up(ed);
    if (ed->ch == KEY_DOWN)
        return arrow_down(ed);
    return 0;
}
