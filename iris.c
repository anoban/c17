#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#define BUFFSIZE 10000
#pragma warning(disable : 4710)

typedef struct {
        long double sepal_length;
        long double sepal_width;
        long double petal_length;
        long double petal_width;
        char        species[18];
} iris_t;

int main(void) {
    // There are 150 records in the text file.
    iris_t* iris_array       = NULL;

    // read buffer.
    char    buffer[BUFFSIZE] = { 0 };

    char*   token            = NULL;
    char*   next_token       = NULL;

    HANDLE  hFile            = CreateFileW(L"./iris.data", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwBytesRead = 0;
        if (ReadFile(hFile, buffer, BUFFSIZE, &dwBytesRead, NULL)) {
            if ((iris_array = (iris_t*) malloc(sizeof(iris_t) * 160))) {
                memset(iris_array, 0, sizeof(iris_t) * 160);
                int offset = 0;
                token      = strtok_s(buffer, "\n", &next_token);

                sscanf_s(
                    token,
                    "%LF,%LF,%Lf,%LF,%[^\n]",
                    &iris_array[offset].sepal_length,
                    &iris_array[offset].sepal_width,
                    &iris_array[offset].petal_length,
                    &iris_array[offset].petal_width,
                    &iris_array[offset].species,
                    (unsigned int) 18
                );
                offset++;

                while ((token = strtok_s(NULL, "\n", &next_token))) {
                    // 4.4,2.9,1.4,0.2,Iris-setosa
                    // puts(token);
                    sscanf_s(
                        token,
                        "%LF,%LF,%Lf,%LF,%[^\n]",
                        &iris_array[offset].sepal_length,
                        &iris_array[offset].sepal_width,
                        &iris_array[offset].petal_length,
                        &iris_array[offset].petal_width,
                        &iris_array[offset].species,
                        (unsigned) 18
                    );
                    offset++;
                }

                long double sepal_length = 0.0, sepal_width = 0.0, petal_length = 0.0, petal_width = 0.0;

                for (int i = 0; i < offset; ++i) {
                    sepal_length += iris_array[i].sepal_length;
                    sepal_width  += iris_array[i].sepal_width;
                    petal_length += iris_array[i].petal_length;
                    petal_width  += iris_array[i].petal_width;
                    // puts(iris_array[i].species);
                }

                printf_s("%d Records have been parsed successfully!\n", offset + 1);
                printf_s(
                    "Mean sepal_length: %10.5Lf cm, sepal_width: %10.5Lf cm, petal_length: %10.5Lf cm, petal_width: %10.5Lf cm\n",
                    sepal_length / offset,
                    sepal_width / offset,
                    petal_length / offset,
                    petal_width / offset
                );

                free(iris_array);

            } else {
                fprintf_s(stderr, "Unable to allocate memory, malloc returned NULL.\n");
                exit(1);
            }
        } else {
            fprintf_s(stderr, "Unable to read ./iris.data, Error %lu in CreateFileW.\n", GetLastError());
            exit(1);
        }
    } else {
        fprintf_s(stderr, "Unable to open ./iris.data, Error %lu in ReadFile.\n", GetLastError());
        exit(1);
    }
    exit(0);
}
