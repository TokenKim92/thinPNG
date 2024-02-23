#include <d2d1.h>
#include <list>
#include <string>
#include <cctype>    // std::tolower
#include <algorithm> // std::equal
#include "Utility.h"

bool PointInRectF(const D2D1_RECT_F &ap_rect, const POINT &ap_pos)
{
	return ap_rect.left <= ap_pos.x &&
		ap_rect.top <= ap_pos.y &&
		ap_rect.right >= ap_pos.x &&
		ap_rect.bottom >= ap_pos.y;
}

bool caseInsensitiveEqual(const std::string &a_text1, const std::string &a_text2)
{
	return std::equal(
		a_text1.begin(), a_text1.end(),
		a_text2.begin(), a_text2.end(),
		[](char a_text1, char a_text2) {
			return std::tolower(static_cast<unsigned char>(a_text1)) == std::tolower(static_cast<unsigned char>(a_text2));
		}
	);
}

bool IsImageFieExtension(const std::string &fileExtension)
{
	std::list<std::string> imageFileExtensios{ ".png", ".jpg", ".jpeg" };

	for (auto const &extension : imageFileExtensios) {
		if (caseInsensitiveEqual(extension, fileExtension)) {
			return true;
		}
	}

	return false;
}