#pragma once

#include "IUnityInterface.h"
#include "IUnityGraphics.h"


// Which platform we are on?
#if _MSC_VER
#define UNITY_WIN 1
#elif defined(__APPLE__)
#if defined(__arm__)
#define UNITY_IPHONE 1
#else
#define UNITY_OSX 1
#endif
#elif defined(__linux__)
#define UNITY_LINUX 1
#elif defined(UNITY_METRO) || defined(UNITY_ANDROID)
// these are defined externally
#else
#error "Unknown platform!"
#endif



// Which graphics device APIs we possibly support?
#if UNITY_METRO
#define SUPPORT_D3D11 1
#elif UNITY_WIN
#define SUPPORT_D3D9 1
#define SUPPORT_D3D11 1 // comment this out if you don't have D3D11 header/library files
#ifdef _MSC_VER
#if _MSC_VER >= 1900
#define SUPPORT_D3D12 1
#endif
#endif
#define SUPPORT_OPENGL 1
#elif UNITY_IPHONE || UNITY_ANDROID
#define SUPPORT_OPENGLES 1
#elif UNITY_OSX || UNITY_LINUX
#define SUPPORT_OPENGL 1
#endif
