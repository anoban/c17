#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define MAX_COUNT 10000U

void tokenizer(char* stream, const char delimiter) {
    // stream needs to have at least 4 unoccupied bytes at the end.

    while (*stream != 0) {
        if (*stream == delimiter) *stream = 0;
        stream++;
    }

    if (*stream == 0) {
        for (int i = 0; i < 4; ++i) {
            // append 4 zeroes at the end. (5 in total, including the null terminator)
            *(stream++) = 0;
        }
    }

    return;
}

void print_tokens(const char* stream) {
    while (!((stream[0] == 0) && (stream[1] == 0) && (stream[2] == 0) && (stream[3] == 0)))
        if (stream[0] == 0 && stream[1] != 0)
            puts(++stream);
        else
            stream++;
}

int main(void) {
    char text[100] = "Hi, there, Anoban, how's, the day, been, for you,?";
    tokenizer(text, ',');
    print_tokens(text);

    return 0;
}