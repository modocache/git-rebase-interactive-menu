#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/command_line_options.h"
#include "lib/git_log_parser.h"
#include "lib/command_line_menu.h"

const char name[] = "git-rebase-interactive-menu";
const char version[] = "0.1.0";
const char usage_string[] = "git-rebase-interactive-menu [-<number>]";
const char kGitRebaseCommand[] = "git rebase --interactive head~%d";

void print_usage() {
    fprintf(stderr, "usage: %s\n", usage_string);
}

void print_version() {
    fprintf(stderr, "%s version %s\n", name, version);
}

int handle_options(CommandLineOptions *options) {
    if (options->print_help) {
        print_usage();
        return options->valid ? 0 : 1;
    } else if (options->print_version) {
        print_version();
        return 0;
    } else if (!options->valid) {
        CommandLineOptions_printErrorMessage(options);
        return 1;
    } else {
        GitLogParser *parser = GitLogParser_create(options->commit_range);
        if (!parser->valid) {
            GitLogParser_printErrorMessage(parser);
            return 1;
        }

        CommandLineMenu *menu = CommandLineMenu_create(parser->commit_range,
                                                       GitLogParser_logs(parser));
        GitLogParser_destroy(parser);

        CommandLineMenu_print(menu, CommandLineMenuFalse);
        int selected_index = CommandLineMenu_run(menu);
        if (selected_index == -1) {
            return 1;
        }

        int buffer_size = strlen(kGitRebaseCommand) + 2;
        char git_command[buffer_size];
        int written_length = snprintf(git_command,
                                      buffer_size,
                                      kGitRebaseCommand,
                                      selected_index);
        if (written_length >= buffer_size || written_length < 0) {
            fprintf(stderr, "Error: Could not execute rebase command.");
        }

        CommandLineMenu_destroy(menu);

        system(git_command);
        return 0;
    }
}

int main(int argc, const char **argv) {
    CommandLineOptions *options = CommandLineOptions_create(argc, argv);
    int exit_code = handle_options(options);
    CommandLineOptions_destroy(options);

    return exit_code;
}
