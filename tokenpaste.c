#include <stdio.h>

#define __TOWSTR(STR) L##STR // token paste ##

int main(void) {
    wchar_t* name = __TOWSTR("Anoban");
    _putws(name);

    return 0;
}
