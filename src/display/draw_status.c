/*
** Cz PROJECT, 2026
** draw_status
** File description:
** status bar rendering
*/

#include "zex.h"

static void format_status_left(editor_t *ed, char *buf, int size)
{
    snprintf(buf, size, " %s%s  |  Ln %d/%d  Col %d  [%s]",
        ed->filename ? ed->filename : "[No File]",
        ed->modified ? " [+]" : "",
        ed->top_line + ed->y + 1,
        ed->num_lines,
        ed->x + 1,
        (ed->has_tree && ed->focus == FOCUS_TREE) ? "TREE" : "EDIT");
}

static void draw_status_hint(int status_len)
{
    char hint[] = "^X^S save  ^Q quit  ^W focus";
    int hint_col = COLS - (int)strlen(hint) - 1;

    if (hint_col > status_len)
        mvprintw(LINES - 1, hint_col, "%s", hint);
}

void draw_status_bar(editor_t *ed)
{
    char status[512];

    attron(COLOR_PAIR(ZP_STATUS) | A_BOLD);
    move(LINES - 1, 0);
    clrtoeol();
    format_status_left(ed, status, sizeof(status));
    mvprintw(LINES - 1, 0, "%s", status);
    draw_status_hint(strlen(status));
    attroff(COLOR_PAIR(ZP_STATUS) | A_BOLD);
}
