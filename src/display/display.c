/*
** Cz PROJECT, 2026
** display
** File description:
** main display orchestration and color init
*/

#include "zex.h"

void init_colors(void)
{
    if (!has_colors())
        return;
    use_default_colors();
    init_pair(ZP_NORMAL,   COLOR_WHITE,   COLOR_BLACK);
    init_pair(ZP_KEYWORD,  COLOR_CYAN,    COLOR_BLACK);
    init_pair(ZP_STRING,   COLOR_GREEN,   COLOR_BLACK);
    init_pair(ZP_COMMENT,  COLOR_WHITE,   COLOR_BLACK);
    init_pair(ZP_NUMBER,   COLOR_YELLOW,  COLOR_BLACK);
    init_pair(ZP_PREPROC,  COLOR_MAGENTA, COLOR_BLACK);
    init_pair(ZP_TREE_DIR, COLOR_BLUE,    COLOR_BLACK);
    init_pair(ZP_TREE_SEL, COLOR_BLACK,   COLOR_CYAN);
    init_pair(ZP_STATUS,   COLOR_BLACK,   COLOR_WHITE);
    init_pair(ZP_TREE_HDR, COLOR_WHITE,   COLOR_BLUE);
    bkgd(COLOR_PAIR(ZP_NORMAL));
}

void display_content(editor_t *ed)
{
    WINDOW *win = ed->edit_win;
    int visible = LINES - 1;
    int edit_cols = getmaxx(win);

    werase(win);
    wbkgd(win, COLOR_PAIR(ZP_NORMAL));
    for (int row = 0; row < visible; row++) {
        int line_idx = ed->top_line + row;
        if (line_idx < ed->num_lines)
            highlight_line(win, row, ed->lines[line_idx], edit_cols);
        else {
            wattron(win, COLOR_PAIR(ZP_COMMENT) | A_DIM);
            mvwaddch(win, row, 0, '~');
            wattroff(win, COLOR_PAIR(ZP_COMMENT) | A_DIM);
        }
    }
}

void display_all(editor_t *ed)
{
    if (ed->has_tree) {
        attron(COLOR_PAIR(ZP_COMMENT) | A_DIM);
        mvvline(0, TREE_WIDTH, ACS_VLINE, LINES - 1);
        attroff(COLOR_PAIR(ZP_COMMENT) | A_DIM);
    }
    draw_status_bar(ed);
    wnoutrefresh(stdscr);
    if (ed->has_tree)
        draw_tree(ed);
    display_content(ed);
    wmove(ed->edit_win, ed->y, ed->x);
    wnoutrefresh(ed->edit_win);
    doupdate();
}
