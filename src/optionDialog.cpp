#include "optionDialog.h"
#include "ColorPalette.h"

#ifdef _DEBUG
#pragma comment (lib, "AppTemplateDebug.lib")
#else
#pragma comment (lib, "AppTemplate.lib")     
#endif

OptionDialog::OptionDialog(unsigned int &a_width, unsigned int &a_height) :
	WindowDialog(L"OPTIONDILAOG", L"optionDialog"),
	m_width(a_width),
	m_height(a_height)
{
	memset(&m_viewRect, 0, sizeof(RECT));
	
	mp_titleFont = nullptr;
	mp_subtitleFont = nullptr;
	mp_textFont = nullptr;
	mp_buttonFont = nullptr;

	memset(&m_titleRect, 0, sizeof(DRect));
	memset(&m_ratioTitleRect, 0, sizeof(DRect));
	memset(&m_ratioWidthRect, 0, sizeof(DRect));
	memset(&m_ratioHeightRect, 0, sizeof(DRect));
	memset(&m_widthRect, 0, sizeof(DRect));
	memset(&m_heightRect, 0, sizeof(DRect));
	memset(&m_buttonBackgroundRect, 0, sizeof(DRect));
	memset(&m_saveButtonRect, 0, sizeof(DRect));
	memset(&m_cancelButtonRect, 0, sizeof(DRect));

	m_textColor = RGB_TO_COLORF(NEUTRAL_100);
	m_saveButtonColor = RGB_TO_COLORF(SKY_400);
	m_cancelButtonColor = RGB_TO_COLORF(NEUTRAL_800);
	
	m_hoverOnButton = false;
	m_clickedOnButton = false;
}

OptionDialog::~OptionDialog()
{

}

void OptionDialog::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();

	InitRects();

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

	DrawTitle();

	DrawRatioSelector();
	
	DrawEditControl(L"Width (px)", m_width, m_widthRect);
	DrawEditControl(L"Height (px)",m_height, m_heightRect);

	DrawSaveAndCancelButton();
}

void OptionDialog::OnSetThemeMode()
{
	if (THEME_MODE::LIGHT_MODE == GetThemeMode()) {

	}
	else {

	}
	::InvalidateRect(mh_window, &m_viewRect, false);
}

// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
int OptionDialog::MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };
	return S_OK;
}

// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
int OptionDialog::MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	return S_OK;
}

// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
int OptionDialog::MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam)
{

	return S_OK;
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

	const float radioButtonHeight = 30.0f;
	// ratio width rect
	m_ratioWidthRect.left = margin * 2.0f;
	m_ratioWidthRect.right = centerPos.x - margin;
	m_ratioWidthRect.top = m_ratioTitleRect.bottom;
	m_ratioWidthRect.bottom = m_ratioWidthRect.top + radioButtonHeight;
	// ratio height rect
	m_ratioHeightRect.left = centerPos.x + margin;
	m_ratioHeightRect.right = m_viewRect.right - margin * 2.0f;
	m_ratioHeightRect.top = m_ratioTitleRect.bottom;
	m_ratioHeightRect.bottom = m_ratioHeightRect.top + radioButtonHeight;

	const float editControlHeight = 50.0f;
	// width edit rect
	m_widthRect.left = margin * 2.0f;
	m_widthRect.right = centerPos.x - margin;
	m_widthRect.top = m_ratioWidthRect.bottom + margin;
	m_widthRect.bottom = m_widthRect.top + editControlHeight;
	// height edit rect
	m_heightRect.left = centerPos.x + margin;
	m_heightRect.right = m_viewRect.right - margin * 2.0f;
	m_heightRect.top = m_ratioWidthRect.bottom + margin;
	m_heightRect.bottom = m_heightRect.top + editControlHeight;

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
}

void OptionDialog::DrawTitle()
{
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_titleFont);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(L"Resize", m_titleRect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}

void OptionDialog::DrawRatioSelector()
{
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_subtitleFont);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(L"Lock aspect ratio per:", m_ratioTitleRect);
	mp_direct2d->SetTextFormat(prevTextFormat);

	DrawRadioButton(L"Width", true, m_ratioWidthRect);
	DrawRadioButton(L"Height", false, m_ratioHeightRect);
}

void OptionDialog::DrawRadioButton(const std::wstring &a_title, const bool isClicked, const DRect &a_rect)
{
	// darw radio button
	const float height = a_rect.bottom - a_rect.top;
	const float margin = 4.0f;
	DRect rect{ a_rect.left + margin, a_rect.top + margin, a_rect.left + height - margin, a_rect.bottom - margin };
	
	mp_direct2d->SetBrushColor(RGB_TO_COLORF(NEUTRAL_900));
	mp_direct2d->FillEllipse(rect);

	if (isClicked) {
		mp_direct2d->SetBrushColor(m_saveButtonColor);
		mp_direct2d->SetStrokeWidth(5.0f);
		mp_direct2d->DrawEllipse(rect);
		mp_direct2d->SetStrokeWidth(1.0f);
	}
	else {
		mp_direct2d->SetBrushColor(RGB_TO_COLORF(NEUTRAL_700));
		mp_direct2d->DrawEllipse(rect);
	}

	// draw text
	rect.left += height;
	rect.right = a_rect.right;
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_textFont);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(a_title.c_str(), rect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}

void OptionDialog::DrawEditControl(const std::wstring &a_title, const unsigned int a_value, const DRect &a_rect)
{
	// draw title
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_subtitleFont);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(a_title.c_str(), a_rect);
	mp_direct2d->SetTextFormat(prevTextFormat);

	// draw value contents background
	const float textOffset = m_subtitleFontSize + 7.0f;
	DRect rect{ a_rect.left, a_rect.bottom - 2.0f, a_rect.right, a_rect.bottom + 2.0f };
	mp_direct2d->SetBrushColor(RGB_TO_COLORF(NEUTRAL_500));
	mp_direct2d->FillRoundedRectangle(rect, 3.0f);

	rect.top = a_rect.top + textOffset;
	rect.bottom = a_rect.bottom;
	mp_direct2d->SetBrushColor(RGB_TO_COLORF(NEUTRAL_700));
	mp_direct2d->FillRoundedRectangle(rect, 3.0f);

	// draw value contents
	const float margin = 10.0f;
	rect.left += margin;
	rect.right -= margin;
	prevTextFormat = mp_direct2d->SetTextFormat(mp_textFont);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(std::to_wstring(a_value).c_str(), rect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}

void OptionDialog::DrawSaveAndCancelButton()
{
	// draw background
	mp_direct2d->SetBrushColor(RGB_TO_COLORF(NEUTRAL_900));
	mp_direct2d->FillRectangle(m_buttonBackgroundRect);

	// draw save button
	mp_direct2d->SetBrushColor(m_saveButtonColor);
	mp_direct2d->FillRoundedRectangle(m_saveButtonRect, 5.0f);
	mp_direct2d->SetBrushColor(m_textColor);

	// draw cancel button
	mp_direct2d->SetBrushColor(m_cancelButtonColor);
	mp_direct2d->FillRoundedRectangle(m_cancelButtonRect, 5.0f);

	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_buttonFont);
	mp_direct2d->SetBrushColor(m_cancelButtonColor);
	mp_direct2d->DrawUserText(L"Save", m_saveButtonRect);
	mp_direct2d->SetBrushColor(m_textColor);
	mp_direct2d->DrawUserText(L"Cancel", m_cancelButtonRect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}