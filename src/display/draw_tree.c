/*
** Cz PROJECT, 2026
** draw_tree
** File description:
** tree panel rendering
*/

#include "zex.h"

static void draw_tree_header(editor_t *ed)
{
    wattron(ed->tree_win, COLOR_PAIR(ZP_TREE_HDR) | A_BOLD);
    mvwprintw(ed->tree_win, 0, 0, "%-*s", TREE_WIDTH, " EXPLORER");
    wattroff(ed->tree_win, COLOR_PAIR(ZP_TREE_HDR) | A_BOLD);
}

static void format_entry_display(tree_entry_t *entry, char *display, int size)
{
    int indent = entry->depth * 2;

    memset(display, ' ', indent);
    display[indent] = '\0';
    if (entry->is_dir)
        snprintf(display + indent, size - indent,
            "%s %s/", entry->expanded ? "v" : ">", entry->name);
    else
        snprintf(display + indent, size - indent, "  %s", entry->name);
}

static void apply_entry_color(editor_t *ed, int idx, int row,
    const char *display)
{
    tree_entry_t *entry = ed->tree_entries[idx];

    if (idx == ed->tree_selected) {
        wattron(ed->tree_win, COLOR_PAIR(ZP_TREE_SEL) | A_BOLD);
        mvwprintw(ed->tree_win, row, 0, "%-*s", TREE_WIDTH, display);
        wattroff(ed->tree_win, COLOR_PAIR(ZP_TREE_SEL) | A_BOLD);
    } else if (entry->is_dir) {
        wattron(ed->tree_win, COLOR_PAIR(ZP_TREE_DIR) | A_BOLD);
        mvwprintw(ed->tree_win, row, 0, "%-*s", TREE_WIDTH, display);
        wattroff(ed->tree_win, COLOR_PAIR(ZP_TREE_DIR) | A_BOLD);
    } else if (ed->filename && strcmp(ed->filename, entry->path) == 0) {
        wattron(ed->tree_win, COLOR_PAIR(ZP_NORMAL) | A_UNDERLINE);
        mvwprintw(ed->tree_win, row, 0, "%-*s", TREE_WIDTH, display);
        wattroff(ed->tree_win, COLOR_PAIR(ZP_NORMAL) | A_UNDERLINE);
    } else {
        wattron(ed->tree_win, COLOR_PAIR(ZP_NORMAL));
        mvwprintw(ed->tree_win, row, 0, "%-*s", TREE_WIDTH, display);
        wattroff(ed->tree_win, COLOR_PAIR(ZP_NORMAL));
    }
}

static void draw_tree_entry(editor_t *ed, int row, int idx)
{
    char display[TREE_WIDTH + 1];

    format_entry_display(ed->tree_entries[idx], display, sizeof(display));
    apply_entry_color(ed, idx, row, display);
}

static void draw_tree_body(editor_t *ed)
{
    int visible = LINES - 2;

    for (int i = 0; i < visible && ed->tree_top + i < ed->tree_count; i++)
        draw_tree_entry(ed, i + 1, ed->tree_top + i);
}

void draw_tree(editor_t *ed)
{
    if (!ed->has_tree || !ed->tree_win)
        return;
    werase(ed->tree_win);
    wbkgd(ed->tree_win, COLOR_PAIR(ZP_NORMAL));
    draw_tree_header(ed);
    draw_tree_body(ed);
    wnoutrefresh(ed->tree_win);
}
