#pragma once

#include "framework.h"
#include "targetver.h"
#include "shellapi.h"

typedef enum MessageID
{
	Destroy = WM_DESTROY,
	Paint = WM_PAINT,
	SystemCommand = WM_SYSCOMMAND,
	DropFile = WM_DROPFILES,

	MouseWheel = WM_MOUSEWHEEL,
	MouseMove = WM_MOUSEMOVE,

	MouseLeftDown = WM_LBUTTONDOWN,
	MouseLeftUp = WM_LBUTTONUP,
	MouseLeftDoubleClick = WM_LBUTTONDBLCLK,

	MouseRightDown = WM_RBUTTONDOWN,
	MouseRightUp = WM_RBUTTONUP,
	MouseRightDoubleClick = WM_RBUTTONDBLCLK,

	MouseMiddleDown = WM_MBUTTONDOWN,
	MouseMiddleUp = WM_MBUTTONUP,
	MouseMiddleDoubleClick = WM_MBUTTONDBLCLK,

	KeyDown = WM_KEYDOWN,
	KeyUp = WM_KEYUP,
	SystemKeyDown = WM_SYSKEYDOWN,
	SystemKeyUp = WM_SYSKEYUP,
} MID;

struct Event {};

struct MouseEvent : Event
{
	long clientX = static_cast<long>(-1);
	long clientY = static_cast<long>(-1);
	short delta = static_cast<short>(-1);
};

struct SystemEvent : Event
{
	size_t m_menuID = static_cast<size_t>(-1);
	size_t m_dumpData = static_cast<size_t>(-1);
};

struct DropFileEvent : Event
{
	HDROP m_dropInfo = nullptr;
};

struct KeyboardEvent : Event
{
	unsigned short virtualKey = static_cast<unsigned short>(-1);
	unsigned short keyFlags = static_cast<unsigned short>(-1);
	unsigned short scanCode = static_cast<unsigned short>(-1);
	bool isExtendedKey = false;
	bool wasKeyDown = false;
	bool isKeyReleased = false;
	unsigned short repeatCount = static_cast<unsigned short>(-1);
};