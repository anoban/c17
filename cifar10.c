// open the CIFAR10 batches and serialize the images as bitmaps

// clang-format off
#define _AMD64_
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windef.h>
#include <wingdi.h>
#include <stdlib.h>
// clang-format on

// since all the images are of the same resolution let's use the same file and info headers!
static const BITMAPFILEHEADER CIFAR10_FHEAD   = { .bfType = 0, .bfSize = 0, .bfReserved1 = 0, .bfReserved2 = 0, .bfOffBits = 0 };
static const BITMAPINFOHEADER CIFAR10_INFHEAD = { 0 };

int main(void) {
    //
    return EXIT_SUCCESS;
}
