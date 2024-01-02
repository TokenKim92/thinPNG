#ifndef _DIRECT_2D_CORE_
#define _DIRECT_2D_CORE_

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#pragma comment(lib, "Shcore.lib")

template<class Interface>
void InterfaceRelease(Interface **ap_interfaceObject)
{
	if (*ap_interfaceObject) {
		(*ap_interfaceObject)->Release();
		(*ap_interfaceObject) = nullptr;
	}
}

class ApplicationCore
{
protected:
	ID2D1Factory *mp_factory;			// an object that creates various objects composing Direct2D.
	IDWriteFactory *mp_wirteFactory;	// an object that creates resources related to string output.
	IWICImagingFactory *mp_wicFactory;	// an object that creates various window imaging components

	HINSTANCE mh_instance;				// handle of application instance to access resources

public:
	ApplicationCore(HINSTANCE ah_instance);
	virtual ~ApplicationCore();

	const int Create();

	ID2D1Factory *const GetFactory();
	IDWriteFactory *const GetWriteFactory();
	IWICImagingFactory *const GetWICFactory();

	const HINSTANCE GetHandleInstance();
};

#endif //_DIRECT_2D_CORE_