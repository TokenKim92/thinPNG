#pragma once

#include "framework.h"
#include "targetver.h"
#include "Direct2DEx.h"
#include "Event.h"
#include <map>
#include <string>
#include <memory>

class WindowDialog;

typedef void (*EventListener)(Event *const ap_event, WindowDialog *const ap_dialog);

class WindowDialog
{
public:
	static int MENU_DARK_MODE;
	static int MENU_LIGHT_MODE;

	typedef enum CLOSED_TYPE
	{
		OK,
		CANCEL
	} CT;

private:
	typedef struct THREAD_DATA
	{
		WindowDialog *p_dialog;
		HANDLE h_thread;
		POINT startPosition;
	}TD;

protected:
	std::wstring m_windowClass;             // name of window class
	std::wstring m_title;                   // title of the application
	int m_showType;                         // the initial output state of the application

	HWND mh_window;                         // to save the main window handle
	std::map<MID, EventListener> m_eventMap;

	Direct2DEx *mp_direct2d;
	CM m_colorMode;
	RECT m_viewRect;
	int m_width;
	int m_height;
	unsigned long m_style;
	unsigned long m_extendStyle;

	CT m_clickedButtonType;

private:
	std::unique_ptr<TD> m_threadData;

public:
	WindowDialog(const std::wstring &a_windowClass, const std::wstring &a_title = L"");
	virtual ~WindowDialog();

	// this method is for a modeless progressing dialog
	// but this is until not stable therefore the method DoModal() is recommended
	bool AsyncCreate(int a_x = CW_USEDEFAULT, int a_y = 0);
	// to destroy window after using Create(...)
	void DestroyWindow();

	WindowDialog::CT Create(HWND ah_parentWindow = nullptr, int a_x = CW_USEDEFAULT, int a_y = 0);
	void Invalidate(const bool backgroundErase = false);

	void SetSize(const int a_width, const int a_height);
	void SetStyle(const unsigned long a_style);
	void SetExtendStyle(const unsigned long a_extendStyle);
	bool SetColorMode(const CM a_mode);
	void InheritDirect2D(Direct2DEx *const ap_direct2d);

	const HWND GetWidnowHandle();
	const SIZE GetSize();
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
	// functions that handle messages issued to the application
	// returns true if closed by the PostQuitMessage.
	bool Run();

	// find the event listener for a given message ID.
	EventListener GetEventListener(const MID &a_messageID);
	Event *const EventAllocate(const MID &a_messageID, const WPARAM a_wordParam, const LPARAM a_longParam);
	void EventFree(Event *const ap_event);

	void AddEventListener(const MID &a_messageID, EventListener ap_eventListener);
	void RemoveEventListener(const MID &a_messageID);

	virtual void OnAddEventListener();
	virtual void OnInitDialog() {};
	virtual void OnDestroy() {};
	virtual void OnQuit() {};
	virtual void OnPaint() {};
	virtual void OnSetColorMode() {};

	virtual bool PreTranslateMessage(MSG &a_msg);

	void SetClosedType(const CT &a_type);

private:
	static DWORD WINAPI RunOnOtherThread(void *ap_data);
};