/*
** Cz PROJECT, 2026
** tree
** File description:
** tree init, expand and open-file logic
*/

#include "zex.h"

void tree_load_init(editor_t *ed)
{
    tree_load(ed, ed->tree_root_path, -1, 0);
}

void tree_expand(editor_t *ed, int idx)
{
    if (idx < 0 || idx >= ed->tree_count)
        return;
    if (!ed->tree_entries[idx]->is_dir)
        return;
    ed->tree_entries[idx]->expanded = 1;
    tree_load(ed, ed->tree_entries[idx]->path, idx,
        ed->tree_entries[idx]->depth + 1);
}

void tree_handle_enter(editor_t *ed)
{
    tree_entry_t *entry;

    if (ed->tree_count == 0)
        return;
    entry = ed->tree_entries[ed->tree_selected];
    if (entry->is_dir) {
        if (entry->expanded)
            tree_collapse(ed, ed->tree_selected);
        else
            tree_expand(ed, ed->tree_selected);
    } else {
        editor_open_file(ed, entry->path);
        ed->focus = FOCUS_EDITOR;
    }
}
