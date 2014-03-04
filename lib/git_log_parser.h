#ifndef GIT_LOG_PARSER_H
#define GIT_LOG_PARSER_H

typedef enum {
    GitLogParserFalse = 0,
    GitLogParserTrue = 1
} GitLogParserBoolean;

typedef struct {
    int commit_range;
    const char *git_log_command;
    char **logs;

    GitLogParserBoolean valid;
    const char *error_message;
} GitLogParser;

extern GitLogParser *GitLogParser_create(int commit_range);
extern const char **GitLogParser_logs(GitLogParser *parser);
extern void GitLogParser_printErrorMessage(GitLogParser *parser);
extern void GitLogParser_destroy(GitLogParser *parser);

#endif
