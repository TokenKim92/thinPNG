#ifndef _WINDOW_DIALOG_H_
#define _WINDOW_DIALOG_H_

#include "framework.h"
#include "targetver.h"
#include <map>
#include <Direct2DEx.h>

// type modifier for message handlers
#ifndef msg_handler
    #define msg_handler
#endif

class WindowDialog; // for typedef of 'MessageHandler'

typedef int (WindowDialog:: *MessageHandler)(WPARAM, LPARAM);

class WindowDialog
{
public:
    enum THEME_MODE
    {
        LIGHT_MODE = 0,
        DARK_MODE
    };

protected:
    wchar_t *mp_windowClass;                // name of window class
    wchar_t *mp_title;                      // title of the application
    int m_showType;                         // the initial output state of the application
    
    HWND mh_window;                         // to save the main window handle
    std::map<unsigned int, MessageHandler> m_messageMap;

    Direct2DEx *mp_direct2d;
    THEME_MODE m_themeMode;
    unsigned long m_style;
    unsigned long m_extendStyle;

public:
    static LRESULT CALLBACK WindowProcedure(HWND ah_window, UINT a_messageID, WPARAM a_wordParam, LPARAM a_longParam);

    WindowDialog(const wchar_t *const ap_windowClass = nullptr, const wchar_t *const ap_title = nullptr);
    virtual ~WindowDialog();

    // window class registration
    void RegistWindowClass();
    // allocate and initialize a main window instance
    bool InitInstance(int a_width = CW_USEDEFAULT, int a_height = 0, int a_x = CW_USEDEFAULT, int a_y = 0);
    // Functions that handle messages issued to the application
    int Run();

    int Create(int a_width = CW_USEDEFAULT, int a_height = 0, int a_x = CW_USEDEFAULT, int a_y = 0);
    int DoModal(HWND ah_parentWindow, int a_width = CW_USEDEFAULT, int a_height = 0, int a_x = CW_USEDEFAULT, int a_y = 0);
    void SetStyle(const unsigned long a_tyle);
    void SetExtendStyle(const unsigned long a_extendStyle);
    int SetThemeMode(const THEME_MODE a_mode);
    void InheritDirect2D(Direct2DEx *const ap_direct2d);
    const THEME_MODE GetThemeMode();

    void DisableMove();
    void DisableSize();
    void DisableMinimize();
    void DisableMaximize();
    
    // find the message handler for a given message ID.
    MessageHandler GetMessageHandler(unsigned int a_messageID);
    void AddMessageHandler(unsigned int a_messageID, MessageHandler a_handler);
    void RemoveMessageHandler(unsigned int a_messageID);

    // to handle the WM_DESTROY message that occurs when a window is destroyed
    msg_handler int DestroyHandler(WPARAM a_wordParam, LPARAM a_longParam);
    // to handle the WM_PAINT message that occurs when a window is created
    msg_handler int PaintHandler(WPARAM a_wordParam, LPARAM a_longParam);
    // to handle the WM_SYSCOMMAND message that occurs when a window is created
    msg_handler int SysCommandHandler(WPARAM a_menuID, LPARAM a_longParam);

    virtual void OnInitDialog();
    virtual void OnDestroy();
    virtual void OnPaint();
    virtual void OnSetThemeMode();
};

#endif //_WINDOW_DIALOG_H_ 