/*
** Cz PROJECT, 2026
** editor_keys
** File description:
** editor-mode key dispatch (chord, focus switch, input dispatch)
*/

#include "zex.h"

void handle_chord(editor_t *ed)
{
    ed->chord_active = 0;
    if (ed->ch == 19) {
        save_file(ed);
        display_all(ed);
    }
}

void handle_focus_switch(editor_t *ed)
{
    ed->focus = (ed->focus == FOCUS_TREE) ? FOCUS_EDITOR : FOCUS_TREE;
    display_all(ed);
}

void handle_editor_input(editor_t *ed)
{
    if (manage_backspace(ed))
        return;
    if (manage_enter(ed))
        return;
    if (arrow_manager(ed))
        return;
    manage_char(ed);
}

int confirm_exit(editor_t *ed)
{
    int choice = 0;
    
    if (!ed->modified)
        return -1;
    choice = prompt("Are you sure you want to exit? Unsaved changes will be lost.", ed);
    if (choice == 1)
        return -1;
    return 1;
}
