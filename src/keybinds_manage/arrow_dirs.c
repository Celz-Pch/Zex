/*
** Cz PROJECT, 2026
** arrow_dirs
** File description:
** individual arrow direction handlers
*/

#include "zex.h"

int arrow_left(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;

    if (ed->x > 0) {
        ed->x--;
    } else if (abs_line > 0) {
        if (ed->y > 0)
            ed->y--;
        else
            ed->top_line--;
        ed->x = strlen(ed->lines[ed->top_line + ed->y]);
    }
    display_all(ed);
    return 1;
}

int arrow_right(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;
    int line_len = strlen(ed->lines[abs_line]);

    if (ed->x < line_len) {
        ed->x++;
    } else if (abs_line < ed->num_lines - 1) {
        ed->x = 0;
        if (ed->y < LINES - 2)
            ed->y++;
        else
            ed->top_line++;
    }
    display_all(ed);
    return 1;
}

int arrow_up(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;
    int line_len;

    if (abs_line > 0) {
        if (ed->y > 0)
            ed->y--;
        else
            ed->top_line--;
        abs_line--;
        line_len = strlen(ed->lines[abs_line]);
        if (ed->x > line_len)
            ed->x = line_len;
    }
    display_all(ed);
    return 1;
}

int arrow_down(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;
    int line_len;

    if (abs_line < ed->num_lines - 1) {
        if (ed->y < LINES - 2)
            ed->y++;
        else
            ed->top_line++;
        abs_line++;
        line_len = strlen(ed->lines[abs_line]);
        if (ed->x > line_len)
            ed->x = line_len;
    }
    display_all(ed);
    return 1;
}
