#ifndef VIOS_GUI_CONSOLE_INCLUDED
#define VIOS_GUI_CONSOLE_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <vui/vui.h>
#include <vui/font.h>

typedef struct {
	char *buff;
	uint32_t *color;
	bool dirty;
} console_row;

typedef struct {
	uint16_t pixel_x;
	uint16_t pixel_y;
	uint16_t pixel_width;
	uint16_t pixel_height;
	
	uint16_t padding;

	uint16_t text_area_x;
	uint16_t text_area_y;
	uint16_t text_area_width;
	uint16_t text_area_height;

	uint16_t char_width;
	uint16_t char_height;
	
	uint16_t num_cols;
	uint16_t num_rows;

	uint16_t current_row;
	uint16_t current_col;

	uint16_t current_pixel_x;
	uint16_t current_pixel_y;

	uint32_t fg_color;
	uint32_t bg_color;

	uint32_t tab_size;

	bool show_cursor;
	bool blink_hidden;

	vui_font *font;

	console_row *rows;
} vui_console;

vui_handle vui_console_create( uint16_t x, uint16_t y, uint16_t width, uint16_t height );
void vui_console_draw( vui_handle H );
void vui_console_draw_from_struct( vui_console *con );
void vui_console_tests( vui_handle H );

void vui_console_put_char( vui_console *con, uint8_t c );
void vui_console_put_char_at( vui_console *con, uint8_t c, uint16_t row, uint16_t col );
void vui_console_set_color( vui_console *con, uint32_t foreground_color, uint32_t background_color );
uint32_t vui_console_get_foreground_color( vui_console *con );
uint32_t vui_console_get_background_color( vui_console *con );
void vui_console_put_string( vui_console *con, char *str );
void vui_console_put_string_at( vui_console *con, char *str, uint16_t row, uint16_t col );
void vui_console_scroll_up( vui_console *con, bool set_current_row_col );
void vui_console_do_tab( vui_console *con );
void vui_console_do_new_line( vui_console *con );
void vui_console_do_backspace( vui_console *con );
void vui_console_update_cursor( vui_console *con );
void vui_console_blink_cursor( vui_console *con );

#ifdef __cplusplus
}
#endif
#endif