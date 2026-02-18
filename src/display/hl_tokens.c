/*
** Cz PROJECT, 2026
** hl_tokens
** File description:
** syntax token handlers (comment, string, number, identifier)
*/

#include "zex.h"

int hl_comment_line(WINDOW *win, const char *line,
    int *i, int *col, int len, int max)
{
    int n;

    if (*i + 1 >= len || line[*i] != '/' || line[*i + 1] != '/')
        return 0;
    n = len - *i;
    if (*col + n > max)
        n = max - *col;
    emit(win, line + *i, n, ZP_COMMENT, A_DIM);
    *i += n;
    *col += n;
    return 1;
}

int hl_block_comment(WINDOW *win, const char *line,
    int *i, int *col, int len, int max)
{
    if (*i + 1 >= len || line[*i] != '/' || line[*i + 1] != '*')
        return 0;
    wattron(win, COLOR_PAIR(ZP_COMMENT) | A_DIM);
    while (*i < len && *col < max) {
        waddch(win, (unsigned char)line[*i]);
        (*col)++;
        if (*i >= 1 && line[*i - 1] == '*' && line[*i] == '/') {
            (*i)++;
            break;
        }
        (*i)++;
    }
    wattroff(win, COLOR_PAIR(ZP_COMMENT) | A_DIM);
    return 1;
}

int hl_string_lit(WINDOW *win, const char *line,
    int *i, int *col, int len, int max, char delim)
{
    if (line[*i] != delim)
        return 0;
    wattron(win, COLOR_PAIR(ZP_STRING));
    waddch(win, (unsigned char)line[(*i)++]);
    (*col)++;
    while (*i < len && *col < max && line[*i] != delim) {
        if (line[*i] == '\\' && *i + 1 < len && *col < max) {
            waddch(win, (unsigned char)line[(*i)++]);
            (*col)++;
        }
        if (*i < len && *col < max) {
            waddch(win, (unsigned char)line[(*i)++]);
            (*col)++;
        }
    }
    if (*i < len && *col < max) {
        waddch(win, (unsigned char)line[(*i)++]);
        (*col)++;
    }
    wattroff(win, COLOR_PAIR(ZP_STRING));
    return 1;
}

int hl_number(WINDOW *win, const char *line,
    int *i, int *col, int len, int max)
{
    if (!isdigit((unsigned char)line[*i]))
        return 0;
    wattron(win, COLOR_PAIR(ZP_NUMBER));
    while (*i < len && *col < max
        && (isxdigit((unsigned char)line[*i]) || line[*i] == '.'
            || line[*i] == 'x' || line[*i] == 'X'
            || line[*i] == 'u' || line[*i] == 'U'
            || line[*i] == 'l' || line[*i] == 'L')) {
        waddch(win, (unsigned char)line[(*i)++]);
        (*col)++;
    }
    wattroff(win, COLOR_PAIR(ZP_NUMBER));
    return 1;
}

int hl_identifier(WINDOW *win, const char *line,
    int *i, int *col, int len, int max)
{
    int start;
    int word_len;
    int n;

    if (!isalpha((unsigned char)line[*i]) && line[*i] != '_')
        return 0;
    start = *i;
    while (*i < len && (isalnum((unsigned char)line[*i]) || line[*i] == '_'))
        (*i)++;
    word_len = *i - start;
    n = (*col + word_len > max) ? max - *col : word_len;
    if (is_keyword(line + start, word_len))
        emit(win, line + start, n, ZP_KEYWORD, A_BOLD);
    else
        emit(win, line + start, n, ZP_NORMAL, 0);
    *col += n;
    return 1;
}
