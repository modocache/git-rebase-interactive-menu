#include "command_line_menu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    CommandLineMenuKeyCodeEnter = 13,
    CommandLineMenuKeyCodeUp = 65,
    CommandLineMenuKeyCodeDown = 66,
} CommandLineMenuKeyCode;

#pragma mark - Public Interface

CommandLineMenu *CommandLineMenu_create(int number_of_items, const char **items) {
    CommandLineMenu *menu = (CommandLineMenu *)malloc(sizeof(CommandLineMenu));
    menu->number_of_items = number_of_items;
    menu->cursor_index = 0;

    menu->items = (char **)malloc(sizeof(char*) * menu->number_of_items);
    for (int i = 0; i < menu->number_of_items; ++i) {
        menu->items[i] = (char *)malloc(strlen(items[i]));
        strcpy(menu->items[i], items[i]);
    }

    return menu;
}

void CommandLineMenu_print(CommandLineMenu *menu,
                           CommandLineMenuBoolean clear_previous_menu) {
    if (clear_previous_menu) {
        fprintf(stdout, "\e[%dA", menu->number_of_items);
    }

    for (int i = 0; i < menu->number_of_items; ++i) {
        if (menu->cursor_index == i) {
            fprintf(stdout, "\r* %s\n\r", CommandLineMenu_items(menu)[i]);
        } else {
            fprintf(stdout, "\r  %s\n\r", CommandLineMenu_items(menu)[i]);
        }
    }
}

const char **CommandLineMenu_items(CommandLineMenu *menu) {
    return (const char **)menu->items;
}

int CommandLineMenu_run(CommandLineMenu *menu) {
    system("stty raw");

    int selected_index = -1;
    char input;
    while((input = getchar())) {
        int input_index = atoi(&input);
        if (input_index > 0 && input_index <= menu->number_of_items) {
            selected_index = input_index - 1;
            break;
        }

        if (input == CommandLineMenuKeyCodeUp) {
            CommandLineMenu_moveCursor(menu, CommandLineMenuCursorDirectionUp);
            CommandLineMenu_print(menu, CommandLineMenuTrue);
        } else if (input == CommandLineMenuKeyCodeDown) {
            CommandLineMenu_moveCursor(menu, CommandLineMenuCursorDirectionDown);
            CommandLineMenu_print(menu, CommandLineMenuTrue);
        } else if (input == CommandLineMenuKeyCodeEnter) {
            selected_index = menu->cursor_index;
            break;
        } else if (input == 'q' || input == 'Q') {
            fprintf(stdout, "Aborting rebase.\n\r");
            break;
        }
    }

    system("stty cooked");
    fprintf(stdout, "\n\r");

    return selected_index;
}

int CommandLineMenu_moveCursor(CommandLineMenu *menu,
                               CommandLineMenuCursorDirection direction) {
    switch (direction) {
        case CommandLineMenuCursorDirectionUp:
            if (menu->cursor_index == 0) {
                return -1;
            }
            menu->cursor_index--;
            break;
        case CommandLineMenuCursorDirectionDown:
            if (menu->cursor_index >= menu->number_of_items - 1) {
                return -1;
            }
            menu->cursor_index++;
            break;
    }

    return 0;
}

void CommandLineMenu_destroy(CommandLineMenu *menu) {
    free(menu);
}
