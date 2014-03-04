#include "command_line_options.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void CommandLineOptions_setErrorMessage(CommandLineOptions *options, const char *message);

#pragma mark - Public Interface

CommandLineOptions *CommandLineOptions_create(int argc, const char **argv) {
    size_t options_size = sizeof(CommandLineOptions);
    CommandLineOptions *options = (CommandLineOptions *)malloc(options_size);

    options->valid = CommandLineOptionsTrue;
    options->command_path = *argv;

    if (argc == 1) {
        CommandLineOptions_setErrorMessage(options, "Error: Too few arguments.");
        options->print_help = CommandLineOptionsTrue;
    } else if (argc > 2) {
        CommandLineOptions_setErrorMessage(options, "Error: Too many arguments.");
    }

    argv++;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(*argv, "--help")) {
            options->print_help = CommandLineOptionsTrue;
            break;
        } else if (!strcmp(*argv, "--version")) {
            options->print_version = CommandLineOptionsTrue;
            break;
        } else {
            if (strlen(*argv) <= 1) {
                CommandLineOptions_setErrorMessage(options, "Error: Invalid argument.");
                break;
            }

            const char *revision_range = *argv + 1;
            options->commit_range = atoi(revision_range);
            if (options->commit_range == 0) {
                CommandLineOptions_setErrorMessage(options, "Error: Invalid argument.");
                break;
            } else if (options->commit_range > 20) {
                CommandLineOptions_setErrorMessage(options, "Error: Too many commits to display in menu.");
            }
        }
        argv++;
    }

    return options;
}

extern void CommandLineOptions_printErrorMessage(CommandLineOptions *options) {
    fprintf(stderr, "%s\n", options->error_message);
}

void CommandLineOptions_destroy(CommandLineOptions *options) {
    free(options);
}

#pragma mark - Private Functions

void CommandLineOptions_setErrorMessage(CommandLineOptions *options, const char *message) {
    options->valid = CommandLineOptionsFalse;
    options->error_message = message;
}
