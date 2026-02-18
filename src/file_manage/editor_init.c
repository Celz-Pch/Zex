/*
** Cz PROJECT, 2026
** editor_init
** File description:
** editor struct initialization helpers
*/

#include "zex.h"

void init_editor_windows(editor_t *ed, char *dir_path)
{
    ed->has_tree = dir_path ? 1 : 0;
    if (ed->has_tree) {
        ed->tree_root_path = strdup(dir_path);
        ed->tree_win = newwin(LINES - 1, TREE_WIDTH, 0, 0);
        ed->edit_win = newwin(LINES - 1, COLS - TREE_WIDTH - 1,
            0, TREE_WIDTH + 1);
        keypad(ed->tree_win, TRUE);
    } else {
        ed->edit_win = newwin(LINES - 1, COLS, 0, 0);
    }
    keypad(ed->edit_win, TRUE);
}

void init_editor_tree_state(editor_t *ed, char *dir_path)
{
    if (!dir_path)
        return;
    ed->focus = FOCUS_TREE;
    tree_load_init(ed);
}

void init_editor_content(editor_t *ed, char *file_path)
{
    ed->lines = malloc(sizeof(char *));
    ed->lines[0] = strdup("");
    ed->num_lines = 1;
    if (file_path) {
        ed->filename = strdup(file_path);
        load_file(ed, file_path);
    }
}
