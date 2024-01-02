#include <d2d1.h>
#include "utility.h"

bool PointInRectF(const D2D1_RECT_F &ap_rect, const POINT &ap_pos)
{
	return ap_rect.left <= ap_pos.x &&
		ap_rect.top <= ap_pos.y &&
		ap_rect.right >= ap_pos.x &&
		ap_rect.bottom >= ap_pos.y;
}