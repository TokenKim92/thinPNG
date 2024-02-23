#pragma once

#include "WindowDialog.h"
#include "optionDialog.h"

class ThinPNGDialog : public WindowDialog
{
protected:
	RECT m_viewRect;
	unsigned int m_size;
	OptionDialog::CONTROL_TYPE m_selectedRatioType;

	ID2D1StrokeStyle *mp_dashStrokeStyle;
	IDWriteTextFormat *mp_gridFont;
	IDWriteTextFormat *mp_optionFont;

	DRect m_gridRect;
	DRect m_optionButtonRect;
	DColor m_textColor;
	DColor m_saveButtonColor;
	DColor m_buttonBorderColor;

	bool m_hoverOnOptionButton;

public:
	ThinPNGDialog();
	virtual ~ThinPNGDialog() = default;

protected:
	virtual void OnAddEventListener() override;
	virtual void OnInitDialog() override;
	virtual void OnDestroy() override;
	virtual void OnPaint() override;
	virtual void OnSetColorMode() override;

	void DrawField();
	void DrawOptionButton();
};
