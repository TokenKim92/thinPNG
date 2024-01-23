#ifndef _OPTION_DIALOG_H_
#define _OPTION_DIALOG_H_

#include "WindowDialog.h"

class OptionDialog : public WindowDialog
{
public:
	enum CONTROL_TYPE {
		NONE = 0,
		WIDTH_RADIO,
		HEIGHT_RADIO,
		SIZE_EDIT,
		SAVE_BUTTON,
		CANCEL_BUTTON
	};

protected:
	RECT m_viewRect;
	unsigned int m_size;
	unsigned int m_tempSize;
	CONTROL_TYPE m_selectedRadioType;
	CONTROL_TYPE m_tempSelectedRadioType;

	IDWriteTextFormat *mp_titleFont;
	IDWriteTextFormat *mp_subtitleFont;
	IDWriteTextFormat *mp_textFont;
	IDWriteTextFormat *mp_buttonFont;
	const float m_subtitleFontSize;

	DRect m_titleRect;
	DRect m_ratioTitleRect;
	DRect m_radioWidthRect;
	DRect m_radioHeightRect;
	DRect m_sizeEditRect;
	DRect m_buttonBackgroundRect;
	DRect m_saveButtonRect;
	DRect m_cancelButtonRect;
	DRect m_warningRect;

	DColor m_textColor;
	DColor m_lightBackgroundColor;
	DColor m_backgroundColor;
	DColor m_darkBackgroundColor;
	DColor m_highlightColor;
	DColor m_shadowColor;
	const float m_defaultTransparency;

	bool m_hoverOnButton;
	bool m_clickedOnButton;
	CONTROL_TYPE m_hoverArea;
	CONTROL_TYPE m_clickedArea;
	std::map<CONTROL_TYPE, DRect> m_controlTable;

public:
	OptionDialog(unsigned int a_size, const CONTROL_TYPE &a_selectedRadioType);
	virtual ~OptionDialog();

	unsigned int GetSizeValue();
	CONTROL_TYPE GetRatioType();

protected:
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;

	// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
	int MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONDOWN message that occurs when a window is destroyed
	int MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_LBUTTONUP message that occurs when a window is destroyed
	int MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam);
	// to handle the WM_KEYDOWN message that occurs when a window is destroyed
	int KeyDownHandler(WPARAM a_wordParam, LPARAM a_longParam);

private:
	void InitRects(); 
	void InitColors();

	void OnNumberKeyDown(const unsigned char a_pressedKey, const unsigned char a_offset);
	void OnControlDown(const CONTROL_TYPE &a_buttonType);
	void OnButtonControlUp(const CONTROL_TYPE &a_buttonType);
	void OnRadioControlUp(const CONTROL_TYPE &a_buttonType);

	void DrawUserText(const std::wstring &a_text, const DRect &a_rect, IDWriteTextFormat * const ap_textFormat);
	void DrawRadioButton(const std::wstring &a_title, const DRect &a_rect, const CONTROL_TYPE &a_type);
	void DrawSizeEdit();
	void DrawSaveButton();
	void DrawCancelButton();
};

#endif //_OPTION_DIALOG_H_