#pragma once

// Find current platform
#ifdef _WIN32
  #ifdef _WIN64
    #define FND_PLATFORM_WINDOWS64
  #else
    #error "x86 builds are not supported!"
  #endif
#else
  #error "Only Windows is supported!"
#endif

// Macro for exporting/importing Windows DLLs
// Taken from https://atomheartother.github.io/c++/2018/07/12/CPPDynLib.html
#ifdef FND_BUILD_SHARED
  #ifdef FND_EXPORT
  // Exporting
    #ifdef __GNUC__
      #define FND_API __attribute__ ((dllexport))
    #else
      #define FND_API __declspec(dllexport)
    #endif
  #else
  // Importing
    #ifdef __GNUC__
      #define FND_API __attribute__ ((dllimport))
    #else
      #define FND_API __declspec(dllimport)
    #endif
  #endif
#else
    #define FND_API
#endif

// Other useful macros
#define BIT(x) (1u << x)