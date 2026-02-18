/*
** EPITECH PROJECT, 2026
** zex
** File description:
** zex
*/

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>

#ifndef ZEX_H
    #define ZEX_H

#define ZP_NORMAL 1
#define ZP_KEYWORD 2
#define ZP_STRING 3
#define ZP_COMMENT 4
#define ZP_NUMBER 5
#define ZP_PREPROC 6
#define ZP_TREE_DIR 7
#define ZP_TREE_SEL 8
#define ZP_STATUS 9
#define ZP_TREE_HDR 10

#define TREE_WIDTH 26

typedef enum {
    FOCUS_TREE,
    FOCUS_EDITOR
} focus_t;

typedef struct tree_entry_s {
    char *name;
    char *path;
    int is_dir;
    int expanded;
    int depth;
} tree_entry_t;

typedef struct editor_s {
    int x;
    int y;
    int ch;
    char **lines;
    int num_lines;
    int top_line;
    char *filename;
    int modified;
    WINDOW *tree_win;
    WINDOW *edit_win;
    int has_tree;
    tree_entry_t **tree_entries;
    int tree_count;
    int tree_selected;
    int tree_top;
    char *tree_root_path;
    focus_t focus;
    int chord_active;
} editor_t;

editor_t *init_editor(char *file_path, char *dir_path);
void editor_open_file(editor_t *ed, const char *path);
int save_file(editor_t *ed);
void init_editor_windows(editor_t *ed, char *dir_path);
void init_editor_tree_state(editor_t *ed, char *dir_path);
void init_editor_content(editor_t *ed, char *file_path);
int load_file(editor_t *ed, const char *path);
void file_read_lines(editor_t *ed, FILE *f);
void file_ensure_nonempty(editor_t *ed);
void free_editor(editor_t *ed);
void tree_load_init(editor_t *ed);
void tree_expand(editor_t *ed, int idx);
void tree_handle_enter(editor_t *ed);
void tree_load(editor_t *ed, const char *dir_path, int insert_after, int depth);
void tree_collapse(editor_t *ed, int idx);
void init_colors(void);
void display_all(editor_t *ed);
void display_content(editor_t *ed);
void draw_tree(editor_t *ed);
void draw_status_bar(editor_t *ed);
void emit(WINDOW *win, const char *s, int len, int pair, int attr);
int is_keyword(const char *word, int len);
void highlight_line(WINDOW *win, int row, const char *line, int max_cols);
int hl_comment_line(WINDOW *win, const char *line, int *i, int *col, int len, int max);
int hl_block_comment(WINDOW *win, const char *line, int *i, int *col, int len, int max);
int hl_string_lit(WINDOW *win, const char *line, int *i, int *col, int len, int max, char delim);
int hl_number(WINDOW *win, const char *line, int *i, int *col, int len, int max);
int hl_identifier(WINDOW *win, const char *line, int *i, int *col, int len, int max);
int manage_char(editor_t *ed);
int manage_backspace(editor_t *ed);
int manage_enter(editor_t *ed);
void main_loop(editor_t *ed);
int arrow_left(editor_t *ed);
int arrow_right(editor_t *ed);
int arrow_up(editor_t *ed);
int arrow_down(editor_t *ed);
int arrow_manager(editor_t *ed);
char *line_insert_char(char *line, int pos, int ch);
void line_delete_char(char *line, int pos);
void backspace_in_line(editor_t *ed);
void backspace_merge(editor_t *ed);
char *enter_split_line(editor_t *ed);
void enter_move_cursor(editor_t *ed);
void handle_chord(editor_t *ed);
void handle_focus_switch(editor_t *ed);
void handle_editor_input(editor_t *ed);

#endif
