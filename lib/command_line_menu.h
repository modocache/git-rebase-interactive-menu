#ifndef COMMAND_LINE_MENU_H
#define COMMAND_LINE_MENU_H

typedef enum {
    CommandLineMenuCursorDirectionUp = 1,
    CommandLineMenuCursorDirectionDown
} CommandLineMenuCursorDirection;

typedef enum {
    CommandLineMenuFalse = 0,
    CommandLineMenuTrue = 1
} CommandLineMenuBoolean;

typedef struct {
    int number_of_items;
    char **items;
    int cursor_index;
} CommandLineMenu;

CommandLineMenu *CommandLineMenu_create(int number_of_items, const char **items);
void CommandLineMenu_print(CommandLineMenu *menu,
                           CommandLineMenuBoolean clear_previous_menu);
const char **CommandLineMenu_items(CommandLineMenu *menu);
int CommandLineMenu_run(CommandLineMenu *menu);
int CommandLineMenu_moveCursor(CommandLineMenu *menu,
                                CommandLineMenuCursorDirection direction);
void CommandLineMenu_destroy(CommandLineMenu *menu);

#endif
