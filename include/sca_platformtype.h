#ifndef SCA_PLATFORM_TYPE_H_
#define SCA_PLATFORM_TYPE_H_

#ifdef __cplusplus
#define EXTERNC         extern "C"
#else
#define EXTERNC
#endif

//
// Define the possible platform types
//

#define SCA_PLATFORM_TYPE_WINDOWS 1
#define SCA_PLATFORM_TYPE_LINUX 2
#define SCA_PLATFORM_TYPE_APPLE 3
#define SCA_PLATFORM_TYPE_APPLE_IOS 4
#define SCA_PLATFORM_TYPE_ANDROID 5

//
// Define the possible platforms sub-types.
//

#define SCA_PLATFORM_WIN32 1
#define SCA_PLATFORM_WIN64 2


//
// Define the active platform, based on the preprocessor settings
//

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#  define SCA_PLATFORM_TYPE SCA_PLATFORM_TYPE_WINDOWS
#  define SCA_PLATFORMSCA_PLATFORM_WIN32
#elif defined(__WIN64__) || defined(_WIN64) || defined(WIN64) || defined(x64)
#  define SCA_PLATFORM_TYPE SCA_PLATFORM_TYPE_WINDOWS
#  define SCA_PLATFORM SCA_PLATFORM_WIN64
#elif defined(__linux__) || defined(__linux)
#  define SCA_PLATFORM_TYPE SCA_PLATFORM_TYPE_LINUX
#endif

#if SCA_PLATFORM_TYPE == SCA_PLATFORM_TYPE_WINDOWS
  #ifdef SCA_EXP
    #define SCA_EXPORT_SYMBOL __declspec (dllexport)
    #define SCA_EXPIMP_TEMPLATE
  #else
  #ifdef SCA_STANDALONE
    #define SCA_EXPORT_SYMBOL 
    #define SCA_EXPIMP_TEMPLATE 
  #else
    #define SCA_EXPORT_SYMBOL __declspec (dllimport)
    #define SCA_EXPIMP_TEMPLATE extern
    #endif
 #endif 
#elif SCA_PLATFORM_TYPE == SCA_PLATFORM_TYPE_LINUX
#if __GNUC__ >= 4
#define SCA_EXPORT_SYMBOL          EXTERNC __attribute__ ((visibility("default")))
#define SCA_PRIVATE_SYMBOL       __attribute__((visibility("hidden")))
#else
#define SCA_EXPORT_SYMBOL          EXTERNC
#define SCA_EXPORT_SYMBOL
#endif
#endif

#endif // SCA_PLATFORM_TYPE_H_