#ifndef _WINDOW_DIALOG_H_
#define _WINDOW_DIALOG_H_

#include "framework.h"
#include "targetver.h"
#include "Direct2DEx.h"
#include <map>

#define MENU_DARK_MODE      20000
#define MENU_LIGHT_MODE     20001

// type modifier for message handlers
#ifndef msg_handler
#define msg_handler
#endif

class WindowDialog; // for typedef of 'MessageHandler'

typedef int (WindowDialog:: *MessageHandler)(WPARAM, LPARAM);

class WindowDialog
{
public:
    typedef enum BUTTON_TYPE
    {
        OK,
        CANCEL
    } BT;

private:
    typedef struct THREAD_DATA
    {
        WindowDialog *p_dialog;
        HANDLE h_thread;
        POINT startPosition;
    }TD;

protected:
    wchar_t *mp_windowClass;                // name of window class
    wchar_t *mp_title;                      // title of the application
    int m_showType;                         // the initial output state of the application

    HWND mh_window;                         // to save the main window handle
    std::map<unsigned int, MessageHandler> m_messageMap;

    Direct2DEx *mp_direct2d;
    CM m_colorMode;
    RECT m_viewRect;
    int m_width;
    int m_height;
    unsigned long m_style;
    unsigned long m_extendStyle;

    BT m_clickedButtonType;

private:
    TD *mp_threadData;

public:
    WindowDialog(const wchar_t *const ap_windowClass = nullptr, const wchar_t *const ap_title = nullptr);
    virtual ~WindowDialog();

    bool Create(int a_x = CW_USEDEFAULT, int a_y = 0);
    void DestroyWindow();

    WindowDialog::BT DoModal(HWND ah_parentWindow = nullptr, int a_x = CW_USEDEFAULT, int a_y = 0);
    void Invalidate(bool backgroundErase = false);

    void SetSize(int a_width, int a_height);
    void SetStyle(const unsigned long a_tyle);
    void SetExtendStyle(const unsigned long a_extendStyle);
    int SetColorMode(const CM a_mode);
    void InheritDirect2D(Direct2DEx *const ap_direct2d);

    HWND GetWidnowHandle();
    SIZE GetSize();
    const CM GetColorMode();

    void DisableMove();
    void DisableSize();
    void DisableMinimize();
    void DisableMaximize();
    void DisableClose();
    void EnableClose();

protected:
    static LRESULT CALLBACK WindowProcedure(HWND ah_window, UINT a_messageID, WPARAM a_wordParam, LPARAM a_longParam);

    // window class registration
    void RegistWindowClass();
    // allocate and initialize a main window instance
    bool InitInstance(int a_width = CW_USEDEFAULT, int a_height = 0, int a_x = CW_USEDEFAULT, int a_y = 0);
    // Functions that handle messages issued to the application
    int Run();

    // find the message handler for a given message ID.
    MessageHandler GetMessageHandler(unsigned int a_messageID);
    void AddMessageHandler(unsigned int a_messageID, MessageHandler a_handler);
    void RemoveMessageHandler(unsigned int a_messageID);

    // to handle the WM_DESTROY message that occurs when a window is destroyed
    msg_handler int DestroyHandler(WPARAM a_wordParam, LPARAM a_longParam);
    // to handle the WM_PAINT message that occurs when a window is created
    msg_handler int PaintHandler(WPARAM a_wordParam, LPARAM a_longParam);
    // to handle the WM_SYSCOMMAND message
    virtual msg_handler int SysCommandHandler(WPARAM a_menuID, LPARAM a_longParam);


    virtual void OnInitDialog();
    virtual void OnDestroy();
    virtual void OnQuit();
    virtual void OnPaint();
    virtual void OnSetColorMode();

    virtual bool PreTranslateMessage(MSG &a_msg);

    void SetClickedButtonType(BT &a_type);

private:
    static DWORD WINAPI RunOnOtherThread(void *ap_data);
};

#endif //_WINDOW_DIALOG_H_ 