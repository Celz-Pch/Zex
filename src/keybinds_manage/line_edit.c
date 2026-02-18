/*
** Cz PROJECT, 2026
** line_edit
** File description:
** low-level line content editing operations
*/

#include "zex.h"

char *line_insert_char(char *line, int pos, int ch)
{
    int len = strlen(line);
    char *new_line = malloc(len + 2);

    if (!new_line)
        return line;
    memcpy(new_line, line, pos);
    new_line[pos] = (char)ch;
    memcpy(new_line + pos + 1, line + pos, len - pos + 1);
    free(line);
    return new_line;
}

void line_delete_char(char *line, int pos)
{
    int len = strlen(line);

    if (pos < 0 || pos >= len)
        return;
    memmove(line + pos, line + pos + 1, len - pos);
}

void backspace_in_line(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;

    ed->x--;
    line_delete_char(ed->lines[abs_line], ed->x);
    ed->modified = 1;
}

void backspace_merge(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;
    int prev_len = strlen(ed->lines[abs_line - 1]);
    int curr_len = strlen(ed->lines[abs_line]);
    char *merged = malloc(prev_len + curr_len + 1);

    if (merged) {
        memcpy(merged, ed->lines[abs_line - 1], prev_len);
        memcpy(merged + prev_len, ed->lines[abs_line], curr_len + 1);
        free(ed->lines[abs_line - 1]);
        free(ed->lines[abs_line]);
        ed->lines[abs_line - 1] = merged;
    }
    for (int i = abs_line; i < ed->num_lines - 1; i++)
        ed->lines[i] = ed->lines[i + 1];
    ed->num_lines--;
    if (ed->y > 0)
        ed->y--;
    else
        ed->top_line--;
    ed->x = prev_len;
    ed->modified = 1;
}

char *enter_split_line(editor_t *ed)
{
    int abs_line = ed->top_line + ed->y;
    char *curr = ed->lines[abs_line];
    char *new_line = strdup(curr + ed->x);

    curr[ed->x] = '\0';
    ed->lines = realloc(ed->lines, (ed->num_lines + 1) * sizeof(char *));
    for (int i = ed->num_lines; i > abs_line + 1; i--)
        ed->lines[i] = ed->lines[i - 1];
    ed->lines[abs_line + 1] = new_line;
    ed->num_lines++;
    return new_line;
}

void enter_move_cursor(editor_t *ed)
{
    ed->x = 0;
    if (ed->y < LINES - 2)
        ed->y++;
    else
        ed->top_line++;
    ed->modified = 1;
}
