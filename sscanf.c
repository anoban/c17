#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char        c;
    uint64_t    a;
    float       b;
    char        string[100] = { 0 };

    const char* text        = "Hi, how are ya?. 4532; 67.653476358\n";
    sscanf_s(text, "%c%[^.].%llu; %f\n", &c, 1U, string, 100U, &a, &b);
    printf_s("char = %c, uint64_t = %llu, float = %.15f, string = %s\n", c, a, b, string);

    char        name[30] = { 0 }, rest[200] = { 0 };
    const char* record =
        "350040, \"Hines, Haileigh\", Female, 22, 156, 61.7, Other, 2.27, 6th, Political Science, Biology, 57.0, 54.0, 0, 1, 33969.1592680493\r\n";
    sscanf_s(record, "%*[^,], \"%[^\"],%[*^\r\n]\r\n", name, 30U, rest, 200U);
    puts(name);
    puts(rest);

    return 0;
}