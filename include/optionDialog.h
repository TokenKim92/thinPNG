#ifndef _OPTION_DIALOG_H_
#define _OPTION_DIALOG_H_

#include "WindowDialog.h"

class OptionDialog : public WindowDialog
{
protected:
	RECT m_viewRect;
	unsigned int &m_width;
	unsigned int &m_height;

	IDWriteTextFormat *mp_titleFont;
	IDWriteTextFormat *mp_subtitleFont;
	IDWriteTextFormat *mp_textFont;
	IDWriteTextFormat *mp_buttonFont;
	const float m_subtitleFontSize = 13.0f;

	DRect m_titleRect;
	DRect m_ratioTitleRect;
	DRect m_ratioWidthRect;
	DRect m_ratioHeightRect;
	DRect m_widthRect;
	DRect m_heightRect;
	DRect m_buttonBackgroundRect;
	DRect m_saveButtonRect;
	DRect m_cancelButtonRect;

	DColor m_textColor;
	DColor m_saveButtonColor;
	DColor m_cancelButtonColor;

	bool m_hoverOnButton;
	bool m_clickedOnButton;

public:
	OptionDialog(unsigned int &a_width, unsigned int &a_height);
	virtual ~OptionDialog();

protected:
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
	virtual void OnSetThemeMode() override;

	// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
	int MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONDOWN message that occurs when a window is destroyed
	int MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONUP message that occurs when a window is destroyed
	int MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam);

private:
	void InitRects();

	void DrawTitle();
	void DrawRatioSelector();
	void DrawRadioButton(const std::wstring &a_title, const bool isClicked, const DRect &a_rect);
	void DrawEditControl(const std::wstring &a_title, const unsigned int a_value, const DRect &a_rect);
	void DrawSaveAndCancelButton();
};

#endif //_OPTION_DIALOG_H_