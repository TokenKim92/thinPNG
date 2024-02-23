#pragma once

#include "OptionDialog.h"
#include "Direct2DEx.h"

class ResizeD2D : public Direct2DEx
{
public:
	ResizeD2D(HWND ah_wnd, const RECT *const ap_viewRect);
	virtual ~ResizeD2D() = default;

	void ResizeImage(const std::wstring &a_filePath, unsigned int a_size, const OptionDialog::CONTROL_TYPE &a_selectedRadioType);
	
protected:
	IWICFormatConverter *ScaleImageToBitmap(const std::wstring &a_filePath, unsigned int a_size, const OptionDialog::CONTROL_TYPE &a_selectedRadioType);
	void SaveBitmapToFile(const std::wstring &a_filePath, IWICFormatConverter *const ap_bitmap);
};

