minishell
Summary
This repository implements a minimal Unix shell (“minishell”) in C, following the 42 School project specifications. It reads user input, parses commands (including arguments, quotes, environment-variable expansion, redirections, and pipes), executes built-in or external programs, and handles basic signals (e.g., Ctrl-C, Ctrl-D). :contentReference[oaicite:0]{index=0}

Repository Structure
.vscode/

VSCode configuration files (e.g., settings.json, launch.json) to streamline debugging. :contentReference[oaicite:1]{index=1}
include/

Header files defining core data structures, function prototypes, macros, and constants.
Common headers likely include:
minishell.h (central definitions for the shell’s state, global variables, and utility macros)
parser.h (prototypes for tokenization and parsing routines)
execute.h (prototypes for execution, redirections, and piping)
builtins.h (prototypes for built-in commands like cd, echo, export, unset, pwd, env, exit)
utils.h (miscellaneous helper functions, e.g., string handling, error reporting). :contentReference[oaicite:2]{index=2}
srcs/

All C source files that implement the shell’s functionality. Typical modules include:
main.c
Initializes shell state (e.g., copying environ, setting up signal handlers), enters the main input loop, and calls the prompt, parser, and executor.
prompt.c
Displays a custom prompt (e.g., minishell$ ), reads a line of input (often using readline or a custom function), and handles end-of-file (e.g., Ctrl-D) gracefully.
tokenizer.c & lexer.c
Breaks the input string into tokens, handling single and double quotes, escape characters, and splitting on whitespace and metacharacters (|, <, >, >>).
parser.c
Builds an abstract syntax tree (or a linear list of command structs) that represents each simple command and its arguments, including any redirections or pipes.
expander.c
Performs environment-variable expansion ($VAR), exit-status expansion ($?), and handles tilde (~) expansion if implemented.
execute.c
Walks the parsed structure, sets up pipes between commands, forks new processes, applies redirections (<, >, >>), and either invokes built-in functions (in the parent process) or uses execve in child processes.
builtins.c & builtin_<name>.c (e.g., builtin_cd.c, builtin_echo.c, builtin_export.c, etc.)
Implement each built-in command by modifying shell state directly (e.g., updating PWD for cd, printing to stdout for echo, adjusting the environment list for export/unset).
redir_utils.c, pipe_utils.c
Helper functions to open files for input (<) or output (>, >>), duplicate file descriptors (dup2), create and close pipes, and restore standard file descriptors after execution.
signals.c
Configures and handles SIGINT (Ctrl-C) and SIGQUIT (Ctrl-) both at the prompt (so that pressing Ctrl-C returns a fresh prompt) and during execution (so that child processes terminate correctly).
env_utils.c
Manages a linked list or array of environment variables: copying environ at startup, searching/updating for export and unset, and converting back to char ** when launching external programs.
utils.c & error.c
General utility functions, e.g., string and memory helpers (ft_strdup, ft_split, ft_strjoin), and error/reporting routines that print standardized error messages (minishell: <command>: <message>) to stderr. :contentReference[oaicite:3]{index=3}
Makefile

Automates building the minishell executable:
all (default): Compiles all .c files in srcs/, including linking necessary libraries (e.g., -lreadline if using readline).
clean, fclean: Remove object files (*.o) and the final minishell binary.
re: Equivalent to fclean followed by all. :contentReference[oaicite:4]{index=4}
.gitignore

Ignores object files, editor swapfiles, and any build artifacts (e.g., *.o, minishell, *.out). :contentReference[oaicite:5]{index=5}
suppression.txt

A list of suppressed sanitizer or linting warnings (e.g., leaks suppression), if sanitizers (like Valgrind or AddressSanitizer) are used during development. :contentReference[oaicite:6]{index=6}
Languages and Tools
C (96.7%): All core logic is implemented in ISO C (C99/C11 compliant). :contentReference[oaicite:7]{index=7}
Makefile (3.3%): Build configuration to compile and link sources into the minishell executable. :contentReference[oaicite:8]{index=8}
(Optional) Readline: If readline is used for input, link against -lreadline and include <readline/readline.h> and <readline/history.h>. :contentReference[oaicite:9]{index=9}
How to Build and Run
Clone the repository:
git clone https://github.com/yuhi-ootani/minishell.git
cd minishell
``` :contentReference[oaicite:10]{index=10}  
Compile with Make:
make
