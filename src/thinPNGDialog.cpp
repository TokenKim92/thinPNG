#include "thinPNGDialog.h"
#include "ColorPalette.h"
#include "utility.h"

#ifdef _DEBUG
#pragma comment (lib, "AppTemplateDebug.lib")
#else
#pragma comment (lib, "AppTemplate.lib")     
#endif

thinPNG::thinPNG() :
	WindowDialog(L"THINPNG", L"thinPNG")
{
	memset(&m_viewRect, 0, sizeof(RECT));
	m_resizeWidth = 0;
	m_resizeHeight = 0;

	mp_dashStrokeStyle = nullptr;
	mp_gridFont = nullptr;
	mp_optionFont = nullptr;

	memset(&m_gridRect, 0, sizeof(DRect));
	memset(&m_optionButtonRect, 0, sizeof(DRect));
	m_textColor = RGB_TO_COLORF(NEUTRAL_100);
	m_optionButtonColor = RGB_TO_COLORF(ZINC_700);
	m_optionButtonBorderColor = RGB_TO_COLORF(NEUTRAL_50);

	m_hoverOnOptionButton = false;
	m_clickedOnOptionButton = false;
}

thinPNG::~thinPNG()
{
}

void thinPNG::OnInitDialog()
{
	DisableMaximize();
	DisableMinimize();
	DisableSize();
	
	// set transparency of window
	::SetLayeredWindowAttributes(mh_window, 0, 230, LWA_ALPHA);

	::GetClientRect(mh_window, &m_viewRect);
	const float gridRectOffset = 10.0f;
	m_gridRect.left = gridRectOffset;
	m_gridRect.top = gridRectOffset;
	m_gridRect.right = m_viewRect.right - gridRectOffset;
	m_gridRect.bottom = m_viewRect.bottom - gridRectOffset * 5;

	const float optionRectOffset = 80.0f;
	m_optionButtonRect.left = optionRectOffset;
	m_optionButtonRect.top = m_gridRect.bottom + gridRectOffset;
	m_optionButtonRect.right = m_viewRect.right - optionRectOffset;
	m_optionButtonRect.bottom = m_viewRect.bottom - gridRectOffset;

	mp_direct2d->SetBrushColor(RGB_TO_COLORF(NEUTRAL_100));
	mp_dashStrokeStyle = mp_direct2d->CreateUserStrokeStyle(D2D1_DASH_STYLE_DASH);

	mp_gridFont = mp_direct2d->CreateTextFormat(DEFAULT_FONT_NAME, 15.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL);
	mp_gridFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mp_gridFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	mp_optionFont = mp_direct2d->CreateTextFormat(DEFAULT_FONT_NAME, 12.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL);
	mp_optionFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mp_optionFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// add message handlers
	AddMessageHandler(WM_MOUSEMOVE, static_cast<MessageHandler>(&thinPNG::MouseMoveHandler));
	AddMessageHandler(WM_LBUTTONDOWN, static_cast<MessageHandler>(&thinPNG::MouseLeftButtonDownHandler));
	AddMessageHandler(WM_LBUTTONUP, static_cast<MessageHandler>(&thinPNG::MouseLeftButtonUpHandler));
}

void thinPNG::OnDestroy()
{
	InterfaceRelease(&mp_dashStrokeStyle);
	InterfaceRelease(&mp_gridFont);
	InterfaceRelease(&mp_optionFont);
}

void thinPNG::OnPaint()
{
	mp_direct2d->Clear();
	DrawField();
	DrawOptionButton();
}

void thinPNG::OnSetThemeMode()
{
	if (THEME_MODE::LIGHT_MODE == GetThemeMode()) {
		m_textColor = RGB_TO_COLORF(NEUTRAL_600);
		m_optionButtonColor = RGB_TO_COLORF(ZINC_200);
		m_optionButtonBorderColor = RGB_TO_COLORF(NEUTRAL_800);

		mp_direct2d->SetBackgroundColor(RGB_TO_COLORF(ZINC_50));
	}
	else {
		m_textColor = RGB_TO_COLORF(NEUTRAL_100);
		m_optionButtonColor = RGB_TO_COLORF(ZINC_700);
		m_optionButtonBorderColor = RGB_TO_COLORF(NEUTRAL_50);

		mp_direct2d->SetBackgroundColor(RGB_TO_COLORF(NEUTRAL_800));
	}

	::InvalidateRect(mh_window, &m_viewRect, false);
}

// to handle the WM_MOUSEMOVE message that occurs when a window is destroyed
int thinPNG::MouseMoveHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	if (PointInRectF(m_optionButtonRect, pos)) {
		if (!m_hoverOnOptionButton) {
			m_hoverOnOptionButton = true;
			::InvalidateRect(mh_window, &m_viewRect, false);
		}
	}
	else {
		if (m_hoverOnOptionButton) {
			m_hoverOnOptionButton = false;
			::InvalidateRect(mh_window, &m_viewRect, false);
		}
	}

	return S_OK;
}

// to handle the WM_LBUTTONDOWN  message that occurs when a window is destroyed
int thinPNG::MouseLeftButtonDownHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	const POINT pos = { LOWORD(a_longParam), HIWORD(a_longParam) };

	if (PointInRectF(m_optionButtonRect, pos)) {
		m_clickedOnOptionButton = true;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}

	return S_OK;
}

// to handle the WM_LBUTTONUP  message that occurs when a window is destroyed
int thinPNG::MouseLeftButtonUpHandler(WPARAM a_wordParam, LPARAM a_longParam)
{
	if (m_clickedOnOptionButton) {
		m_clickedOnOptionButton = false;
		::InvalidateRect(mh_window, &m_viewRect, false);
	}

	return S_OK;
}

void thinPNG::DrawField()
{
	mp_direct2d->SetBrushColor(m_textColor);

	auto prevStrokeStyle = mp_direct2d->SetStrokeStyle(mp_dashStrokeStyle);
	mp_direct2d->DrawRoundedRectangle(m_gridRect, 10.0f);
	mp_direct2d->SetStrokeStyle(prevStrokeStyle);

	
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_gridFont);
	mp_direct2d->DrawUserText(L"Drop your PNG or JPEF files here!", m_gridRect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}

void thinPNG::DrawOptionButton()
{
	float transparency = m_hoverOnOptionButton ? 1.0f : 0.7f;
	if (m_clickedOnOptionButton) {
		transparency = 0.9f;
	}

	m_optionButtonColor.a = transparency;
	m_optionButtonBorderColor.a = transparency;

	mp_direct2d->SetBrushColor(m_optionButtonColor);
	mp_direct2d->FillRoundedRectangle(m_optionButtonRect, 5.0f);
	mp_direct2d->SetBrushColor(m_optionButtonBorderColor);
	mp_direct2d->DrawRoundedRectangle(m_optionButtonRect, 5.0f);


	mp_direct2d->SetBrushColor(m_textColor);
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_optionFont);
	mp_direct2d->DrawUserText(L"Resize option", m_optionButtonRect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}