#include "vui/vui.h"
#include "vui/button.h"

vui_handle vui_button_create( char *text, uint16_t x, uint16_t y, uint16_t width, uint16_t height, vui_handle parent ) {
	vdf( "Create Button: x: %d    y: %d    width: %d    height: %d\n", x, y, width, height );

	vui_theme *theme = vui_get_active_theme();
	vui_button *button = vmalloc( sizeof(vui_button) );
	memset( button, 0, sizeof(vui_button) );

	vui_handle H = vui_allocate_handle( VUI_HANDLE_TYPE_BUTTON );
	vui_set_handle_data( H, button );

	button->handle = H;
	button->type = VUI_HANDLE_TYPE_BUTTON;
	button->flags = 0;

	button->width = width;
	button->height = height;
	button->x = x;
	button->absolute_x = x;
	button->y = y;
	button->absolute_y = y;

	button->inner_width = width;
	button->inner_height = height;
	button->inner_x = x;
	button->inner_y = y;

	button->width = width + VUI_BUTTON_BORDER_SIZE*2;
	button->height = height + VUI_BUTTON_BORDER_SIZE*2;
	button->inner_x = button->inner_x + VUI_BUTTON_BORDER_SIZE;
	button->inner_y = button->inner_y + VUI_BUTTON_BORDER_SIZE;

	button->color_foreground = theme->button_foreground;
	button->color_background = theme->button_background;

	strcpy( button->text, text );

	vdf( "    Inner Button: x: %d    y: %d    width: %d    height: %d\n", button->inner_x, button->inner_y, button->inner_width, button->inner_height );

	vui_add_to_parent( parent, H );
	vui_create_cleanup(H);
	return H;
}

void vui_button_draw( vui_handle H ) {
	vui_button_draw_from_struct( vui_get_handle_data(H) );
}

void vui_button_draw_from_struct( vui_button *button ) {
	vui_theme *theme = vui_get_active_theme();

	// Border
	vui_draw_rect( button->x, button->y, button->width, button->height, theme->border );

	// Inner window
	vui_draw_rect( button->inner_x, button->inner_y, button->inner_width, button->inner_height, button->color_background );

	// Text
	vui_draw_string( button->text, button->inner_x + 3, button->inner_y + 3, button->color_foreground, button->color_background, vui_font_get_font("Zap VGA"), true );
}

/**
 * @brief Sets the background color of the window
 * 
 * @param H Handle to the window
 * @param color Color to set the window to
 */
void vui_window_set_color( vui_handle H, uint32_t color_foreground, uint32_t color_background ) {
	vui_button *b = vui_get_handle_data(H);

	b->color_foreground = color_foreground;
	b->color_background = color_background;
}

/**
 * @brief Sets the window title
 * 
 * @param H Handle to the window
 * @param title String to set the title to
 */
void vui_button_set_text( vui_handle H, char *text ) {
	vui_button *b = vui_get_handle_data(H);

	strncpy( b->text, text, VUI_BUTTON_TEXT_MAX );
}