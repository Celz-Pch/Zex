/*
** Cz PROJECT, 2026
** main_loop
** File description:
** main editor loop
*/

#include "zex.h"

static void handle_tree_key(editor_t *ed)
{
    int visible = LINES - 2;

    if ((ed->ch == KEY_UP || ed->ch == 'k') && ed->tree_selected > 0) {
        ed->tree_selected--;
        if (ed->tree_selected < ed->tree_top)
            ed->tree_top = ed->tree_selected;
    } else if ((ed->ch == KEY_DOWN || ed->ch == 'j')
        && ed->tree_selected < ed->tree_count - 1) {
        ed->tree_selected++;
        if (ed->tree_selected >= ed->tree_top + visible)
            ed->tree_top = ed->tree_selected - visible + 1;
    } else if (ed->ch == 10 || ed->ch == KEY_ENTER) {
        tree_handle_enter(ed);
    }
}

static int handle_global_keys(editor_t *ed)
{
    if (ed->ch == 17)
        return confirm_exit(ed);
    if (ed->ch == 23 && ed->has_tree) {
        handle_focus_switch(ed);
        return 1;
    }
    if (ed->ch == 24) {
        ed->chord_active = 1;
        return 1;
    }
    if (ed->chord_active) {
        handle_chord(ed);
        return 1;
    }
    return 0;
}

void main_loop(editor_t *ed)
{
    int ret;

    display_all(ed);
    while (1) {
        ed->ch = getch();
        ret = handle_global_keys(ed);
        if (ret == -1)
            break;
        if (ret)
            continue;
        if (ed->has_tree && ed->focus == FOCUS_TREE) {
            handle_tree_key(ed);
            display_all(ed);
        } else {
            handle_editor_input(ed);
        }
    }
}
