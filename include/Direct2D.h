#ifndef _DIRECT_2D_H_
#define _DIRECT_2D_H_

#include "ApplicationCore.h"

#define DPoint	D2D1_POINT_2F
#define DRect	D2D1_RECT_F
#define DColor	D2D1_COLOR_F
#define DSize	D2D1_SIZE_F

class Direct2D
{
protected:
	const HWND mh_window;
	RECT *mp_viewRect;

	ID2D1RenderTarget *mp_renderTarget;				// instance to draw in window client area
	ID2D1Brush *mp_brush;							// used as output brush for lines and strings
	ID2D1StrokeStyle *mp_strokeStyle;

	DColor m_brushColor;
	DColor m_backgroundColor;
	float m_strokeWidth;

public:
	Direct2D(const HWND ah_window, const RECT *const ap_viewRect = nullptr);
	virtual ~Direct2D();

	virtual int Create();

	void BeginDraw();
	void EndDraw();
	void Clear();

	ID2D1LinearGradientBrush *const CreateLinearGradientBrush(
		const D2D1_GRADIENT_STOP *const a_gradientStopList,
		const unsigned int gradientStopsCount,
		const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES *const a_gradientPositionData
	);
	ID2D1StrokeStyle *const CreateUserStrokeStyle(
		const D2D1_DASH_STYLE a_dashStyle, const D2D1_CAP_STYLE a_sideCap = D2D1_CAP_STYLE_ROUND,
		const D2D1_CAP_STYLE a_dashCap = D2D1_CAP_STYLE_ROUND, const D2D1_LINE_JOIN a_lineJoin = D2D1_LINE_JOIN_ROUND,
		const float a_miterLimit = 10.0f, const float a_dashOffset = 0.0f
	);

	void SetBrushColor(const DColor &a_color);
	void SetBackgroundColor(const DColor &a_backgroundColor);
	// returns the previous brush. must be released from the user
	ID2D1Brush *SetBrush(ID2D1Brush *const ap_brush);
	// returns the previous stroke style. must be released from the user
	ID2D1StrokeStyle *const SetStrokeStyle(ID2D1StrokeStyle *const ap_strokeStyle);
	void SetStrokeWidth(const float a_strokeWidth);

protected:
	virtual HRESULT CreateDeviceResources();
	virtual void DestroyDeviceResources();

// drawing methode
public:
	void DrawLine(const DPoint &a_startPoint, const DPoint &a_endPoint);
	void DrawRectangle(const DPoint &a_startPoint, const DPoint &a_endPoint);
	void DrawRectangle(const DRect &a_rect);
	void DrawRoundedRectangle(const DPoint &a_startPoint, const DPoint &a_endPoint, const float radius);
	void DrawRoundedRectangle(const DRect &a_rect, const float radius);
	void DrawEllipse(const DPoint &a_startPoint, const DPoint &a_endPoint);
	void DrawEllipse(const DRect &a_rect);
	void DrawGeometry(ID2D1Geometry *const ap_geometry);

	void FillRectangle(const DRect &a_rect);
	void FillRectangle(const DPoint &a_startPoint, const DPoint &a_endPoint);
	void FillRoundedRectangle(const DRect &a_rect, const float radius);
	void FillRoundedRectangle(const DPoint &a_startPoint, const DPoint &a_endPoint, const float radius);
	void FillEllipse(const DRect &a_rect);
	void FillGeometry(ID2D1Geometry *const p_geometry);
};

#endif //_DIRECT_2D_H_