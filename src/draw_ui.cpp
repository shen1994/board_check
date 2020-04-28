/**
* @brief: to see more about fonts, read https://blog.csdn.net/fengbingchun/article/details/79806616
**/
#include "draw_ui.h"

UI::UI()
{
	if(FT_Init_FreeType(&m_library)) throw;
	if(FT_New_Face(m_library, "../fonts/chinese.simhei.ttf", 0, &m_face)) throw;

	UI::restore_fonts();
	setlocale(LC_ALL, "");

	// init some chinese characters
	const wchar_t distance_char_tmp[] = {L'\u8DDD', L'\u79BB'};
	distance_char = (wchar_t*)malloc(2 * sizeof(wchar_t));
	memcpy(distance_char, distance_char_tmp, 2 * sizeof(wchar_t));

	const wchar_t raw_char_tmp[] = {L'\u524D', L'\u540E'};
	raw_char = (wchar_t*)malloc(2 * sizeof(wchar_t));
	memcpy(raw_char, raw_char_tmp, 2 * sizeof(wchar_t));

	const wchar_t pitch_char_tmp[] = {L'\u5DE6', L'\u53F3'};
	pitch_char = (wchar_t*)malloc(2 * sizeof(wchar_t));
	memcpy(pitch_char, pitch_char_tmp, 2 * sizeof(wchar_t));

	const wchar_t yoll_char_tmp[] = {L'\u4E0A', L'\u4E0B'};
	yoll_char = (wchar_t*)malloc(2 * sizeof(wchar_t));
	memcpy(yoll_char, yoll_char_tmp, 2 * sizeof(wchar_t));
}

UI::~UI()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);

	if(distance_char != nullptr)
	{
		free(distance_char);
		distance_char = nullptr;
	}
	if(raw_char != nullptr)
	{
		free(raw_char);
		raw_char = nullptr;
	}
	if(pitch_char != nullptr)
	{
		free(pitch_char);
		pitch_char = nullptr;
	}
	if(yoll_char != nullptr)
	{
		free(yoll_char);
		yoll_char = nullptr;
	}
}

/**
* @brief: draw table to show params
**/
void UI::ui_draw_default(cv::Mat &image)
{
	int align_top = 20;
	int table_width = 200, table_height = 50;
	int table_cols = 4, table_rows = 2;
	cv::Point pt_start(0, 0), pt_end(0, 0);

	for(int i = 0; i < table_cols + 1; i++)
	{
		pt_start = cv::Point((int)(image.cols / 2.0 - table_width), align_top + table_height * i);
		pt_end = cv::Point((int)(image.cols / 2.0 + table_width), align_top + table_height * i);
		cv::line(image, pt_start, pt_end, cv::Scalar(255, 127, 0), 2, 8, 0);	
	}

	pt_start = cv::Point((int)(image.cols / 2.0), align_top);
	pt_end = cv::Point((int)(image.cols / 2.0), align_top + table_height * table_cols);
	cv::line(image, pt_start, pt_end, cv::Scalar(255, 127, 0), 2, 8, 0);

	for(int i = 1; i < (int)(table_rows / 2) + 1; i++)
	{
		pt_start = cv::Point((int)(image.cols / 2.0 - table_width * i), align_top);
		pt_end = cv::Point((int)(image.cols / 2.0 - table_width * i), align_top + table_height * table_cols);
		cv::line(image, pt_start, pt_end, cv::Scalar(255, 127, 0), 2, 8, 0);	

		pt_start = cv::Point((int)(image.cols / 2.0 + table_width * i), align_top);
		pt_end = cv::Point((int)(image.cols / 2.0 + table_width * i), align_top + table_height * table_cols);
		cv::line(image, pt_start, pt_end, cv::Scalar(255, 127, 0), 2, 8, 0);
	}
}

/**
* @brief: in target, draw rect, center point, line, etc
**/
void UI::ui_draw_target(cv::Mat &image, cv::Rect &user_rect, cv::Point2f &center_point, int board_cows, int board_rows)
{
    cv::rectangle(image, user_rect, cv::Scalar(0, 0, 255), 2, 8, 0);
    cv::circle(image, center_point, 0, cv::Scalar(0, 0, 255), 5, 8, 0);
    double ew = user_rect.width / board_cows;
    double eh = user_rect.height / board_rows;

    cv::Point2f pt_start(0, 0), pt_end(0, 0);

    // for(int i = 0; i < board_cows - 1; i++)
    // {
    //  	pt_start = cv::Point2f(user_rect.x + ew * (i + 1), user_rect.y);
    // 	pt_end = cv::Point2f(user_rect.x + ew * (i + 1), user_rect.y + eh * board_rows);
    // 	cv::line(image, pt_start, pt_end, cv::Scalar(0, 0, 255), 2, 8, 0);   	
    // }

    for(int i = 0; i < board_rows - 1; i++)
    {
     	pt_start = cv::Point2f(user_rect.x, user_rect.y + eh * (i + 1));
    	pt_end = cv::Point2f(user_rect.x + ew * board_cows, user_rect.y + eh * (i + 1));
    	cv::line(image, pt_start, pt_end, cv::Scalar(0, 0, 255), 2, 8, 0);    	
    }
}

/**
* @brief: in source, draw rect, center point, line, etc.
**/
void UI::ui_draw_source(cv::Mat &image, const std::vector<cv::Point2f> &data, cv::Point2f &center_point, int board_cows, int board_rows)
{
	std::vector<cv::Point2f> pts;
	pts.push_back(data[0]);
	pts.push_back(data[board_cows - 1]);
	pts.push_back(data[board_cows * (board_rows - 1)]);
	pts.push_back(data[board_cows * board_rows - 1]);

    cv::circle(image, center_point, 0, cv::Scalar(255, 0, 0), 5, 8, 0);

    // draw rect
    cv::line(image, pts[0], pts[1], cv::Scalar(255, 0, 0), 10, 8, 0); 
    cv::line(image, pts[2], pts[3], cv::Scalar(255, 0, 0), 10, 8, 0);
    cv::line(image, pts[0], pts[2], cv::Scalar(255, 0, 0), 10, 8, 0);
    cv::line(image, pts[1], pts[3], cv::Scalar(255, 0, 0), 10, 8, 0); 

    // draw table lines
    cv::Point2f top_e((pts[1].x - pts[0].x) / board_cows, (pts[1].y - pts[1].y) / board_rows);
    cv::Point2f bot_e((pts[3].x - pts[2].x) / board_cows, (pts[3].y - pts[2].y) / board_rows);
    cv::Point2f left_e((pts[2].x - pts[0].x) / board_cows, (pts[2].y - pts[0].y) / board_rows);
    cv::Point2f right_e((pts[3].x - pts[1].x) / board_cows, (pts[3].y - pts[1].y) / board_rows);

    cv::Point2f pt_start(0, 0), pt_end(0, 0);

    for(int i = 0; i < board_rows - 1; i++)
    {
     	pt_start = cv::Point2f(data[board_cows * (i + 1)].x, data[board_cows * (i + 1)].y);
    	pt_end = cv::Point2f(data[board_cows * (i + 1) + board_cows - 1].x, data[board_cows * (i + 1) + board_cows - 1].y);
    	cv::line(image, pt_start, pt_end, cv::Scalar(255, 0, 0), 10, 8, 0);    	
    }
}

/**
* @brief: draw texts on table
**/
void UI::ui_draw_text(cv::Mat &image, ParamsType &params)
{
	int align_top = 20;
	int table_width = 200, table_height = 50;
	int char_left = 80, char_top = 38;
	cv::Point pt_start(0, 0);

	cv::Scalar font_size{ 30, 0.5, 0.1, 0 };
	UI::ui_set_font(nullptr, &font_size, nullptr, 0);

	pt_start = cv::Point((int)(image.cols / 2.0 - table_width) + char_left, align_top + char_top);
	UI::ui_put_text(image, distance_char, pt_start, cv::Scalar(0, 0, 255));		

	pt_start = cv::Point((int)(image.cols / 2.0 - table_width) + char_left, align_top + table_height + char_top);
	UI::ui_put_text(image, raw_char, pt_start, cv::Scalar(0, 0, 255));	

	pt_start = cv::Point((int)(image.cols / 2.0 - table_width) + char_left, align_top + table_height * 2 + char_top);
	UI::ui_put_text(image, pitch_char, pt_start, cv::Scalar(0, 0, 255));

	pt_start = cv::Point((int)(image.cols / 2.0 - table_width) + char_left, align_top + table_height * 3 + char_top);
	UI::ui_put_text(image, yoll_char, pt_start, cv::Scalar(0, 0, 255));
}

/**
* @brief: draw params on table
**/
void UI::ui_draw_params(cv::Mat &image, ParamsType &params)
{
	int align_top = 20;
	int table_width = 200, table_height = 50;
	int char_left = 30, char_top = 38;
	cv::Point start_pt(0, 0);
	std::string text, sub_text;

	start_pt = cv::Point((int)(image.cols / 2.0) + char_left, align_top + char_top);
	text = std::to_string((int)(params.distance * 1000) / 1000.);
	sub_text = text.substr(0, text.length() - 3);
	cv::putText(image, sub_text, start_pt, cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1, 8, 0);

	start_pt = cv::Point((int)(image.cols / 2.0) + char_left, align_top + table_height + char_top);
	text = std::to_string((int)(params.raw_angle * 1000) / 1000.);
	sub_text = text.substr(0, text.length() - 3);
	cv::putText(image, sub_text, start_pt, cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1, 8, 0);

	start_pt = cv::Point((int)(image.cols / 2.0) + char_left, align_top + table_height * 2 + char_top);
	text = std::to_string((int)(params.pitch_angle * 1000) / 1000.);
	sub_text = text.substr(0, text.length() - 3);
	cv::putText(image, sub_text, start_pt, cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1, 8, 0);

	start_pt = cv::Point((int)(image.cols / 2.0) + char_left, align_top + table_height * 3 + char_top);
	text = std::to_string((int)(params.yoll_angle * 1000) / 1000.);
	sub_text = text.substr(0, text.length() - 3);
	cv::putText(image, sub_text, start_pt, cv::FONT_HERSHEY_COMPLEX, 1.0, cv::Scalar(0, 0, 255), 1, 8, 0);	
}

/**
* @brief: put one character in image
* @special: 0 (1, 2) 3 (4, 5) \0  (1, 2) is one character
**/
void UI::ui_put_text(cv::Mat &image, const wchar_t* text, cv::Point &loc, cv::Scalar color)
{
    for(int i = 0; text[i] != '\0'; ++i) {
    	UI::ui_put_wchar(image, text[i], loc, cv::Scalar(0, 0, 255));
    }
}

/**
* @brief: put one word(double char) on image
**/
void UI::ui_put_wchar(cv::Mat &image, wchar_t wc, cv::Point &pos, cv::Scalar color)
{
	FT_UInt glyph_index = FT_Get_Char_Index(m_face, wc);
	FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
	FT_Render_Glyph(m_face->glyph, FT_RENDER_MODE_MONO);
 
	FT_GlyphSlot slot = m_face->glyph;
 
	int rows = slot->bitmap.rows;
	int cols = slot->bitmap.width;
 
	for (int i = 0; i < rows; ++i) {
		for(int j = 0; j < cols; ++j) {
			int off  = i * slot->bitmap.pitch + j / 8;
			
			if (slot->bitmap.buffer[off] & (0xC0 >> (j%8))) {
				int r = pos.y - (rows-1-i);
				int c = pos.x + j;
				
				if(r >= 0 && r < image.rows && c >= 0 && c < image.cols) {
					cv::Vec3b pixel = image.at<cv::Vec3b>(cv::Point(c, r));
					cv::Scalar scalar = cv::Scalar(pixel.val[0], pixel.val[1], pixel.val[2]);

					float p = m_fontDiaphaneity;
					for (int k = 0; k < 4; ++k) {
						scalar.val[k] = scalar.val[k]*(1-p) + color.val[k]*p;
					}
 
					image.at<cv::Vec3b>(cv::Point(c, r))[0] = (uint8_t)(scalar.val[0]);
					image.at<cv::Vec3b>(cv::Point(c, r))[1] = (uint8_t)(scalar.val[1]);
					image.at<cv::Vec3b>(cv::Point(c, r))[2] = (uint8_t)(scalar.val[2]);
				}
			}
		}
	}
	
	double space = m_fontSize.val[0]*m_fontSize.val[1];
	double sep   = m_fontSize.val[0]*m_fontSize.val[2];
	
	pos.x += (int)((cols? cols: space) + sep);
}

/**
* @brief: get fonts size
**/
void UI::ui_get_font(int* type, cv::Scalar* size, bool* underline, float* diaphaneity)
{
    if (type) *type = m_fontType;
    if (size) *size = m_fontSize;
    if (underline) *underline = m_fontUnderline;
    if (diaphaneity) *diaphaneity = m_fontDiaphaneity;
}

/**
* @brief: set fonts size
**/
void UI::ui_set_font(int* type, cv::Scalar* size, bool* underline, float* diaphaneity)
{
	if (type) {
		if(type >= 0) m_fontType = *type;
	}
	if (size) {
		m_fontSize.val[0] = std::fabs(size->val[0]);
		m_fontSize.val[1] = std::fabs(size->val[1]);
		m_fontSize.val[2] = std::fabs(size->val[2]);
		m_fontSize.val[3] = std::fabs(size->val[3]);
	}
	if (underline) {
		m_fontUnderline   = *underline;
	}
	if (diaphaneity) {
		m_fontDiaphaneity = *diaphaneity;
	}
 
	FT_Set_Pixel_Sizes(m_face, (int)m_fontSize.val[0], 0);
}

/**
* @brief: restore font settings
**/
void UI::restore_fonts()
{
	m_fontType = 0;
	
	m_fontSize.val[0] = 20;
	m_fontSize.val[1] = 0.5;
	m_fontSize.val[2] = 0.1;
	m_fontSize.val[3] = 0;
	
	m_fontUnderline   = false;
	
	m_fontDiaphaneity = 1.0;

	FT_Set_Pixel_Sizes(m_face, (int)m_fontSize.val[0], 0);
}