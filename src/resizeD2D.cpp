#include "framework.h"
#include "resizeD2D.h"
#include "utility.h"
#include <filesystem>
#include <shcore.h>

extern ApplicationCore *gp_appCore;

ResizeD2D::ResizeD2D(HWND ah_wnd, const RECT *const ap_viewRect)
	:Direct2DEx(ah_wnd, ap_viewRect)
{
	
}

ResizeD2D::~ResizeD2D()
{
	
}

void ResizeD2D::ResizeImage(const std::wstring &a_filePath, unsigned int a_size, const OptionDialog::CONTROL_TYPE &a_selectedRadioType)
{
    auto p_bitmap = ScaleImageToBitmap(a_filePath, a_size, a_selectedRadioType);
    if (nullptr != p_bitmap) {
        SaveBitmapToFile(a_filePath, p_bitmap);
        p_bitmap->Release();
    }
}

IWICFormatConverter *ResizeD2D::ScaleImageToBitmap(const std::wstring &a_filePath, unsigned int a_size, const OptionDialog::CONTROL_TYPE &a_selectedRadioType)
{
    IWICImagingFactory *p_wicFactory = gp_appCore->GetWICFactory();

    IWICBitmapDecoder *p_decorder;
    if (S_OK != p_wicFactory->CreateDecoderFromFilename(a_filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &p_decorder)) {
        return nullptr;
    }

    IWICBitmapFrameDecode *p_frameDecode;
    if (S_OK != p_decorder->GetFrame(0, &p_frameDecode)) {
        p_decorder->Release();
        return nullptr;
    }

    IWICBitmapScaler *p_scaler;
    if (S_OK != p_wicFactory->CreateBitmapScaler(&p_scaler)) {
        p_frameDecode->Release();
        p_decorder->Release();
        return nullptr;
    }
    
    unsigned int originalWidth;
    unsigned int originalHeight;
    p_frameDecode->GetSize(&originalWidth, &originalHeight);
    
    unsigned int resizeWidth = a_size;
    unsigned int resizeHeight = a_size;
    if (OptionDialog::CONTROL_TYPE::WIDTH_RADIO == a_selectedRadioType) {
        resizeHeight = static_cast<unsigned int>(static_cast<float>(a_size) * originalHeight / originalWidth);
    }
    else {
        resizeWidth = static_cast<unsigned int>(static_cast<float>(a_size) * originalWidth / originalHeight);
    }

    p_scaler->Initialize(p_frameDecode, resizeWidth, resizeHeight, WICBitmapInterpolationModeFant);
    
    IWICFormatConverter *p_converter;
    p_wicFactory->CreateFormatConverter(&p_converter);
    HRESULT result = p_converter->Initialize(
        static_cast<IWICBitmapSource *>(p_scaler), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom
    );

    p_frameDecode->Release();
    p_decorder->Release();
    p_scaler->Release();

    return S_OK == result ? p_converter : nullptr;
}

void ResizeD2D::SaveBitmapToFile(const std::wstring &a_filePath, IWICFormatConverter *const ap_bitmap)
{
    IWICImagingFactory *p_wicFactory = gp_appCore->GetWICFactory();

    IWICStream *p_stream;
    if (S_OK != p_wicFactory->CreateStream(&p_stream)) {
        return;
    }

    std::filesystem::path filePath(a_filePath);
    // check file extension
    GUID guidContainerFormat = GUID_ContainerFormatJpeg;
    if (caseInsensitiveEqual(filePath.extension().string(), ".png")) {
        guidContainerFormat = GUID_ContainerFormatPng;
    }
    // adjust file name
    auto fileName = filePath.stem().wstring();
    std::wstring adjustedFileName = filePath.parent_path().wstring() + L"\\" + L"resize-" + fileName + filePath.extension().wstring();
    p_stream->InitializeFromFilename(adjustedFileName.c_str(), GENERIC_WRITE);

    IWICBitmapEncoder *p_encoder;
    if (S_OK != p_wicFactory->CreateEncoder(guidContainerFormat, nullptr, &p_encoder)) {
        p_stream->Release();
        return;
    }

    if (S_OK != p_encoder->Initialize(p_stream, WICBitmapEncoderNoCache)) {
        p_encoder->Release();
        p_stream->Release();
        return;
    }

    IWICBitmapFrameEncode *p_frameEncode;
    if (S_OK != p_encoder->CreateNewFrame(&p_frameEncode, nullptr)) {
        p_encoder->Release();
        p_stream->Release();
        return;
    }

    // save bitmap to file
    p_frameEncode->Initialize(nullptr);
    WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
    p_frameEncode->SetPixelFormat(&format);
    p_frameEncode->WriteSource(ap_bitmap, nullptr);
    p_frameEncode->Commit();
    p_encoder->Commit();

    // destroy all interface which are used
    p_encoder->Release();
    p_frameEncode->Release();
    p_stream->Release();
}