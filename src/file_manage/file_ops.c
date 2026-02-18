/*
** Cz PROJECT, 2026
** file_ops
** File description:
** editor creation, open-file and save
*/

#include "zex.h"

editor_t *init_editor(char *file_path, char *dir_path)
{
    editor_t *ed = calloc(1, sizeof(editor_t));

    if (!ed)
        return NULL;
    init_editor_windows(ed, dir_path);
    init_editor_tree_state(ed, dir_path);
    init_editor_content(ed, file_path);
    return ed;
}

void editor_open_file(editor_t *ed, const char *path)
{
    for (int i = 0; i < ed->num_lines; i++)
        free(ed->lines[i]);
    free(ed->lines);
    ed->lines = malloc(sizeof(char *));
    ed->lines[0] = strdup("");
    ed->num_lines = 1;
    free(ed->filename);
    ed->filename = strdup(path);
    ed->x = 0;
    ed->y = 0;
    ed->top_line = 0;
    ed->modified = 0;
    load_file(ed, path);
}

int save_file(editor_t *ed)
{
    FILE *f;

    if (!ed->filename)
        return -1;
    f = fopen(ed->filename, "w");
    if (!f)
        return -1;
    for (int i = 0; i < ed->num_lines; i++) {
        fputs(ed->lines[i], f);
        if (i < ed->num_lines - 1)
            fputc('\n', f);
    }
    fclose(f);
    ed->modified = 0;
    return 0;
}
