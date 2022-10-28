#ifndef _UTIL_H_
#define _UTIL_H_
#include <stddef.h>
#ifndef offsetof
// get the offset of 'member' relative to the beginning of a struct 'type'
#  define offsetof(type, member) ((size_t) (&((type*) 0)->member))
#endif

#ifndef tostruct
// get the pointer to the instance of struct 'type' from a 'member' 'ptr'
#  define tostruct(ptr, type, member) \
    ((type*) ((char*) (ptr) - (offsetof(type, member))))
#endif

#ifndef lengthof
// get the 'array' length
#  define lengthof(array) (sizeof(array) / sizeof((array)[0]))
#endif
#endif  // _UTIL_H_