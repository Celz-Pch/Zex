/*
** Cz PROJECT, 2026
** file_load
** File description:
** file content loading into editor
*/

#include "zex.h"

void file_ensure_nonempty(editor_t *ed)
{
    if (ed->num_lines == 0) {
        ed->lines = malloc(sizeof(char *));
        ed->lines[0] = strdup("");
        ed->num_lines = 1;
    }
}

void file_read_lines(editor_t *ed, FILE *f)
{
    char buf[4096];
    int len;

    while (fgets(buf, sizeof(buf), f)) {
        len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        ed->lines = realloc(ed->lines, (ed->num_lines + 1) * sizeof(char *));
        ed->lines[ed->num_lines] = strdup(buf);
        ed->num_lines++;
    }
}

int load_file(editor_t *ed, const char *path)
{
    FILE *f = fopen(path, "r");

    if (!f)
        return 0;
    for (int i = 0; i < ed->num_lines; i++)
        free(ed->lines[i]);
    free(ed->lines);
    ed->lines = NULL;
    ed->num_lines = 0;
    file_read_lines(ed, f);
    fclose(f);
    file_ensure_nonempty(ed);
    return 0;
}
