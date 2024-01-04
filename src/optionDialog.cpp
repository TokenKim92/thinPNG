#include "optionDialog.h"
#include "ColorPalette.h"
#include "utility.h"

#ifdef _DEBUG
#pragma comment (lib, "AppTemplateDebug.lib")
#else
#pragma comment (lib, "AppTemplate.lib")     
#endif

OptionDialog::OptionDialog(unsigned int a_size, const CONTROL_TYPE &a_selectedRadioType) :
	WindowDialog(L"OPTIONDILAOG", L"optionDialog"),
	m_size(a_size),
	m_tempSize(a_size),
	m_selectedRadioType(a_selectedRadioType),
	m_tempSelectedRadioType(a_selectedRadioType),
	m_subtitleFontSize(13.0f),
	m_defaultTransparency(0.7f)
{
	memset(&m_viewRect, 0, sizeof(RECT));
	
	mp_titleFont = nullptr;
	mp_subtitleFont = nullptr;
	mp_textFont = nullptr;
	mp_buttonFont = nullptr;

	memset(&m_titleRect, 0, sizeof(DRect));
	memset(&m_ratioTitleRect, 0, sizeof(DRect));
	memset(&m_radioWidthRect, 0, sizeof(DRect));
	memset(&m_radioHeightRect, 0, sizeof(DRect));
	memset(&m_sizeEditRect, 0, sizeof(DRect));
	memset(&m_buttonBackgroundRect, 0, sizeof(DRect));
	memset(&m_saveButtonRect, 0, sizeof(DRect));
	memset(&m_cancelButtonRect, 0, sizeof(DRect));
	memset(&m_warningRect, 0, sizeof(DRect));

	memset(&m_textColor, 0, sizeof(DColor));
	memset(&m_lightBackgroundColor, 0, sizeof(DRect));
	memset(&m_backgroundColor, 0, sizeof(DRect));
	memset(&m_darkBackgroundColor, 0, sizeof(DRect));
	memset(&m_highlightColor, 0, sizeof(DRect));
	memset(&m_shadowColor, 0, sizeof(DRect));
	
	m_hoverOnButton = false;
	m_clickedOnButton = false;
	m_hoverArea = CONTROL_TYPE::NONE;
	m_clickedArea = CONTROL_TYPE::NONE;
}

OptionDialog::~OptionDialog()
{

}

void OptionDialog::InitRects()
{
	::GetClientRect(mh_window, &m_viewRect);

	const float margin = 20.0f;
	const POINT centerPos{ m_viewRect.right / 2, m_viewRect.bottom / 2 };

	// title rect
	m_titleRect.left = centerPos.x - 40.0f;
	m_titleRect.top = margin;
	m_titleRect.right = centerPos.x + 40.0f;
	m_titleRect.bottom = m_titleRect.top + 20.0f;

	// ratio title rect
	m_ratioTitleRect.left = margin * 2.0f;
	m_ratioTitleRect.right = m_viewRect.right - margin * 2.0f;
	m_ratioTitleRect.top = m_titleRect.bottom + margin;
	m_ratioTitleRect.bottom = m_ratioTitleRect.top + 20.0f;

	const float radioButtonWidth = 100.0f;
	const float radioButtonHeight = 30.0f;
	// ratio width rect
	m_radioWidthRect.left = margin * 2.0f;
	m_radioWidthRect.right = m_radioWidthRect.left + radioButtonWidth;
	m_radioWidthRect.top = m_ratioTitleRect.bottom;
	m_radioWidthRect.bottom = m_radioWidthRect.top + radioButtonHeight;
	// ratio height rect
	m_radioHeightRect.left = centerPos.x + margin;
	m_radioHeightRect.right = m_radioHeightRect.left + radioButtonWidth;
	m_radioHeightRect.top = m_ratioTitleRect.bottom;
	m_radioHeightRect.bottom = m_radioHeightRect.top + radioButtonHeight;

	// width edit rect
	m_sizeEditRect.left = margin * 2.0f;
	m_sizeEditRect.right = centerPos.x - margin;
	m_sizeEditRect.top = m_radioWidthRect.bottom + margin;
	m_sizeEditRect.bottom = m_sizeEditRect.top + 50.0f;

	const float buttonHeight = 35.0f;
	// buttons background rect
	m_buttonBackgroundRect.left = 0.0f;
	m_buttonBackgroundRect.top = m_viewRect.bottom - buttonHeight - margin * 2.0f;
	m_buttonBackgroundRect.right = static_cast<float>(m_viewRect.right);
	m_buttonBackgroundRect.bottom = static_cast<float>(m_viewRect.bottom);

	// save button rect
	m_saveButtonRect.left = margin;
	m_saveButtonRect.right = centerPos.x - margin / 2.0f;
	m_saveButtonRect.bottom = m_viewRect.bottom - margin;
	m_saveButtonRect.top = m_saveButtonRect.bottom - buttonHeight;
	// cancel button rect
	m_cancelButtonRect.left = centerPos.x + margin / 2.0f;
	m_cancelButtonRect.right = m_viewRect.right - margin;
	m_cancelButtonRect.bottom = m_viewRect.bottom - margin;
	m_cancelButtonRect.top = m_cancelButtonRect.bottom - buttonHeight;

	// warning rect
	m_warningRect.left = margin * 2.0f;
	m_warningRect.right = m_viewRect.right - margin * 2.0f;
	m_warningRect.top = m_sizeEditRect.bottom;
	m_warningRect.bottom = m_buttonBackgroundRect.top;
}

void OptionDialog::InitColors()
{
	if (THEME_MODE::LIGHT_MODE == m_themeMode) {
		mp_direct2d->SetBackgroundColor(RGB_TO_COLORF(NEUTRAL_200));

		m_textColor = RGB_TO_COLORF(NEUTRAL_900);
		m_lightBackgroundColor = RGB_TO_COLORF(NEUTRAL_100);
		m_backgroundColor = RGB_TO_COLORF(NEUTRAL_200);
		m_darkBackgroundColor = RGB_TO_COLORF(NEUTRAL_300);
		m_highlightColor = RGB_TO_COLORF(ORANGE_300);
		m_shadowColor = RGB_TO_COLORF(NEUTRAL_400);
	}
	else {
		m_textColor = RGB_TO_COLORF(NEUTRAL_100);
		m_lightBackgroundColor = RGB_TO_COLORF(NEUTRAL_700);
		m_backgroundColor = RGB_TO_COLORF(NEUTRAL_800);
		m_darkBackgroundColor = RGB_TO_COLORF(NEUTRAL_900);
		m_highlightColor = RGB_TO_COLORF(SKY_400);
		m_shadowColor = RGB_TO_COLORF(NEUTRAL_500);
	}

	m_lightBackgroundColor.a = m_defaultTransparency;
	m_backgroundColor.a = m_defaultTransparency;
	m_darkBackgroundColor.a = m_defaultTransparency;
	m_highlightColor.a = m_defaultTransparency;
	m_shadowColor.a = m_defaultTransparency;
}

void OptionDialog::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();

	InitRects();
	InitColors();
	m_controlTable = {
		{CONTROL_TYPE::WIDTH_RADIO, m_radioWidthRect},
		{CONTROL_TYPE::HEIGHT_RADIO, m_radioHeightRect},
		{CONTROL_TYPE::SIZE_EDIT, m_sizeEditRect},
		{CONTROL_TYPE::SAVE_BUTTON, m_saveButtonRect},
		{CONTROL_TYPE::CANCEL_BUTTON, m_cancelButtonRect},
	};

	// create title font
	mp_titleFont = mp_direct2d->CreateTextFormat(DEFAULT_FONT_NAME, 20.0f, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL);
	mp_titleFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mp_titleFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	// create subtitle font
	mp_subtitleFont = mp_direct2d->CreateTextFormat(DEFAULT_FONT_NAME, m_subtitleFontSize, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL);
	// text subtitle font
	mp_textFont = mp_direct2d->CreateTextFormat(DEFAULT_FONT_NAME, 15.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL);
	mp_textFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	// button text font
	mp_buttonFont = mp_direct2d->CreateTextFormat(DEFAULT_FONT_NAME, 15.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL);
	mp_buttonFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mp_buttonFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	
	// add message handlers
	AddMessageHandler(WM_MOUSEMOVE, static_cast<MessageHandler>(&OptionDialog::MouseMoveHandler));
	AddMessageHandler(WM_LBUTTONDOWN, static_cast<MessageHandler>(&OptionDialog::MouseLeftButtonDownHandler));
	AddMessageHandler(WM_LBUTTONUP, static_cast<MessageHandler>(&OptionDialog::MouseLeftButtonUpHandler));
	AddMessageHandler(WM_KEYDOWN, static_cast<MessageHandler>(&OptionDialog::KeyDownHandler));
}

void OptionDialog::OnDestroy()
{
	InterfaceRelease(&mp_titleFont);
	InterfaceRelease(&mp_subtitleFont);
	InterfaceRelease(&mp_textFont);
	InterfaceRelease(&mp_buttonFont);
}

void OptionDialog::OnPaint()
{
	mp_direct2d->Clear();

	// draw title section
	DrawUserText(L"Resize", m_titleRect, mp_titleFont);

	// draw radio button section
	DrawUserText(L"Lock aspect ratio per:", m_ratioTitleRect, mp_subtitleFont);
	DrawRadioButton(L"Width", m_radioWidthRect, CONTROL_TYPE::WIDTH_RADIO);
	DrawRadioButton(L"Height", m_radioHeightRect, CONTROL_TYPE::HEIGHT_RADIO);

	// draw size edit section
	DrawSizeEdit();

	// draw save and cencel button saection
	mp_direct2d->SetBrushColor(m_darkBackgroundColor);
	mp_direct2d->FillRectangle(m_buttonBackgroundRect);
	DrawSaveButton();
	DrawCancelButton();

	// draw warning section if the size is 0
	if (0 == m_tempSize) {
		auto prevTextFormat = mp_direct2d->SetTextFormat(mp_textFont);
		mp_direct2d->SetBrushColor(RGB_TO_COLORF(RED_300));
		mp_direct2d->DrawUserText(L"(!) Invalid number value.", m_warningRect);
		mp_direct2d->SetTextFormat(prevTextFormat);
	}
}

// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
int OptionDialog::MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	for (auto const &[type, rect] : m_controlTable) {
		if (PointInRectF(rect, pos)) {
			if (type != m_hoverArea) {
				m_hoverArea = type;
				::InvalidateRect(mh_window, &m_viewRect, true);
			}

			return S_OK;
		}
	}

	if (CONTROL_TYPE::NONE != m_hoverArea) {
		m_hoverArea = CONTROL_TYPE::NONE;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}

	return S_OK;
}

// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
int OptionDialog::MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };
	
	for (auto const &[type, rect] : m_controlTable) {
		if (PointInRectF(rect, pos)) {
			::SetCapture(mh_window);
			m_clickedArea = type;
			::InvalidateRect(mh_window, &m_viewRect, false);
			return S_OK;
		}
	}

	if (CONTROL_TYPE::NONE != m_clickedArea) {
		m_clickedArea = CONTROL_TYPE::NONE;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}

	return S_OK;
}

// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
int OptionDialog::MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam)
{	
	if (CONTROL_TYPE::NONE != m_clickedArea) {
		::ReleaseCapture();

		switch (m_clickedArea)
		{
		case CONTROL_TYPE::WIDTH_RADIO:
			OnRadioControlUp(CONTROL_TYPE::WIDTH_RADIO);
			break;
		case CONTROL_TYPE::HEIGHT_RADIO:
			OnRadioControlUp(CONTROL_TYPE::HEIGHT_RADIO);
			break;
		case CONTROL_TYPE::SIZE_EDIT:
			break;
		case CONTROL_TYPE::SAVE_BUTTON:
			OnButtonControlUp(CONTROL_TYPE::SAVE_BUTTON);
			break;
		case CONTROL_TYPE::CANCEL_BUTTON:
			OnButtonControlUp(CONTROL_TYPE::CANCEL_BUTTON);
			break;
		default:
			break;
		}
	}

	return S_OK;
}

// to handle the WM_KEYDOWN message that occurs when a window is destroyed
int OptionDialog::KeyDownHandler(WPARAM a_wordParam, LPARAM a_longParam) 
{
	static const unsigned char number0 = 0x30;
	static const unsigned char number9 = 0x39;
	const unsigned char pressedKey = static_cast<unsigned char>(a_wordParam);

	if (VK_RETURN == pressedKey) {
		m_hoverArea = CONTROL_TYPE::SAVE_BUTTON;
		OnButtonControlUp(CONTROL_TYPE::SAVE_BUTTON);
	}
	else if (VK_ESCAPE == pressedKey) {
		m_hoverArea = CONTROL_TYPE::CANCEL_BUTTON;
		OnButtonControlUp(CONTROL_TYPE::CANCEL_BUTTON);
	}

	if (CONTROL_TYPE::SIZE_EDIT == m_clickedArea) {
		if (number0 <= pressedKey && pressedKey <= number9) {
			OnNumberKeyDown(pressedKey, number0);
		}
		else if ((VK_NUMPAD0 <= pressedKey && pressedKey <= VK_NUMPAD9)) {
			OnNumberKeyDown(pressedKey, VK_NUMPAD0);
		}
		else if (VK_BACK == pressedKey) {
			if (0 != m_tempSize) {
				std::string numberText = std::to_string(m_tempSize);
				numberText.pop_back();
				m_tempSize = numberText.empty() ? 0 : std::stoi(numberText);

				::InvalidateRect(mh_window, &m_viewRect, false);
			}
		}
	}	

	return S_OK;
}

void OptionDialog::OnNumberKeyDown(const unsigned char a_pressedKey, const unsigned char a_offset)
{
	std::string previousNumber = std::to_string(m_tempSize);

	if (previousNumber.length() < 6) {
		std::string currentNumber = std::to_string(m_tempSize) + std::to_string(a_pressedKey - a_offset);
		m_tempSize = std::stoi(currentNumber);

		::InvalidateRect(mh_window, &m_viewRect, false);
	}
}

void OptionDialog::OnControlDown(const CONTROL_TYPE &a_buttonType)
{
	if (a_buttonType != m_clickedArea) {
		m_clickedArea = a_buttonType;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}
}

void OptionDialog::OnButtonControlUp(const CONTROL_TYPE &a_buttonType)
{
	if (a_buttonType == m_hoverArea) {
		// on click cancel button
		if (CONTROL_TYPE::CANCEL_BUTTON == a_buttonType) {
			::DestroyWindow(mh_window);
			return;
		}
		
		// on click save button
		if (0 != m_tempSize) {
			m_size = m_tempSize;
			m_selectedRadioType = m_tempSelectedRadioType;
			::DestroyWindow(mh_window);
		}		
	}
	else {
		m_clickedArea = CONTROL_TYPE::NONE;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}
}

void OptionDialog::OnRadioControlUp(const CONTROL_TYPE &a_buttonType)
{
	if (a_buttonType != m_clickedArea) {
		return;
	}

	if (a_buttonType == m_hoverArea) {
		if (a_buttonType != m_tempSelectedRadioType) {
			m_tempSelectedRadioType = a_buttonType;
			::InvalidateRect(mh_window, &m_viewRect, false);
		}
	}
	else {
		m_clickedArea = CONTROL_TYPE::NONE;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}
}

void OptionDialog::DrawUserText(const std::wstring &a_text, const DRect &a_rect, IDWriteTextFormat *const ap_textFormat)
{
	auto prevTextFormat = mp_direct2d->SetTextFormat(ap_textFormat);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(a_text.c_str(), a_rect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}

void OptionDialog::DrawRadioButton(const std::wstring &a_title, const DRect &a_rect, const CONTROL_TYPE &a_type)
{
	// darw radio button
	const float height = a_rect.bottom - a_rect.top;
	const float margin = 4.0f;
	DRect rect{ a_rect.left + margin, a_rect.top + margin, a_rect.left + height - margin, a_rect.bottom - margin };
	bool isHover = a_type == m_hoverArea;

	DColor color = m_darkBackgroundColor;
	if (isHover) {
		color.a = 1.0f;
	}

	mp_direct2d->SetBrushColor(color);
	mp_direct2d->FillEllipse(rect);

	if (a_type == m_tempSelectedRadioType) {
		const float strokeWidth = isHover ? 3.0f : 5.0f;
		mp_direct2d->SetStrokeWidth(strokeWidth);
		mp_direct2d->SetBrushColor(m_highlightColor);
		mp_direct2d->DrawEllipse(rect);
		mp_direct2d->SetStrokeWidth(1.0f);
	}
	else {
		mp_direct2d->SetBrushColor(m_lightBackgroundColor);
		mp_direct2d->DrawEllipse(rect);
	}

	rect.left += height;
	rect.right = a_rect.right;
	DrawUserText(a_title, rect, mp_textFont);
}

void OptionDialog::DrawSizeEdit()
{
	DColor editControlColor = CONTROL_TYPE::SIZE_EDIT == m_clickedArea 
		? m_darkBackgroundColor 
		: m_lightBackgroundColor;
	DColor editShadowColor = CONTROL_TYPE::SIZE_EDIT == m_clickedArea 
		? m_highlightColor 
		: m_shadowColor;	
	if (CONTROL_TYPE::SIZE_EDIT == m_hoverArea) {
		editControlColor.a = 1.0f;
		editShadowColor.a = 1.0f;
	}

	// draw title
	DrawUserText(L"Size (px)", m_sizeEditRect, mp_subtitleFont);

	// draw value contents background
	const float textOffset = m_subtitleFontSize + 7.0f;
	DRect rect{ m_sizeEditRect.left, m_sizeEditRect.bottom - 2.0f, m_sizeEditRect.right, m_sizeEditRect.bottom + 2.0f };
	mp_direct2d->SetBrushColor(editShadowColor);
	mp_direct2d->FillRoundedRectangle(rect, 3.0f);

	rect.top = m_sizeEditRect.top + textOffset;
	rect.bottom = m_sizeEditRect.bottom;
	mp_direct2d->SetBrushColor(editControlColor);
	mp_direct2d->FillRoundedRectangle(rect, 3.0f);

	// draw value contents
	const float margin = 10.0f;
	rect.left += margin;
	rect.right -= margin;
	DrawUserText(std::to_wstring(m_tempSize), rect, mp_textFont);
}

void OptionDialog::DrawSaveButton()
{
	DColor buttonColor;
	if (0 == m_tempSize) {
		buttonColor = m_backgroundColor;
	}
	else {
		buttonColor = m_highlightColor;
		if (CONTROL_TYPE::SAVE_BUTTON == m_hoverArea && CONTROL_TYPE::SAVE_BUTTON != m_clickedArea) {
			buttonColor.a = 1.0f;
		}
	}

	mp_direct2d->SetBrushColor(buttonColor);
	mp_direct2d->FillRoundedRectangle(m_saveButtonRect, 5.0f);
	DrawUserText(L"Save", m_saveButtonRect, mp_buttonFont);
}

void OptionDialog::DrawCancelButton()
{
	DColor buttonColor = m_lightBackgroundColor;
	if (CONTROL_TYPE::CANCEL_BUTTON == m_hoverArea && CONTROL_TYPE::CANCEL_BUTTON != m_clickedArea) {
		buttonColor.a = 1.0f;
	}

	mp_direct2d->SetBrushColor(buttonColor);
	mp_direct2d->FillRoundedRectangle(m_cancelButtonRect, 5.0f);
	DrawUserText(L"Cancel", m_cancelButtonRect, mp_buttonFont);
}

unsigned int OptionDialog::GetSize()
{
	return m_size;
}

OptionDialog::CONTROL_TYPE OptionDialog::GetRatioType()
{
	return m_selectedRadioType;
}