#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "./pca.h"

void static __forceinline __stdcall vectorStats(
    long double* array, int size, long double* mean, long double* std, long double* min, long double* max
) {
    long double _sum = 0.0, _ssqdiff = 0.0, _mean = 0.0;
    int         _min = *array, _max = *array;

    for (int i = 0; i < size; ++i) {
        if (*(array + i) < _min) _min = *(array + i);
        if (*(array + i) > _max) _max = *(array + i);
        _sum += *(array + i);
    }

    _mean = _sum / size;

    for (int i = 0; i < size; ++i) _ssqdiff += ((*(array + i) - _mean) * (*(array + i) - _mean));

    *min  = _min;
    *max  = _max;
    *mean = _mean;
    *std  = sqrtl(_ssqdiff / VECTOR_SZIE);

    return;
}

void static __forceinline __stdcall printVectorStats(long double* vector) {
    long double mean = 0.0, std = 0.0, min = 0.0, max = 0.0;

    vectorStats(vector, VECTOR_SZIE, &mean, &std, &min, &max);
    printf_s("C0: mean %10.5Lf, std %10.5Lf, min %10.5Lf, max %10.5Lf\n", mean, std, min, max);

    return;
}

// Destructive, modifies tahe arrays in-place.
// Mean centering the vector.
void static __forceinline __stdcall recenterVector(long double* vector) {
    long double mean = 0.0;

    for (int i = 0; i < VECTOR_SZIE; ++i) mean += *(vector + i);

    mean /= VECTOR_SZIE;

    for (int i = 0; i < VECTOR_SZIE; ++i) *(vector + i) -= mean;

    return;
}

// Once two vectors are mean-centered, a line of the equation y = ax needs to be fitted to the data.
// The line must go through the origin, no intercepts!
// This line should be the best possible fit to describe the data! (least deviation from the data points!)

// Passed vectors need to be mean centered.
long double sumSquaredDistances(long double slope, long double* vector_x, long double* vector_y) {
    // This function needs to compute the perpendicular distance between an actual data point in the x-y plane
    // and the fitted line.
    // For a point, we know the x and y coordinates.
}

int main(void) {
    printVectorStats(Table.c0);
    recenterVector(Table.c0);
    printVectorStats(Table.c0);
}