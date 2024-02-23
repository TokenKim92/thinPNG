#pragma once

#include "Direct2D.h"
#include <string>

struct FONT_FORMAT
{
	std::wstring name;
	float size;
	DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
	DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;
};

// possible draw text
class Direct2DEx : public Direct2D
{
public:
	static std::wstring DEFAULT_FONT_NAME;

protected:
	IDWriteTextFormat *mp_textFormat;				// font information used as output for strings
	IDWriteFontFace *mp_fontFace;

	FONT_FORMAT m_fontFormat;

public:
	Direct2DEx(const HWND &ah_window, const RECT *const ap_viewRect = nullptr);
	virtual ~Direct2DEx();

	// the return object of 'IDWriteTextFormat *' should be deleted from the user with the function 'InterfaceRelease'
	IDWriteTextFormat *CreateTextFormat(
		const std::wstring &a_fontName, float a_fontSize, DWRITE_FONT_WEIGHT a_fontWeight,
		DWRITE_FONT_STYLE a_fontStyle, DWRITE_FONT_STRETCH a_fontStretch = DWRITE_FONT_STRETCH_NORMAL, const wchar_t *ap_localName = L"en-us"
	);
	// the return object of 'IDWriteFontFace *' should be deleted from the user with the function 'InterfaceRelease'
	IDWriteFontFace *CreateFontFace(const wchar_t *const ap_name, const DWRITE_FONT_WEIGHT a_weight, const DWRITE_FONT_STYLE a_style);

	bool SetFontFormat(const FONT_FORMAT &a_format);
	bool SetFontName(const std::wstring &ap_name);
	bool SetFontSize(const float a_size);
	bool SetFontWeight(const DWRITE_FONT_WEIGHT a_weight);
	bool SetFontStyle(const DWRITE_FONT_STYLE a_style);

	void SetTextAlignment(const DWRITE_TEXT_ALIGNMENT a_hType, const DWRITE_PARAGRAPH_ALIGNMENT a_vType = DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	// return the previous object of `IDWriteTextFormat`
	IDWriteTextFormat *const SetTextFormat(IDWriteTextFormat *const ap_textFormat);
	// return the previous object of `IDWriteFontFace`
	IDWriteFontFace *const SetFontFace(IDWriteFontFace *const ap_fontFace);

	DSize GetTextExtent(const std::wstring &a_text, const float a_maxWidth = 0.0f, const float a_maxHeight = 0.0f);

protected:
	virtual bool CreateDeviceResources() override;
	virtual void DestroyDeviceResources() override;

	// the return object of `ID2D1PathGeometry *` should be deleted from the user with the function `InterfaceRelease`
	ID2D1PathGeometry *CreateTextPathGeometry(const std::wstring a_text, const float a_fontSize);

// drawing method
public:
	void DrawUserText(const std::wstring &a_text, const DRect &a_rect);
	DRect DrawTextOutline(const std::wstring &a_text, const DPoint &a_startPos, const float a_textHeight = 0.0f);
};