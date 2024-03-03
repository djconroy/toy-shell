#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "prompt.h"
#include "tokenize.h"

char *input = NULL;
size_t len = 0;
ssize_t num_chars;

int main(void) { 
    for (;;) {
        prompt();
        handle_input();
        if (num_chars == 1) { // Only newline character entered
            continue;
        }
        char **tokens = tokenize(input);
        
        if (strcmp("cd", tokens[0]) == 0) { // cd was entered
            char *path = tokens[1];
            if (path == NULL) {
                path = getenv("HOME");
            }
            if (chdir(path) == -1) { // chdir failed
                perror("cd");
            }
            free(tokens);
            continue;
        }
        // A command was entered
            
        int redirect = -1;
        int i;
        // Search the tokens for the redirect builtin
        for (i = 1; tokens[i] != NULL; i++) {
            if (strcmp(tokens[i], ">") == 0) {
                redirect = i;
                break;
            }
        }
        char *file;
        if (redirect > 0) {
            file = tokens[redirect + 1];
            if (file == NULL) {
                printf("No filename was entered. Usage: command > filename\n");
                free(tokens);
                continue;
            }
            // Create new memory block that will only store the command without the redirect part
            char **temp = calloc(redirect + 1, sizeof(char*));
            memcpy(temp, tokens, redirect * sizeof(char*));
            temp[redirect] = NULL;
            free(tokens);
            tokens = temp;
        }
        
        int child_status;
        pid_t pid = fork();
        if (pid == 0) { // Child process
            if (redirect > 0) {
                int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                dup2(fd, 1); // Redirect stdout to file in the child process
            }
            execvp(tokens[0], tokens); // Execute command entered
            printf("Unknown command\n");
            exit(EXIT_SUCCESS);
        } else { // Parent process
            signal(SIGINT, SIG_IGN); // Ensure the parent isn't interrupted if the child is
            wait(&child_status); // Wait for the child to finish executing the command
            signal(SIGINT, SIG_DFL);
        }
        free(tokens);
    }
}

void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n"); // Go to a new line
        prompt(); // Reprint prompt
        fflush(stdout);
    }
}

void handle_input(void) {
    signal(SIGINT, sig_handler); // Set up interrupt handler (in case the user types Ctrl+C)
    num_chars = getline(&input, &len, stdin); // Read a line from standard input
    signal(SIGINT, SIG_DFL);
    
    if (feof(stdin)) { // EOF encountered (e.g., if Ctrl+D is entered on the terminal)
        //free(input);
        exit(EXIT_SUCCESS);
    }
    if (num_chars <= 0) { // An error occurred or no characters were read
        //free(input);
        exit(EXIT_FAILURE);
    }
    input[num_chars - 1] = '\0'; // Trim trailing newline character
}
