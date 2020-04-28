#ifndef DRAW_UI_H
#define DRAW_UI_H

#include <string>

#include <wchar.h>
#include <locale.h>
#include <ctype.h>
#include <cmath>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "params.h"

class UI{
public:
	UI();
	~UI();
	void ui_draw_default(cv::Mat &image);
	void ui_draw_target(cv::Mat &image, cv::Rect &user_rect, cv::Point2f &center_point, int board_cows, int board_rows);
	void ui_draw_source(cv::Mat &image, const std::vector<cv::Point2f> &data, cv::Point2f &center_point, int board_cows, int board_rows);
	void ui_draw_text(cv::Mat &image, ParamsType &params);
	void ui_draw_params(cv::Mat &image, ParamsType &params);
	void ui_put_text(cv::Mat &image, const wchar_t* text, cv::Point &loc, cv::Scalar color);
	void ui_put_wchar(cv::Mat &image, wchar_t wc, cv::Point &pos, cv::Scalar color);
	void ui_get_font(int* type, cv::Scalar* size, bool* underline, float* diaphaneity);
	void ui_set_font(int* type, cv::Scalar* size, bool* underline, float* diaphaneity);
	void restore_fonts();

public:
	FT_Library m_library;
	FT_Face m_face;

	int m_fontType;
	cv::Scalar m_fontSize;
	bool m_fontUnderline;
	float m_fontDiaphaneity;

	wchar_t *distance_char;
	wchar_t *raw_char;
	wchar_t *pitch_char;
	wchar_t *yoll_char;
};

#endif