#include <stdio.h>
#include <time.h>

#include "prompt.h"

#define LENGTH 17

void prompt(void) {
    char prompt[LENGTH];
    time_t now;
    time(&now);
    strftime(prompt, LENGTH, "[%d/%m %H:%M]# ", localtime(&now));
    printf("%s", prompt);
}
