#include <stdio.h>

int main(void) {
    char         buffer[10000] = { 0 };
    char*        caret         = buffer;
    unsigned int buff_len      = 10000;
    unsigned int twrite        = 0;

    for (int i = 0; i < 1665; ++i) {
        twrite    = sprintf_s(caret, buff_len, "%4d, ", i);
        caret    += twrite;
        buff_len -= twrite;
    }

    puts(buffer);
    printf_s("%u bytes remain empty in the buffer.\n", buff_len);
}
