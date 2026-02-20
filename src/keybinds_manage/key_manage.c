/*
** Cz PROJECT, 2026
** key_manage
** File description:
** character, backspace and enter key handlers
*/

#include "zex.h"

int manage_char(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;

    if (ed->ch < 32 || ed->ch > 126)
        return 0;
    ed->lines[abs_line] = line_insert_char(ed->lines[abs_line],
        ed->x, ed->ch);
    ed->x++;
    ed->modified = 1;
    display_all(ed);
    return 1;
}

int manage_backspace(editor_t *ed)
{
    if (!(ed->ch == KEY_BACKSPACE || ed->ch == 263
        || ed->ch == 127 || ed->ch == 8))
        return 0;
    if (ed->x > 0)
        backspace_in_line(ed);
    else if (ed->top_line + ed->y > 0)
        backspace_merge(ed);
    display_all(ed);
    return 1;
}

int manage_enter(editor_t *ed)
{
    if (!(ed->ch == 10 || ed->ch == KEY_ENTER))
        return 0;
    enter_split_line(ed);
    enter_move_cursor(ed);
    display_all(ed);
    return 1;
}

int manage_tab(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;

    if (ed->ch != 9)
        return 0;
    if (completion_apply(ed)) {
        display_all(ed);
        return 1;
    }
    for (int i = 0; i < 4; i++) {
        ed->lines[abs_line] = line_insert_char(ed->lines[abs_line],
            ed->x, ' ');
        ed->x++;
    }
    ed->modified = 1;
    display_all(ed);
    return 1;
}
