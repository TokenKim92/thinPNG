#ifndef _RESIZE_D2D_H_
#define _RESIZE_D2D_H_

#include "Direct2DEx.h"

class ResizeD2D : public Direct2DEx
{
public:
	ResizeD2D(HWND ah_wnd, const RECT *const ap_viewRect);
	virtual ~ResizeD2D();

	void ResizeImage(const std::wstring &a_filePath, unsigned int a_width, unsigned int a_height);
	
protected:
	IWICFormatConverter *ScaleImageToBitmap(const std::wstring &a_filePath, unsigned int a_width, unsigned int a_height);
	void SaveBitmapToFile(const std::wstring &a_filePath, IWICFormatConverter *const ap_bitmap);
};

#endif // !_SCREEN_D2D_H_
