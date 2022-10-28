#ifndef _UTILS_H_
#define _UTILS_H_
#include <stddef.h>
// get the offset of 'member' relative to the beginning of a struct 'type'
#define offsetof(type, member) ((size_t) (&((type*) 0)->member))

// get the pointer to the instance of struct 'type' from a 'member' 'ptr'
#define tostruct(ptr, type, member) \
  ((type*) ((char*) (ptr) - (offsetof(type, member))))

// get the 'array' length
#define lengthof(array) (sizeof(array) / sizeof((array)[0]))
#endif  // _UTILS_H_