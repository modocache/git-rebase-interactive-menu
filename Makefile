CC=g++
CFLAGS=-c -Wall -O2
ROOT_DIR = $(realpath .)/

all:
	$(CC) $(ROOT_DIR)git-rebase-interactive-menu.c $(ROOT_DIR)lib/command_line_options.c $(ROOT_DIR)lib/git_log_parser.c $(ROOT_DIR)lib/command_line_menu.c -o $(ROOT_DIR)git-rebase-interactive-menu
