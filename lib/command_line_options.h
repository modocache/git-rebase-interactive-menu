#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

typedef enum {
    CommandLineOptionsFalse = 0,
    CommandLineOptionsTrue = 1
} CommandLineOptionsBoolean;

typedef struct {
    const char *command_path;
    CommandLineOptionsBoolean valid;
    const char *error_message;

    CommandLineOptionsBoolean print_help;
    CommandLineOptionsBoolean print_version;
    int commit_range;
} CommandLineOptions;

extern CommandLineOptions *CommandLineOptions_create(int argc, const char **argv);
extern void CommandLineOptions_printErrorMessage(CommandLineOptions *options);
extern void CommandLineOptions_destroy(CommandLineOptions *options);

#endif
