#include <stdio.h>
#include <stdint.h>

// all works fine with clang and MSVC :)

// without struct packing, using raw offsets caluclated using sizeof() on members won't help much when paddings are involved
#pragma pack(push, 1)
typedef struct something{
	unsigned long x;	
	long double y; 	
	char z[10];	
} something_t;
#pragma pack(pop)

typedef struct unpacked{
	unsigned long x;	// 4 bytes padding added after data member x
	long double y; 	
	char z[10];	// 6 bytes padding added after data member z
} unpacked_t;

static inline something_t __stdcall GetSomething(void){
	return (something_t) {.x = 12345, .y = 6.3764527645L, .z = {"Anoban"}};
}

static inline unpacked_t __stdcall GetUnpacked(void){
	return (unpacked_t) {.x = 12345, .y = 6.3764527645L, .z = {"Anoban"}};
}

int main(void){
	
	// const something_t* lvalue = &(GetSomething());	// Error: cannot take the address of an rvalue of type 'something_t' (aka 'struct something')
	const something_t some = GetSomething();
	const unsigned long x = GetSomething().x;
	printf_s("%lu\n", x);
	
	char* const as = GetSomething().z;
	puts(as);
	
	// let's try some trickery
	// if the return values of GetSomething() were stored in memory, unused member variables could be addressed too!!
	const long double* const maybe_y = (long double*) (as - sizeof(long double));
	printf_s("%.10Lf 6.3764527645\n", *maybe_y);	// access violations huh?
	const unsigned long* const maybe_x = (unsigned long*) (as - sizeof(long double) - sizeof(unsigned long));
	printf_s("%lu 12345\n", *maybe_x);	// we are winning son!
	// fucking works woohooo!!!!!
	
	const char* const name = GetUnpacked().z;
	puts(name);	// will work fine :)
	// now is the tricky part
	const long double* const willbe_y = (long double*) (name - sizeof(long double));	// will work because no padding bytes were added after y
	const unsigned long * const willbe_x = (unsigned long *) (name - sizeof(long double) - sizeof(unsigned long) - 4);	// will work because the 4 padding bytes added after x are accounted for
	printf_s("%.10Lf 6.3764527645\n", *willbe_y);
	printf_s("%lu 12345\n", *willbe_x);
	
	return 0;
}