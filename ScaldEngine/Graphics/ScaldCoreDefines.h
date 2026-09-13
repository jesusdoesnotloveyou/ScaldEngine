#pragma once

#if defined(WIN32) || defined(_WINDOWS)
    #ifdef _EXPORTING
        #define SCALD_API __declspec(dllexport)
    #elif defined(_IMPORTING)
        #define SCALD_API __declspec(dllimport)
    #else
        #define SCALD_API
    #endif
#elif defined(__linux__)
        #define SCALD_API __attribute__((visibility("default")))
#endif

#ifndef FORCEINLINE
    #define FORCEINLINE __forceinline
#endif FORCEINLINE

#define SAFE_RELEASE(p)     \
    {                       \
        if (p)              \
        {                   \
            (p)->Release(); \
            (p) = nullptr;  \
        }                   \
    }
