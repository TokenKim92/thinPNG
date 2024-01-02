#include "thinPNGDialog.h"
#include "ColorPalette.h"

#ifdef _DEBUG
#pragma comment (lib, "AppTemplateDebug.lib")
#else
#pragma comment (lib, "AppTemplate.lib")     
#endif

thinPNG::thinPNG() :
	WindowDialog(L"THINPNG", L"thinPNG")
{
	memset(&m_viewRect, 0, sizeof(RECT));
}

thinPNG::~thinPNG()
{
}

void thinPNG::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();

	::GetClientRect(mh_window, &m_viewRect);
}

void thinPNG::OnDestroy()
{

}

void thinPNG::OnPaint()
{
	mp_direct2d->Clear();
}

void thinPNG::OnSetThemeMode()
{

}

// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
int thinPNG::MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	return S_OK;
}

// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
int thinPNG::MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	return S_OK;
}

// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
int thinPNG::MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	return S_OK;
}

// to handle the WM_MOUSEWHEEL  message that occurs when a window is destroyed
int thinPNG::MouseWheelHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	short delta = GET_WHEEL_DELTA_WPARAM(a_wordParam);

	if (delta < 0 ) {
	}
	else {
	}

	return S_OK;
}
