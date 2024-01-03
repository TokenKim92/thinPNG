// AppTemplate.cpp : Defines the entry point for the application.

#include "framework.h"
#include "ApplicationCore.h"
#include "thinPNGDialog.h"

#ifdef _DEBUG
    #pragma comment (lib, "AppTemplateDebug.lib")
#else
    #pragma comment (lib, "AppTemplate.lib")     
#endif

int APIENTRY wWinMain(_In_ HINSTANCE ah_instance, _In_opt_ HINSTANCE ah_notUseInstance, _In_ wchar_t *ap_cmdLine, _In_ int a_showType)
{
    ApplicationCore appCore(ah_instance);
    if (S_OK == appCore.Create()) {
        const int centerPosX = ::GetSystemMetrics(SM_CXSCREEN) / 2;
        const int centerPosY = ::GetSystemMetrics(SM_CYSCREEN) / 2;
        const int width = 330;
        const int height = 240;

        thinPNG dialog;
        dialog.SetExtendStyle(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_ACCEPTFILES);
        return dialog.Create(width, height, centerPosX - width / 2, centerPosY - height / 2);
    }

    return 0;
}
