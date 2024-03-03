#include <stdlib.h>
#include <string.h>

#include "tokenize.h"

char **tokenize(char *input) {
    char **tokens = malloc(sizeof(char*));
    int num_tokens = 0;
    char *token = strtok(input, " ");
    
    while (token != NULL) {
        tokens[num_tokens] = token;
        num_tokens++;
        tokens = realloc(tokens, (num_tokens + 1) * sizeof(char*));
        token = strtok(NULL, " ");
    }
    tokens[num_tokens] = NULL; // Indicates end of list of tokens
    return tokens;
}
