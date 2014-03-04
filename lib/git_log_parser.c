#include "git_log_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const size_t kGitLogBufferSize = 100;
static const int kGitLogShortHashLength = 7;
static const char kGitLogCommand[] = "git log --oneline -%d";

GitLogParserBoolean is_log(const char *hash);
void strip_newlines(char *hash);
void GitLogParser_getLogs(GitLogParser *parser);
void GitLogParser_setErrorMessage(GitLogParser *parser, const char *error_message);

#pragma mark - Public Interface

GitLogParser *GitLogParser_create(int commit_range) {
    size_t parser_size = sizeof(GitLogParser);
    GitLogParser *parser = (GitLogParser *)malloc(parser_size);
    parser->valid = GitLogParserTrue;
    parser->commit_range = commit_range;

    parser->logs = (char **)malloc(sizeof(char*) * parser->commit_range);
    for (int i = 0; i < parser->commit_range; ++i) {
        parser->logs[i] = (char *)malloc(sizeof(char) * kGitLogBufferSize);
    }

    int buffer_size = 30;
    char git_command[buffer_size];
    int written_length = snprintf(git_command,
                                  buffer_size,
                                  kGitLogCommand,
                                  commit_range);
    if (written_length >= buffer_size || written_length < 0) {
        GitLogParser_setErrorMessage(parser, "Error: Could not create 'git log' command for given commit range.");
    }

    parser->git_log_command = git_command;
    GitLogParser_getLogs(parser);

    return parser;
}

const char **GitLogParser_logs(GitLogParser *parser) {
    return (const char **)parser->logs;
}

void GitLogParser_destroy(GitLogParser *parser) {
    for (int i = 0; i < parser->commit_range; ++i) {
        free(parser->logs[i]);
    }
    free(parser->logs);
    free(parser);
}

#pragma mark - Private Methods

GitLogParserBoolean is_log(const char *hash) {
    size_t size = strlen(hash);
    int count = size < kGitLogShortHashLength ? size : kGitLogShortHashLength;
    for (int i = 0; i < count; ++i) {
        if (hash[i] == ' ' || hash[i] == ':') {
            return GitLogParserFalse;
        }
    }

    return GitLogParserTrue;
}

void strip_newlines(char *hash) {
    size_t size = strlen(hash);
    hash[size - 1] = '\0';
}

void GitLogParser_getLogs(GitLogParser *parser) {
    FILE *pipe = popen(parser->git_log_command, "r");
    if (!pipe) {
        GitLogParser_setErrorMessage(parser, "Error: Could not execute 'git log' command.");
        return;
    }

    int line_number = 0;
    char line_buffer[kGitLogBufferSize];
    char *line;
    while ((line = fgets(line_buffer, sizeof(line_buffer), pipe))) {
        strip_newlines(line);
        strncpy(parser->logs[line_number], line, kGitLogBufferSize);
        if (!is_log(parser->logs[line_number])) {
            GitLogParser_setErrorMessage(parser, "Error: Could not parse 'git log' command.");
        }

        ++line_number;
    }

    pclose(pipe);

    if (line_number < parser->commit_range) {
        GitLogParser_setErrorMessage(parser, "Error: Attempted to log more commits than exist in tree.");
    }
}

extern void GitLogParser_printErrorMessage(GitLogParser *parser) {
    fprintf(stderr, "%s\n", parser->error_message);
}

void GitLogParser_setErrorMessage(GitLogParser *parser, const char *error_message) {
    parser->valid = GitLogParserFalse;
    parser->error_message = error_message;
}
