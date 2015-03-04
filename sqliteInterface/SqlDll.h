#ifdef SQLDLL_EXPORTS
#define SQLDLL_API __declspec(dllexport) 
#else
#define SQLDLL_API __declspec(dllimport) 
#endif