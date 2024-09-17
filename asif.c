// compilers can do optimizations that bypass or overstep the implementations by the programmer
// as long as the end results are identical

#define _USE_MATH_DEFINES
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define NBYTES 78568LLU

// NOLINTBEGIN(cppcoreguidelines-init-variables)

// the volatile keyword in C means that the object qualified as volatile may be accessed by different threads
// at instances unpredictable at compile time, hence the compilers cannot treat volatile objects lightly because
// optimizations to volatile objects could change the programme behaviour
volatile double    epsilon   = 1.0E-10; // these are defined globally to make the idea of multi-threaded access plausible
volatile long long hundred   = 100;
volatile bool      is_locked = false;

int wmain(void) {
    unsigned int update; // uninitialized
    update                   = 11;
    update                  *= 10;

    // the compiler could choose to treat the above three statements as
    unsigned int alt_update  = 110; // which is essentially the same as what's accomplished by those three lines

    // but if `update` were to be accessed between the two consecutive updates i.e if a function was to access the variable update when
    // it was 11, our optimization will have changed the expected behaviour!
    // HENCE, THE OPTIMIZATION IS NOT CONSISTENT WITH THE ORIGINAL IMPLEMENTATION BUT IT IS EVENTUALLY CONSISITENT WITH IT!

    unsigned long sum        = 0;
    for (unsigned i = 0; i < SHRT_MAX; ++i) sum = i;
    // the compiler might chose to optimize away the loop with
    unsigned long alt_sum = SHRT_MAX - 1;
    // here too, if some function were to access sum at i = 45, the optimization will not have been done by the compiler
    // THE OPTIMIZED VERSION BECOMES CONSISTENT WHEN i = SHRT_MAX - 1, HENCE THE EVENTUALLY CONSISTENCY

    float zero            = 0.0000;
    float one             = 1.0000;
    // the compiler is at liberty to reorder the two definitions as follows guranteed that there won't be any side effects
    float alt_one         = 1.0000;
    float alt_zero        = 0.0000;

    // but the compiler cannot reorder these two statements because that would change the sequence of expected side effects i.e IO
    _putws(L"Hi there!");
    _putws(L"How was your day?");

    // a different thread may have to access the variable in its original state
    epsilon      = 0.5E-5; // so, optimizing it as volatile double epsilon = 0.5E-5; would cause erroneous behaviour

    long long g  = hundred;
    long long gg = hundred; // there is a chance that the volatile variable hundred to be modified by a different thread before it is read
    // for this assignment. Hence an optimization like
    long long alt_g  = hundred;
    long long alt_gg = alt_g;
    // has a very different semantics because the statement long long _gg = _g; will not be able to materialize the possible mutations
    // the volatile variable hundred may have undergone since the first assignment in the variable _gg!

    bool predicate   = false;
    while (predicate) _putws(L"stil true :)"); // a potential infinite loop

    // compiler can optimize the above as
    if (predicate)
        while (true) _putws(L"stil true :)"); // same infinite loop
    // since the preicate is not updated inside the loop body, there's no point in checking it in every iteration
    // by evaluating the condition only once we may save some execution time

    while (is_locked) _putws(L"stil locked!");
    // above is not equivalent to the one below because is_locked is a volatile object
    if (is_locked)
        for (;;) _putws(L"stil locked!");

    // the following pair of expressions cannot be reordered
    volatile float vf0, vf1;
    vf0                = 7.78658726F;
    vf1                = 4.78658726F; // e.g. when a different thread reads vf1, it may expect vf0 to be in a certain state
    // that expectation will not be honoured if the compiler decides to reorder the assignments as follows

    vf1                = 4.78658726F; // when vf1 is accessed vf0 will be garbage
    vf0                = 7.78658726F;

    //
    double* shared_ptr = NULL;
    double* hdouble    = malloc(sizeof(double));
    assert(hdouble);

    *hdouble   = M_PI * _MAX_FNAME;
    shared_ptr = hdouble;
    // the above two line of statements cannot be reordered as below, because according to the original implementation logic,
    // shared_ptr will point to the value (M_PI * _MAX_FNAME) when it has been assigned hdouble
    // i.e a read of shared_ptr following its assignment from hdouble should be able to retrieve the value M_PI * _MAX_FNAME

    shared_ptr = hdouble; // that semantic is not valid here, if a read of shared_ptr happened here, it'll read garbage
    *hdouble   = M_PI * _MAX_FNAME;

    free(hdouble);
    hdouble = NULL;

    return EXIT_SUCCESS;
}

// volatile
// volatile objects can be used for objects that are to be accessed by multiple threads
// may used to represent a hardware component e.g. the status of a sensor pin in an embedded system whose state could change unpredictably
// may represent a memory mapped files where two consecutive reads can be optimized into one because there's a chance that the file may
// become modified on disk by a different process, invalidating the data read in by the first access

// NOLINTEND(cppcoreguidelines-init-variables)
