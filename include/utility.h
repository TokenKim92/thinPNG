#ifndef _UTILITY_H_ 
#define _UTILITY_H_

bool PointInRectF(const D2D1_RECT_F &ap_rect, const POINT &ap_pos);
bool caseInsensitiveEqual(const std::string &a_text1, const std::string &a_text2);
bool IsImageFieExtension(const std::string &fileExtension);

#endif //_UTILITY_H_