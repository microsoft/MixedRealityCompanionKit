#pragma once

#include "pch.h"

inline HRESULT GetVideoResolution(
	_In_ IMediaEncodingProfile* mediaEncodingProfile,
	_Out_ UINT32* width,
	_Out_ UINT32* height) throw()
{
	NULL_CHK(mediaEncodingProfile);
	NULL_CHK(width);
	NULL_CHK(height);

	ComPtr<IVideoEncodingProperties> spVideoEncodingProperties;
	IFR(mediaEncodingProfile->get_Video(&spVideoEncodingProperties));

	IFR(spVideoEncodingProperties->get_Height(height));
	IFR(spVideoEncodingProperties->get_Width(width));

	return S_OK;
}


