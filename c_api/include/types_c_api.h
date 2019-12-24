#pragma once

#ifdef WINDOWSPC
	#define C_API_EXPORT __declspec(dllexport)
#else
	#define C_API_EXPORT
#endif

struct OrmaBufferObject;
struct OrmaAllocatorObject;