#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#ifndef STRING_TYPE
#define STRING_TYPE             std::string
#define STRING_TYPE_INCLUDE     <string>
#endif // STRING_TYPE

#ifdef DEBUG
#define debug(...)  fprintf(stderr, __VA_ARGS__)

#define SOFT_ASSERT(a) if(a)

#else // DEBUG
#define debug(...)  (void(0))
#define cerr if(0)cerr
#define SOFT_ASSERT(a) if(0)
#endif // DEBUG

#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32)
#define _UNIX
#define PLATFORM_LINUX
#else
#define PLATFORM_WINDOWS
#endif


#endif // COMMONUTILS_H
