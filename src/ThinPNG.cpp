// AppTemplate.cpp : Defines the entry point for the application.

#include "framework.h"
#include "ApplicationCore.h"
#include "ThinPNGDialog.h"

#ifdef _DEBUG
#pragma comment (lib, "Win32CoreDebug.lib")
#else
#pragma comment (lib, "Win32Core.lib")     
#endif

int APIENTRY wWinMain(_In_ HINSTANCE ah_instance, _In_opt_ HINSTANCE ah_notUseInstance, _In_ wchar_t *ap_cmdLine, _In_ int a_showType)
{
    ApplicationCore appCore(ah_instance);
    if (appCore.Create()) {
        const int centerPosX = ::GetSystemMetrics(SM_CXSCREEN) / 2;
        const int centerPosY = ::GetSystemMetrics(SM_CYSCREEN) / 2;

        ThinPNGDialog dialog;
        const auto dialogSize = dialog.GetSize();
        return dialog.Create(nullptr, centerPosX - dialogSize.cx / 2, centerPosY - dialogSize.cy / 2);
    }

    return 0;
}
