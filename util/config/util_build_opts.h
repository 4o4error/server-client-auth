#pragma once


#ifdef UTIL_EXP
#define UTIL_EXPORT_SYMBOL __declspec (dllexport)
#define UTIL_EXPIMP_TEMPLATE
#else
#ifdef UTIL_STANDALONE
#define UTIL_EXPORT_SYMBOL 
#define UTIL_EXPIMP_TEMPLATE 
#else
#define UTIL_EXPORT_SYMBOL __declspec (dllimport)
#define UTIL_EXPIMP_TEMPLATE extern
#endif
#endif // UTIL_EXP

#ifdef LAUNCHER__EXP
#define EXP_STATIC_VAR extern "C" __declspec(dllexport)
#else
#define EXP_STATIC_VAR extern "C" __declspec(dllimport)
#endif

