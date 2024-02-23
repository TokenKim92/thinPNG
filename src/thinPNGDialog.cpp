#include "ThinPNGDialog.h"
#include "ColorPalette.h"
#include "Utility.h"
#include "ResizeD2D.h"
#include <filesystem>

#ifdef _DEBUG
#pragma comment (lib, "Win32CoreDebug.lib")
#else
#pragma comment (lib, "Win32Core.lib")     
#endif

ThinPNGDialog::ThinPNGDialog() :
	WindowDialog(L"THINPNGDIALOG", L"ThinPNGDialog")
{
	SetSize(330, 240);
	SetExtendStyle(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_ACCEPTFILES);

	memset(&m_viewRect, 0, sizeof(RECT));
	m_size = 800;
	m_selectedRatioType = OptionDialog::CONTROL_TYPE::WIDTH_RADIO;

	mp_dashStrokeStyle = nullptr;
	mp_gridFont = nullptr;
	mp_optionFont = nullptr;

	memset(&m_gridRect, 0, sizeof(DRect));
	memset(&m_optionButtonRect, 0, sizeof(DRect));
	m_textColor = ChangeRgbToColorF(NEUTRAL_100);
	m_saveButtonColor = ChangeRgbToColorF(ZINC_700);
	m_buttonBorderColor = ChangeRgbToColorF(NEUTRAL_50);

	m_hoverOnOptionButton = false;
}

void ThinPNGDialog::OnAddEventListener()
{
	WindowDialog::OnAddEventListener();

	AddEventListener(
		MID::MouseMove,
		[](Event *const ap_event, WindowDialog *const ap_dialog) {
			auto mouseEvent = static_cast<MouseEvent *>(ap_event);
			auto dialog = static_cast<ThinPNGDialog *>(ap_dialog);

			if (PointInRectF(dialog->m_optionButtonRect, { mouseEvent->clientX, mouseEvent->clientY })) {
				if (!dialog->m_hoverOnOptionButton) {
					dialog->m_hoverOnOptionButton = true;
					::InvalidateRect(dialog->mh_window, &dialog->m_viewRect, false);
				}

				return;
			}

			if (dialog->m_hoverOnOptionButton) {
				dialog->m_hoverOnOptionButton = false;
				::InvalidateRect(dialog->mh_window, &dialog->m_viewRect, false);
			}
		}
	);

	AddEventListener(
		MID::MouseLeftDown,
		[](Event *const ap_event, WindowDialog *const ap_dialog) {
			auto mouseEvent = static_cast<MouseEvent *>(ap_event);
			auto dialog = static_cast<ThinPNGDialog *>(ap_dialog);

			if (PointInRectF(dialog->m_optionButtonRect, { mouseEvent->clientX, mouseEvent->clientY })) {
				RECT rect;
				::GetWindowRect(dialog->mh_window, &rect);

				const int centerPosX = rect.left + (rect.right - rect.left) / 2;
				const int centerPosY = rect.top + (rect.bottom - rect.top) / 2;

				OptionDialog instanceDialog(dialog->m_size, dialog->m_selectedRatioType);
				auto colorMode = dialog->GetColorMode();
				if (CM::DARK != colorMode) {
					instanceDialog.SetColorMode(colorMode);
				}
				const auto dialogSize = instanceDialog.GetSize();

				if (CT::OK == instanceDialog.Create(dialog->mh_window, centerPosX - dialogSize.cx / 2, centerPosY - dialogSize.cy / 2)) {
					dialog->m_size = instanceDialog.GetSizeValue();
					dialog->m_selectedRatioType = instanceDialog.GetRatioType();
				}
			}
		}
	);

	AddEventListener(
		MID::DropFile,
		[](Event *const ap_event, WindowDialog *const ap_dialog) {
			auto dropFileInfo = static_cast<DropFileEvent *>(ap_event)->m_dropInfo;
			auto dialog = static_cast<ThinPNGDialog *>(ap_dialog);

			const unsigned int count = DragQueryFile(dropFileInfo, 0xFFFFFFFF, nullptr, 0);
			wchar_t filePath[MAX_PATH] = { 0, };
			bool existNoneImageFile = false;

			for (unsigned int i = 0; i < count; i++) {
				DragQueryFile(dropFileInfo, i, filePath, MAX_PATH);

				std::filesystem::path path(filePath);
				if (std::filesystem::is_directory(path)) {
					// TODO:: if a directory is dropped
					//for (auto const &directoryEntry : std::filesystem::directory_iterator(path)) {
					//}
					existNoneImageFile = true;
				}
				else {
					if (IsImageFieExtension(path.extension().string())) {
						static_cast<ResizeD2D *>(dialog->mp_direct2d)->ResizeImage(
							path.wstring(), dialog->m_size, dialog->m_selectedRatioType
						);
					}
					else {
						existNoneImageFile = true;
					}
				}
			}

			if (existNoneImageFile) {
				MessageBox(
					dialog->mh_window,
					L"Files which are not in PNG or JPEG format are ignored.",
					L"Invalid file format warning",
					MB_OK | MB_ICONWARNING
				);
			}
		}
	);
}

void ThinPNGDialog::OnInitDialog()
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

	mp_direct2d->SetBrushColor(ChangeRgbToColorF(NEUTRAL_100));
	mp_dashStrokeStyle = mp_direct2d->CreateUserStrokeStyle(D2D1_DASH_STYLE_DASH);

	mp_gridFont = mp_direct2d->CreateTextFormat(Direct2DEx::DEFAULT_FONT_NAME, 15.0f, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL);
	mp_gridFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mp_gridFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	mp_optionFont = mp_direct2d->CreateTextFormat(Direct2DEx::DEFAULT_FONT_NAME, 12.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL);
	mp_optionFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	mp_optionFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	InheritDirect2D(new ResizeD2D(mh_window, &m_viewRect));
	mp_direct2d->Create();
}

void ThinPNGDialog::OnDestroy()
{
	InterfaceRelease(&mp_dashStrokeStyle);
	InterfaceRelease(&mp_gridFont);
	InterfaceRelease(&mp_optionFont);
}

void ThinPNGDialog::OnPaint()
{
	mp_direct2d->Clear();
	DrawField();
	DrawOptionButton();
}

void ThinPNGDialog::OnSetColorMode()
{
	if (CM::LIGHT == GetColorMode()) {
		m_textColor = ChangeRgbToColorF(NEUTRAL_600);
		m_saveButtonColor = ChangeRgbToColorF(ZINC_200);
		m_buttonBorderColor = ChangeRgbToColorF(NEUTRAL_800);

		mp_direct2d->SetBackgroundColor(ChangeRgbToColorF(ZINC_50));
	}
	else {
		m_textColor = ChangeRgbToColorF(NEUTRAL_100);
		m_saveButtonColor = ChangeRgbToColorF(ZINC_700);
		m_buttonBorderColor = ChangeRgbToColorF(NEUTRAL_50);

		mp_direct2d->SetBackgroundColor(ChangeRgbToColorF(NEUTRAL_800));
	}

	::InvalidateRect(mh_window, &m_viewRect, false);
}

void ThinPNGDialog::DrawField()
{
	mp_direct2d->SetBrushColor(m_textColor);

	auto prevStrokeStyle = mp_direct2d->SetStrokeStyle(mp_dashStrokeStyle);
	mp_direct2d->DrawRoundedRectangle(m_gridRect, 10.0f);
	mp_direct2d->SetStrokeStyle(prevStrokeStyle);

	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_gridFont);
	mp_direct2d->DrawUserText(L"Drop your PNG or JPG files here!", m_gridRect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}

void ThinPNGDialog::DrawOptionButton()
{
	float transparency = m_hoverOnOptionButton ? 1.0f : 0.7f;

	m_saveButtonColor.a = transparency;
	m_buttonBorderColor.a = transparency;

	mp_direct2d->SetBrushColor(m_saveButtonColor);
	mp_direct2d->FillRoundedRectangle(m_optionButtonRect, 5.0f);
	mp_direct2d->SetBrushColor(m_buttonBorderColor);
	mp_direct2d->DrawRoundedRectangle(m_optionButtonRect, 5.0f);

	mp_direct2d->SetBrushColor(m_textColor);
	auto prevTextFormat = mp_direct2d->SetTextFormat(mp_optionFont);
	mp_direct2d->DrawUserText(L"Resize option", m_optionButtonRect);
	mp_direct2d->SetTextFormat(prevTextFormat);
}
