#pragma once


#ifdef CONFIG_READER_EXP
#define CONFIG_READER_EXPORT_SYMBOL __declspec (dllexport)
#define CONFIG_READER_EXPIMP_TEMPLATE
#else
#ifdef CONFIG_READER_STANDALONE
#define CONFIG_READER_EXPORT_SYMBOL 
#define CONFIG_READER_EXPIMP_TEMPLATE 
#else
#define CONFIG_READER_EXPORT_SYMBOL __declspec (dllimport)
#define CONFIG_READER_EXPIMP_TEMPLATE extern
#endif
#endif // CONFIG_READER_EXP

#ifdef LAUNCHER__EXP
#define EXP_STATIC_VAR extern "C" __declspec(dllexport)
#else
#define EXP_STATIC_VAR extern "C" __declspec(dllimport)
#endif

