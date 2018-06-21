#pragma once

#include "CompositorConstants.h"

#ifdef COMPOSITORDLL_EXPORTS
#define EXPORT_STATIC_CONFIG __declspec(dllexport)
#else
#define EXPORT_STATIC_CONFIG __declspec(dllimport)
#endif

struct  FrameProviderStaticConfig
{
	EXPORT_STATIC_CONFIG static int width;
	EXPORT_STATIC_CONFIG static int height;
	EXPORT_STATIC_CONFIG static float fps;

	static size_t getFrameBuffSize()
	{
		return width * height * FRAME_BPP;
	}

	static size_t getFrameBuffSizeRaw()
	{
		return width * height * FRAME_BPP_RAW;
	}
};