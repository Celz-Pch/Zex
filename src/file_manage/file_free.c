/*
** Cz PROJECT, 2026
** file_free
** File description:
** editor memory cleanup
*/

#include "zex.h"

static void free_tree_entries(editor_t *ed)
{
    for (int i = 0; i < ed->tree_count; i++) {
        free(ed->tree_entries[i]->name);
        free(ed->tree_entries[i]->path);
        free(ed->tree_entries[i]);
    }
    free(ed->tree_entries);
}

void free_editor(editor_t *ed)
{
    if (!ed)
        return;
    for (int i = 0; i < ed->num_lines; i++)
        free(ed->lines[i]);
    free(ed->lines);
    free(ed->filename);
    free(ed->tree_root_path);
    free_tree_entries(ed);
    if (ed->tree_win)
        delwin(ed->tree_win);
    if (ed->edit_win)
        delwin(ed->edit_win);
    free(ed);
}
