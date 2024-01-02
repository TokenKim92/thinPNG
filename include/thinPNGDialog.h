#ifndef _THIN_PNG_DIALOG_H_
#define _THIN_PNG_DIALOG_H_

#include "WindowDialog.h"

class thinPNG : public WindowDialog
{
protected:
	RECT m_viewRect;

public:
	thinPNG();
	virtual ~thinPNG();

protected:
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
	virtual void OnSetThemeMode() override;

	// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
	int MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
	int MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
	int MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_MOUSEWHEEL  message that occurs when a window is destroyed
	int MouseWheelHandler(WPARAM a_wordParam, LPARAM a_longParam);
};

#endif //_THIN_PNG_DIALOG_H_