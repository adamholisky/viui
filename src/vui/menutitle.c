#include <vui/vui.h>
#include <vui/menubar.h>
#include <vui/menu.h>

vui_handle vui_menu_title_create( char *text, vui_handle menu ) {
	vui_handle H = vui_allocate_handle( VUI_HANDLE_TYPE_MENUTITLE );
	vui_menu_title *title = vmalloc( sizeof(vui_menu_title) );
	memset( title, 0, sizeof(vui_menu_title) );

	title->handle = H;

	vui_menu *m = vui_get_handle_data(menu);
	title->menu = m;
	strcpy( title->text, text );

	char temp[50];
	memset( temp, 0, 50 );
	sprintf( temp, "%s_title", m->name );
	strcpy( title->name, temp );
	
	m->menu_title = title;

	vui_set_handle_data( H, title );
	vui_create_cleanup( H );
	return H;
}

void vui_menu_title_set_box( vui_handle H, uint16_t x, uint16_t y, uint16_t width, uint16_t height ) {
	vui_menu_title *title = vui_get_handle_data(H);

	title->x = x;
	title->y = y;
	title->absolute_x = x;
	title->absolute_y = y;
	title->width = width + 10;
	title->height = height;

	title->ops.default_on_mouse_enter = vui_menu_title_on_mouse_enter;
	title->ops.default_on_mouse_exit = vui_menu_title_on_mouse_exit;
}

void vui_menu_title_draw( vui_handle H ) {
	vui_menu_title_draw_from_struct( vui_get_handle_data(H) );
}

void vui_menu_title_draw_from_struct( vui_menu_title *title ) {
	vui_theme *theme = vui_get_active_theme();

	uint32_t bg_color = title->is_hover ? theme->menubar_select : theme->menubar_background;

	vui_draw_rect( title->absolute_x, title->absolute_y, title->width, title->height, bg_color );
	vui_draw_string_ttf( title->text, title->x + 5, title->y + 5, theme->menubar_foreground, bg_color, vui_font_get_font("noto-sans-bold"), 13, VUI_DRAW_FLAGS_NONE );
}

void vui_menu_title_on_mouse_enter( vui_event *e ) {
	vui_menu_title *mt = vui_get_handle_data( e->H );

	mt->is_hover = true;

	vui_menu_title_draw_from_struct(mt);
	vui_refresh_handle(e->H);
}

void vui_menu_title_on_mouse_exit( vui_event *e ) {
	vui_menu_title *mt = vui_get_handle_data( e->H );

	mt->is_hover = false;

	vui_menu_title_draw_from_struct(mt);
	vui_refresh_handle(e->H);
}
