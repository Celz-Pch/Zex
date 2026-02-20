/*
** Cz PROJECT, 2026
** autocompletion
** File description:
** inline completion helpers for C sources
*/

#include "zex.h"

static int is_ident_char(int c)
{
    if (isalnum(c))
        return 1;
    if (c == '_')
        return 1;
    return 0;
}

static int is_ident_start(int c)
{
    if (isalpha(c))
        return 1;
    if (c == '_')
        return 1;
    return 0;
}

static int completion_enabled(editor_t *ed)
{
    char *dot = NULL;

    if (!ed->filename)
        return 1;
    dot = strrchr(ed->filename, '.');
    if (!dot)
        return 0;
    if (strcmp(dot, ".c") == 0)
        return 1;
    if (strcmp(dot, ".h") == 0)
        return 1;
    return 0;
}

static int get_max_line_len(editor_t *ed)
{
    int i = 0;
    int max_len = 0;
    int len = 0;

    while (i < ed->num_lines) {
        len = strlen(ed->lines[i]);
        if (len > max_len)
            max_len = len;
        i++;
    }
    return max_len;
}

static int get_max_keyword_len(void)
{
    const char **keywords = get_c_keywords();
    int i = 0;
    int max_len = 0;
    int len = 0;

    while (keywords[i]) {
        len = strlen(keywords[i]);
        if (len > max_len)
            max_len = len;
        i++;
    }
    return max_len;
}

static int completion_buffer_size(editor_t *ed)
{
    int max_line = get_max_line_len(ed);
    int max_kw = get_max_keyword_len();
    int max_len = (max_line > max_kw) ? max_line : max_kw;

    if (max_len < 1)
        max_len = 1;
    return max_len + 1;
}

static void consider_candidate(completion_ctx_t *ctx, const char *cand)
{
    int len = strlen(cand);

    if (len <= ctx->prefix_len)
        return;
    if (strncmp(cand, ctx->prefix, ctx->prefix_len) != 0)
        return;
    if (ctx->best_len != 0 && len > ctx->best_len)
        return;
    if (ctx->best_len != 0 && len == ctx->best_len
        && strcmp(cand, ctx->best) >= 0)
        return;
    if (len >= ctx->best_size)
        len = ctx->best_size - 1;
    memcpy(ctx->best, cand, len);
    ctx->best[len] = '\0';
    ctx->best_len = len;
}

static void scan_keywords(completion_ctx_t *ctx)
{
    const char **keywords = get_c_keywords();
    int i = 0;

    while (keywords[i]) {
        consider_candidate(ctx, keywords[i]);
        i++;
    }
}

static void scan_line(const char *line, completion_ctx_t *ctx,
    char *token, int token_size)
{
    int j = 0;
    int start = 0;
    int len = 0;

    while (line[j]) {
        if (is_ident_start((unsigned char)line[j])) {
            start = j;
            j++;
            while (line[j] && is_ident_char((unsigned char)line[j]))
                j++;
            len = j - start;
            if (len > 0 && len < token_size) {
                memcpy(token, line + start, len);
                token[len] = '\0';
                consider_candidate(ctx, token);
            }
        } else {
            j++;
        }
    }
}

static void scan_buffer(editor_t *ed, completion_ctx_t *ctx)
{
    char *token = NULL;
    int token_size = 0;
    int i = 0;

    token_size = completion_buffer_size(ed);
    token = malloc(token_size);
    if (!token)
        return;
    while (i < ed->num_lines) {
        scan_line(ed->lines[i], ctx, token, token_size);
        i++;
    }
    free(token);
}

static int compute_prefix(editor_t *ed, const char *line,
    prefix_info_t *info)
{
    int len = strlen(line);
    int x = ed->x;
    int start = 0;

    if (!completion_enabled(ed))
        return 0;
    if (x < 0 || x > len || x < len)
        return 0;
    start = x;
    while (start > 0 && is_ident_char((unsigned char)line[start - 1]))
        start--;
    if (start == x)
        return 0;
    if (!is_ident_start((unsigned char)line[start]))
        return 0;
    info->start = start;
    info->len = x - start;
    return 1;
}

static int build_best_match(editor_t *ed, completion_ctx_t *ctx)
{
    scan_keywords(ctx);
    scan_buffer(ed, ctx);
    return ctx->best_len;
}

static int find_best_match(editor_t *ed, const char *line,
    completion_result_t *res)
{
    completion_ctx_t ctx;

    if (!compute_prefix(ed, line, &res->info))
        return 0;
    ctx.prefix = line + res->info.start;
    ctx.prefix_len = res->info.len;
    ctx.best = res->best;
    ctx.best_size = res->best_size;
    ctx.best_len = 0;
    build_best_match(ed, &ctx);
    res->best_len = ctx.best_len;
    return res->best_len;
}

static int write_suffix(completion_result_t *res,
    char *out, int out_size)
{
    int suffix_len = 0;

    if (res->best_len <= res->info.len)
        return 0;
    suffix_len = res->best_len - res->info.len;
    if (suffix_len >= out_size)
        suffix_len = out_size - 1;
    if (suffix_len <= 0)
        return 0;
    memcpy(out, res->best + res->info.len, suffix_len);
    out[suffix_len] = '\0';
    return suffix_len;
}

int completion_get_suffix(editor_t *ed, char *out, int out_size)
{
    int abs_line = 0;
    const char *line = NULL;
    completion_result_t res;
    char *best = NULL;
    int buf_size = 0;
    int ret = 0;

    if (out_size <= 1)
        return 0;
    abs_line = ed->top_line + ed->y;
    line = ed->lines[abs_line];
    buf_size = completion_buffer_size(ed);
    best = malloc(buf_size);
    if (!best)
        return 0;
    res.best = best;
    res.best_size = buf_size;
    res.best_len = 0;
    if (find_best_match(ed, line, &res) == 0) {
        free(best);
        return 0;
    }
    if (out_size > buf_size)
        out_size = buf_size;
    if (out_size <= 1) {
        free(best);
        return 0;
    }
    ret = write_suffix(&res, out, out_size);
    free(best);
    return ret;
}

int completion_apply(editor_t *ed)
{
    char *suffix = NULL;
    int suffix_len = 0;
    int abs_line = 0;
    int i = 0;
    int buf_size = 0;

    buf_size = completion_buffer_size(ed);
    suffix = malloc(buf_size);
    if (!suffix)
        return 0;
    suffix_len = completion_get_suffix(ed, suffix, buf_size);
    if (suffix_len <= 0) {
        free(suffix);
        return 0;
    }
    abs_line = ed->top_line + ed->y;
    i = 0;
    while (i < suffix_len) {
        ed->lines[abs_line] = line_insert_char(ed->lines[abs_line],
            ed->x, suffix[i]);
        ed->x++;
        i++;
    }
    ed->modified = 1;
    free(suffix);
    return 1;
}

void completion_draw_inline(editor_t *ed, int row, int max_cols)
{
    char *suffix = NULL;
    int suffix_len = 0;
    int x = 0;
    WINDOW *win = NULL;
    int buf_size = 0;

    buf_size = completion_buffer_size(ed);
    suffix = malloc(buf_size);
    if (!suffix)
        return;
    suffix_len = completion_get_suffix(ed, suffix, buf_size);
    if (suffix_len <= 0) {
        free(suffix);
        return;
    }
    x = ed->x;
    win = ed->edit_win;
    if (x + suffix_len > max_cols)
        suffix_len = max_cols - x;
    if (suffix_len <= 0) {
        free(suffix);
        return;
    }
    wattron(win, COLOR_PAIR(ZP_COMMENT) | A_DIM);
    mvwaddnstr(win, row, x, suffix, suffix_len);
    wattroff(win, COLOR_PAIR(ZP_COMMENT) | A_DIM);
    free(suffix);
}
