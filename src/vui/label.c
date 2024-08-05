#include <vui/vui.h>
#include <vui/label.h>

vui_handle vui_label_create( uint16_t x, uint16_t y, char *text, uint32_t flags ) {
	vui_theme *theme = vui_get_active_theme();
	vui_handle H = vui_allocate_handle( VUI_HANDLE_TYPE_LABEL );

	vui_label *label = vmalloc( sizeof(vui_label) );

	label->handle = H;
	label->flags = flags;

	label->x = x;
	label->y = y;
	label->width = 0;
	label->height = 0;

	label->color_foreground = COLOR_RGB_BLACK;
	label->color_background = COLOR_RGB_WHITE;

	label->text = vmalloc( strlen(text) );
	strcpy( label->text, text );

	vui_set_handle_data( H, label );

	return H;
}

void vui_label_draw( vui_handle H ) {
	vui_label_draw_from_struct( vui_get_handle_data(H) );
}

void vui_label_set_color( vui_handle H, uint32_t foreground, uint32_t background ) {
	vui_label *label = vui_get_handle_data(H);

	label->color_background = background;
	label->color_foreground = foreground;
}

void vui_label_draw_from_struct( vui_label *label ) {
	if( label->flags & VUI_LABEL_FLAG_NO_SMOOTHING ) {
		vui_draw_string( label->text, label->x, label->y, label->color_foreground, label->color_background, NULL, false );
	} else {
		vui_draw_string( label->text, label->x, label->y, label->color_foreground, label->color_background, NULL, true );
	}
	
}