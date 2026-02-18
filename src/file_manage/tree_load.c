/*
** Cz PROJECT, 2026
** tree_load
** File description:
** directory reading and tree entry insertion
*/

#include "zex.h"

typedef struct {
    char    name[256];
    char    path[4096];
    int     is_dir;
} raw_entry_t;

static int raw_entry_cmp(const void *a, const void *b)
{
    const raw_entry_t *ea = a;
    const raw_entry_t *eb = b;

    if (ea->is_dir != eb->is_dir)
        return eb->is_dir - ea->is_dir;
    return strcmp(ea->name, eb->name);
}

static void tree_read_entry(raw_entry_t *entry, const char *dir, const char *name)
{
    struct stat st;

    strncpy(entry->name, name, 255);
    entry->name[255] = '\0';
    snprintf(entry->path, sizeof(entry->path), "%s/%s", dir, name);
    entry->is_dir = (stat(entry->path, &st) == 0
        && S_ISDIR(st.st_mode)) ? 1 : 0;
}

static raw_entry_t *tree_read_dir(const char *dir_path, int *count_out)
{
    DIR *dir = opendir(dir_path);
    struct dirent *de;
    raw_entry_t *raw = NULL;

    *count_out = 0;
    if (!dir)
        return NULL;
    while ((de = readdir(dir))) {
        if (de->d_name[0] == '.')
            continue;
        raw = realloc(raw, (*count_out + 1) * sizeof(raw_entry_t));
        tree_read_entry(&raw[*count_out], dir_path, de->d_name);
        (*count_out)++;
    }
    closedir(dir);
    return raw;
}

static void tree_insert_raw(editor_t *ed, raw_entry_t *raw,
    int count, int insert_pos, int depth)
{
    ed->tree_entries = realloc(ed->tree_entries,
        (ed->tree_count + count) * sizeof(tree_entry_t *));
    memmove(ed->tree_entries + insert_pos + count,
        ed->tree_entries + insert_pos,
        (ed->tree_count - insert_pos) * sizeof(tree_entry_t *));
    for (int i = 0; i < count; i++) {
        tree_entry_t *entry = malloc(sizeof(tree_entry_t));
        entry->name = strdup(raw[i].name);
        entry->path = strdup(raw[i].path);
        entry->is_dir = raw[i].is_dir;
        entry->expanded = 0;
        entry->depth = depth;
        ed->tree_entries[insert_pos + i] = entry;
    }
    ed->tree_count += count;
}

void tree_load(editor_t *ed, const char *dir_path,
    int insert_after, int depth)
{
    raw_entry_t *raw;
    int count;

    raw = tree_read_dir(dir_path, &count);
    if (!raw || count == 0) {
        free(raw);
        return;
    }
    qsort(raw, count, sizeof(raw_entry_t), raw_entry_cmp);
    tree_insert_raw(ed, raw, count, insert_after + 1, depth);
    free(raw);
}
