#include <stdio.h>

void static __forceinline __stdcall CapitalizeFirstLetter(char* restrict string) {
    *string = (*string) - 32;
    return;
}

void static __forceinline __stdcall Capitalize(char* restrict string) {
    while ((*string) != 0) {
        *string = (*string) - 32;
        string++;
    }
    return;
}

int main(void) {
    char* name   = "anoban";
    char* laptop = "inspiron";

    puts(name);
    CapitalizeFirstLetter(name);
    puts(name);

    puts(laptop);
    Capitalize(laptop);
    puts(laptop);

    return 0;
}