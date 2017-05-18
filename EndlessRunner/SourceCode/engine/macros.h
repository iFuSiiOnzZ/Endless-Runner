#pragma once

#ifndef ASSERT
    #define ASSERT(x, msg) do{ if(!(x)){ *(int *) 0 = 0; } }while(0);
#endif

#ifndef ARRAY_COUNT
    #define ARRAY_COUNT(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef MAX
    #define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
    #define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef CLAMP
    #define CLAMP(lo, v, hi) MIN(hi, MAX(v, lo))
#endif