// thinPNG.cpp : Defines the class behaviors for the application.

#include "pch.h"
#include "framework.h"
#include "thinPNG.h"
#include "thinPNGDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CThinPNGApp
BEGIN_MESSAGE_MAP(CThinPNGApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CThinPNGApp construction
CThinPNGApp::CThinPNGApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

// The one and only CthinPNGApp object
CThinPNGApp theApp;

// CThinPNGApp initialization
BOOL CThinPNGApp::InitInstance()
{
    CWinApp::InitInstance();

    CThinPNGDlg dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();

    return FALSE;
}

