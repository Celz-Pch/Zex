/*
** Cz PROJECT, 2026
** syntax
** File description:
** syntax highlighting: keyword list, emit helper, line dispatcher
*/

#include "zex.h"

static const char *C_KEYWORDS[] = {
    "auto", "break", "case", "char", "const", "continue", "default",
    "do", "double", "else", "enum", "extern", "float", "for", "goto",
    "if", "inline", "int", "long", "register", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while",
    "NULL", "true", "false", "bool",
    "define", "include", "ifndef", "ifdef", "endif", "pragma", "undef",
    NULL
};

int is_keyword(const char *word, int len)
{
    for (int i = 0; C_KEYWORDS[i]; i++)
        if ((int)strlen(C_KEYWORDS[i]) == len
            && strncmp(C_KEYWORDS[i], word, len) == 0)
            return 1;
    return 0;
}

void emit(WINDOW *win, const char *s, int len, int pair, int attr)
{
    wattron(win, COLOR_PAIR(pair) | attr);
    for (int i = 0; i < len; i++)
        waddch(win, (unsigned char)s[i]);
    wattroff(win, COLOR_PAIR(pair) | attr);
}

static void hl_default_char(WINDOW *win, const char *line, int *i, int *col)
{
    wattron(win, COLOR_PAIR(ZP_NORMAL));
    waddch(win, (unsigned char)line[(*i)++]);
    (*col)++;
    wattroff(win, COLOR_PAIR(ZP_NORMAL));
}

static int hl_dispatch(WINDOW *win, const char *line,
    int *i, int *col, int len, int max)
{
    if (hl_comment_line(win, line, i, col, len, max))
        return -1;
    if (hl_block_comment(win, line, i, col, len, max)) return 1;
    if (hl_string_lit(win, line, i, col, len, max, '"')) return 1;
    if (hl_string_lit(win, line, i, col, len, max, '\'')) return 1;
    if (hl_number(win, line, i, col, len, max)) return 1;
    if (hl_identifier(win, line, i, col, len, max)) return 1;
    hl_default_char(win, line, i, col);
    return 1;
}

void highlight_line(WINDOW *win, int row, const char *line, int max_cols)
{
    int i = 0;
    int col = 0;
    int len = strlen(line);
    int first_char = 0;

    wmove(win, row, 0);
    wclrtoeol(win);
    if (len == 0)
        return;
    wmove(win, row, 0);
    while (first_char < len && line[first_char] == ' ')
        first_char++;
    if (first_char < len && line[first_char] == '#') {
        emit(win, line, len > max_cols ? max_cols : len, ZP_PREPROC, A_BOLD);
        return;
    }
    while (i < len && col < max_cols)
        if (hl_dispatch(win, line, &i, &col, len, max_cols) == -1)
            return;
}
