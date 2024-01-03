#ifndef _THIN_PNG_DIALOG_H_
#define _THIN_PNG_DIALOG_H_

#include "WindowDialog.h"

class thinPNG : public WindowDialog
{
protected:
	RECT m_viewRect;
	unsigned int m_resizeWidth;
	unsigned int m_resizeHeight;

	ID2D1StrokeStyle *mp_dashStrokeStyle;
	IDWriteTextFormat *mp_gridFont;
	IDWriteTextFormat *mp_optionFont;

	DRect m_gridRect;
	DRect m_optionButtonRect;
	DColor m_textColor;
	DColor m_saveButtonColor;
	DColor m_buttonBorderColor;

	bool m_hoverOnOptionButton;
	bool m_clickedOnOptionButton;

public:
	thinPNG();
	virtual ~thinPNG();

protected:
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
	virtual void OnSetThemeMode() override;

	void DrawField();
	void DrawOptionButton();

	// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
	int MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONDOWN message that occurs when a window is destroyed
	int MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONUP message that occurs when a window is destroyed
	int MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_DROPFILES message that occurs when a window is destroyed
	int DropFilesHandler(WPARAM a_wordParam, LPARAM a_longParam);
};

#endif //_THIN_PNG_DIALOG_H_