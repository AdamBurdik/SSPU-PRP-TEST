#pragma once

int get_int(const char *text);

#ifdef INPUT_IMPLEMENTATION
#include <stdio.h>

int get_int(const char *text) {
    int number;
    bool been_cleared = false;

    for (;;) {
        printf(text);
        scanf("%d", &number);

        been_cleared = false;
        while (getchar() != '\n') {been_cleared = true;}

        if (been_cleared == true) {
            printf("[ERROR] Please input a valid integer\n");
            continue;
        }

        return number;
    }
}

#endif
