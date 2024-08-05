#include <vui/vui.h>
#include <vui/console.h>
#include <vui/font.h>

vui_handle vui_console_create( uint16_t x, uint16_t y, uint16_t width, uint16_t height ) {
	vdf( "Create Console: x: %d    y: %d    width: %d    height: %d\n", x, y, width, height );

	vui_theme *theme = vui_get_active_theme();
	vui_console *con = vmalloc( sizeof(vui_console) );

	vui_handle H = vui_allocate_handle( VUI_HANDLE_TYPE_CONSOLE );
	vui_set_handle_data( H, con );

	con->pixel_x= x;
	con->pixel_y = y;
	con->pixel_width = width;
	con->pixel_height = height;

	con->padding = 5;

	con->text_area_x = con->pixel_x + con->padding;
	con->text_area_y = con->pixel_y + con->padding;
	con->text_area_width = con->pixel_width - (2*con->padding);
	con->text_area_height = con->pixel_height - (2*con->padding);

	con->current_pixel_x = con->text_area_x;
	con->current_pixel_y = con->text_area_y;
	
	con->font = vui_font_get_font( "Zap VGA" );
	con->char_width = con->font->info.width;
	con->char_height = con->font->info.height;

	con->num_cols = con->text_area_width / con->char_width;
	con->num_rows = con->text_area_height / con->char_height;

	con->fg_color = 0x00eaeaea;
	con->bg_color = 0x00232323;

	vdf( "    num_cols: %d    num_rows: %d\n", con->num_cols, con->num_rows );

	con->rows = vmalloc( sizeof(console_row) * con->num_rows );

	for( int i = 0; i < con->num_rows; i++ ) {
		con->rows[i].buff = vmalloc( sizeof(char) * con->num_cols );
		memset( con->rows[i].buff, 0, sizeof(char) * con->num_cols );
		con->rows[i].color = vmalloc( sizeof(uint32_t) * con->num_cols );
		memset( con->rows[i].color, 0, sizeof(uint32_t) * con->num_cols );
		con->rows[i].dirty = false;
	}

	con->current_row = 1;
	con->current_col = 1;
	con->tab_size = 4;

	con->show_cursor = false;
	con->blink_hidden = false;

	return H;
}

void vui_console_draw( vui_handle H ) {
	vui_console_draw_from_struct( vui_get_handle_data(H) );
}

void vui_console_draw_from_struct( vui_console *con ) {
	vui_draw_rect( con->pixel_x, con->pixel_y, con->pixel_width, con->pixel_height, con->bg_color );

	for( int i = 0; i < con->num_rows; i++ ) {
		if( con->rows[i].dirty ) {
			uint16_t x = con->text_area_x;
			uint16_t y = con->text_area_y + (con->char_height * i);

			for( int j = 0; j < con->num_cols; j++ ) {
				char c = (con->rows[i].buff[j] == 0 ? ' ' : con->rows[i].buff[j]);

				vui_draw_char_with_color( c, x, y, con->fg_color, con->bg_color, con->font, true );

				x = x + con->char_width;
			}
		}
	}

	vui_refresh_rect( con->pixel_x, con->pixel_y, con->pixel_width, con->pixel_height );
}

void vui_console_put_char( vui_console *con, uint8_t c ) { 
	vui_console_put_char_at( con, c, con->current_row, con->current_col );
}

void vui_console_put_char_at( vui_console *con, uint8_t c, uint16_t row, uint16_t col ) {
	uint16_t cursor_x = con->current_pixel_x;
	uint16_t cursor_y = con->current_pixel_y;

	bool cursor_visibility = con->show_cursor;
	con->show_cursor = false;

	switch( c ) {
		case '\t':
			vui_console_do_tab( con );
			break;
		case '\n':
			vui_console_do_new_line( con );
			break;
		case '\b':
			vui_console_do_backspace( con );
			break;
		default:
			if( con->current_col == con->num_cols + 1 ) {
				vui_console_put_char( con, '\n' );
			}

			con->rows[row - 1].buff[col - 1] = c;
			con->rows[row - 1].color[col - 1] = 0;
			con->rows[row - 1].dirty = true;

			//*(con->buffer + (row*con->num_cols) + col) = c;
			vui_draw_char_with_color( c, con->current_pixel_x, con->current_pixel_y, con->fg_color, con->bg_color, con->font, true );
			//vui_refresh_rect( con->current_pixel_x, con->current_pixel_y, con->char_width, con->char_height );

			con->current_col++;
			con->current_pixel_x = con->current_pixel_x + con->char_width;
	}

	con->show_cursor = cursor_visibility;

	if( con->show_cursor == true ) {
		// Clear the previous cursor if a new line
		if( c == '\n' ) {
			vui_draw_char_with_color( ' ', con->current_pixel_x, con->current_pixel_y, con->fg_color, con->bg_color, con->font, true );
			//vui_refresh_rect( con->current_pixel_x, con->current_pixel_y, con->char_width, con->char_height );
		}

		vui_console_update_cursor( con );
	}
}

void vui_console_set_color( vui_console *con, uint32_t foreground_color, uint32_t background_color ) {
	con->fg_color = foreground_color;
	con->bg_color = background_color;
}

uint32_t vui_console_get_foreground_color( vui_console *con ) {
	return con->fg_color;
}

uint32_t vui_console_get_background_color( vui_console *con ) {
	return con->bg_color;
}

void vui_console_put_string( vui_console *con, char *str ) {
	vui_console_put_string_at( con, str, con->current_row, con->current_col );
}

void vui_console_put_string_at( vui_console *con, char *str, uint16_t row, uint16_t col ) {
	for( ; *str; *str++ ) {
		vui_console_put_char_at( con, *str, con->current_row, con->current_col );
	}
}

void vui_console_scroll_up( vui_console *con, bool set_current_row_col ) {
	for(int i = 0; i < con->num_rows - 2; i++ ) {
		memcpy( con->rows[i].buff, con->rows[i + 1].buff, sizeof(char) * con->num_cols );
		memcpy( con->rows[i].color, con->rows[i + 1].color, sizeof(uint32_t) * con->num_cols );
		con->rows[i].dirty = true;
	}

	memset( con->rows[con->num_rows - 1].buff, 0, sizeof(char) * (con->num_cols) );



/* 	// Move the console text up one line
	vui_move_rect(  con->text_area_x, con->text_area_y, 
					con->text_area_width, con->text_area_height - con->char_height,
					con->text_area_x, con->text_area_y + con->char_height, 
					con->text_area_width, con->text_area_height - con->char_height );
	
	// Fill in the last line to blank
	vui_draw_rect(  con->text_area_x, con->text_area_y + (con->num_rows - 1) * con->char_height, con->text_area_width, con->char_height, con->bg_color ); */

	// If we're asked to set row, col to the last line, do so
	if( set_current_row_col ) {
		con->current_pixel_x = con->text_area_x;
		con->current_pixel_y = con->text_area_y + ((con->num_rows - 1) * con->char_height);
		con->current_col = 1;
		con->current_row = con->num_rows;
	}
}

void vui_console_do_tab( vui_console *con ) {
	// Only do a tab if there's space left. Need to look into this.
	if( con->current_col + con->tab_size < con->num_cols ) {
		if( con->current_col == 1 ) {
			for( int i = 0; i < con->tab_size; i++ ) {
				vui_console_put_char( con, ' ');
			}
		} else {
			int num_spaces = (con->current_col - 1) % con->tab_size;

			if( num_spaces == 0 ) {
				num_spaces = con->tab_size;
			}

			for( int i = 0; i < num_spaces; i++ ) {
				vui_console_put_char( con, ' ');
			}
		}
	}
}

void vui_console_do_new_line( vui_console *con ) {
	con->current_col = 1;
	
	if( con->current_row == con->num_rows ) {
		vui_console_scroll_up( con, true );
	} else {
		con->current_row++;
		con->current_pixel_y = con->current_pixel_y + con->char_height;
	}

	con->current_pixel_x = con->text_area_x;
}

void vui_console_do_backspace( vui_console *con ) {
	bool cursor_visibility = con->show_cursor;

	// set to false while we're backspacing so a timer tick doesn't mess up the drawn cursor position
	con->show_cursor = false; 

	con->current_col--;
	con->current_pixel_x = con->current_pixel_x - con->char_width;
	vui_draw_char_with_color( ' ', con->current_pixel_x, con->current_pixel_y, con->fg_color, con->bg_color, con->font, true );
	vui_refresh_rect( con->current_pixel_x, con->current_pixel_y, con->char_width, con->char_height );

	if( cursor_visibility == true ) {
		vui_draw_char_with_color( ' ', con->current_pixel_x, con->current_pixel_y, con->fg_color, con->bg_color, con->font, true );
		vui_refresh_rect( con->current_pixel_x, con->current_pixel_y, con->char_width, con->char_height );
	}

	// restore cursor visiblity
	con->show_cursor = cursor_visibility;
}

/**
 * @brief Move the cursor its correct position.
 * 
 * @todo Redo this to keep track of the cursor position independently
 * 
 * @param con vui console to operate on
 */
void vui_console_update_cursor( vui_console *con ) {
	if( con->show_cursor == true ) {
		vui_draw_char_with_color( 0xDB, con->current_pixel_x, con->current_pixel_y, con->fg_color, con->bg_color, con->font, true );
		vui_refresh_rect( con->current_pixel_x, con->current_pixel_y, con->char_width, con->char_height );
	}
}

void vui_console_blink_cursor( vui_console *con ) {
	uint8_t c = 0;

	if( con->show_cursor == true ) {
		if( con->blink_hidden == true ) {
			c = 0xDB;
			con->blink_hidden = false;
		} else {
			c = ' ';
			con->blink_hidden = true;
		}

		vui_draw_char_with_color( c, con->current_pixel_x, con->current_pixel_y, con->fg_color, con->bg_color, con->font, true );
		vui_refresh_rect( con->current_pixel_x, con->current_pixel_y, con->char_width, con->char_height );
	}
}

char long_string_for_test[] = "Versions VI Console Test\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25";

void vui_console_tests( vui_handle H ) {
	vui_console *con = vui_get_handle_data(H);

	//vui_console_put_string( con, "VUI Console test suite." );

	vui_console_put_string( con, long_string_for_test );
	vui_console_draw_from_struct( con );
}