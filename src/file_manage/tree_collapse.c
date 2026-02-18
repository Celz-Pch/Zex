/*
** Cz PROJECT, 2026
** tree_collapse
** File description:
** tree directory collapse logic
*/

#include "zex.h"

static int tree_find_last_child(editor_t *ed, int idx)
{
    int target_depth = ed->tree_entries[idx]->depth;
    int last = idx;

    while (last + 1 < ed->tree_count
        && ed->tree_entries[last + 1]->depth > target_depth)
        last++;
    return last;
}

static void tree_free_children(editor_t *ed, int first, int last)
{
    for (int i = first; i <= last; i++) {
        free(ed->tree_entries[i]->name);
        free(ed->tree_entries[i]->path);
        free(ed->tree_entries[i]);
    }
}

static void tree_remove_range(editor_t *ed, int first, int last)
{
    int num_removed = last - first + 1;

    memmove(ed->tree_entries + first,
        ed->tree_entries + last + 1,
        (ed->tree_count - last - 1) * sizeof(tree_entry_t *));
    ed->tree_count -= num_removed;
    if (ed->tree_selected > last)
        ed->tree_selected -= num_removed;
    else if (ed->tree_selected >= first)
        ed->tree_selected = first - 1;
}

void tree_collapse(editor_t *ed, int idx)
{
    int last_child;

    if (idx < 0 || idx >= ed->tree_count
        || !ed->tree_entries[idx]->is_dir)
        return;
    ed->tree_entries[idx]->expanded = 0;
    last_child = tree_find_last_child(ed, idx);
    if (last_child == idx)
        return;
    tree_free_children(ed, idx + 1, last_child);
    tree_remove_range(ed, idx + 1, last_child);
}
