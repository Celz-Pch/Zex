/*
** Cz PROJECT, 2026
** draw_tree
** File description:
** tree panel rendering
*/

#include "zex.h"

static int continues_at_depth(editor_t *ed, int idx, int d)
{
    int i;

    for (i = idx + 1; i < ed->tree_count; i++) {
        if (ed->tree_entries[i]->depth < d)
            return 0;
        if (ed->tree_entries[i]->depth == d)
            return 1;
    }
    return 0;
}

static void draw_tree_header(editor_t *ed)
{
    const char *root = ed->tree_root_path;
    const char *name;
    char buf[TREE_WIDTH + 1];

    name = strrchr(root, '/');
    name = (name && name[1]) ? name + 1 : root;
    snprintf(buf, sizeof(buf), " %s", name);
    wattron(ed->tree_win, COLOR_PAIR(ZP_TREE_HDR) | A_BOLD);
    mvwprintw(ed->tree_win, 0, 0, "%-*s", TREE_WIDTH, buf);
    wattroff(ed->tree_win, COLOR_PAIR(ZP_TREE_HDR) | A_BOLD);
}

static void draw_connectors(editor_t *ed, int idx)
{
    tree_entry_t *entry = ed->tree_entries[idx];
    int d = entry->depth;
    int selected = (idx == ed->tree_selected);
    int conn_attr;
    int l;

    conn_attr = selected ? (COLOR_PAIR(ZP_TREE_SEL) | A_BOLD)
                         : (COLOR_PAIR(ZP_TREE_CONN) | A_DIM);
    wattron(ed->tree_win, conn_attr);
    waddch(ed->tree_win, ' ');
    for (l = 0; l < d; l++) {
        waddch(ed->tree_win, continues_at_depth(ed, idx, l) ? ACS_VLINE : ' ');
        waddch(ed->tree_win, ' ');
        waddch(ed->tree_win, ' ');
        waddch(ed->tree_win, ' ');
    }
    waddch(ed->tree_win, continues_at_depth(ed, idx, d) ? ACS_LTEE : ACS_LLCORNER);
    waddch(ed->tree_win, ACS_HLINE);
    waddch(ed->tree_win, ACS_HLINE);
    waddch(ed->tree_win, ' ');
    wattroff(ed->tree_win, conn_attr);
}

static void draw_entry_name(editor_t *ed, int idx)
{
    tree_entry_t *entry = ed->tree_entries[idx];
    int selected = (idx == ed->tree_selected);
    int is_open = (ed->filename && entry->path
        && strcmp(ed->filename, entry->path) == 0);
    int d = entry->depth;
    int overhead = 1 + (d + 1) * 4 + 2;
    int remaining = TREE_WIDTH - overhead;
    int nlen;
    int attr;

    if (selected)
        attr = COLOR_PAIR(ZP_TREE_SEL) | A_BOLD;
    else if (entry->is_dir)
        attr = COLOR_PAIR(ZP_TREE_DIR) | A_BOLD;
    else if (is_open)
        attr = COLOR_PAIR(ZP_TREE_OPEN) | A_BOLD;
    else
        attr = COLOR_PAIR(ZP_NORMAL);
    wattron(ed->tree_win, attr);
    waddstr(ed->tree_win, entry->is_dir
        ? (entry->expanded ? "▾ " : "▸ ")
        : "  ");
    if (remaining > 0) {
        nlen = (int)strlen(entry->name);
        if (nlen > remaining)
            waddnstr(ed->tree_win, entry->name, remaining);
        else
            waddstr(ed->tree_win, entry->name);
    }
    wattroff(ed->tree_win, attr);
}

static void draw_tree_entry(editor_t *ed, int row, int idx)
{
    int selected = (idx == ed->tree_selected);

    wmove(ed->tree_win, row, 0);
    if (selected) {
        wattron(ed->tree_win, COLOR_PAIR(ZP_TREE_SEL) | A_BOLD);
        mvwhline(ed->tree_win, row, 0, ' ', TREE_WIDTH);
        wattroff(ed->tree_win, COLOR_PAIR(ZP_TREE_SEL) | A_BOLD);
        wmove(ed->tree_win, row, 0);
    }
    draw_connectors(ed, idx);
    draw_entry_name(ed, idx);
}

static void draw_tree_body(editor_t *ed)
{
    int visible = LINES - 2;
    int i;

    for (i = 0; i < visible && ed->tree_top + i < ed->tree_count; i++)
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
