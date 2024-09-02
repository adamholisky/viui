#include <vui/vui.h>
#include <vui/menubar.h>
#include <vui/menu.h>

vui_handle vui_menu_item_create( char *text, char *name, vui_handle menu ) {
	vui_handle H = vui_allocate_handle( VUI_HANDLE_TYPE_MENUITEM );
	vui_menu_item *item = vmalloc( sizeof(vui_menu_item) );
	memset( item, 0, sizeof(vui_menu_item) );

	item->handle = H;
	item->menu = vui_get_handle_data(menu);
	item->is_visible = true;
	strcpy( item->name, name );
	strcpy( item->text, text );

	vui_set_handle_data( H, item );
	vui_create_cleanup( H );
	return H;
}

void vui_menu_item_set_box( vui_handle H, uint16_t x, uint16_t y, uint16_t width, uint16_t height ) {
	vui_menu_item *item = vui_get_handle_data(H);

	item->x = x;
	item->y = y;
	item->absolute_x = x;
	item->absolute_y = y;
	item->width = width + 10;
	item->height = height;

	item->ops.default_on_mouse_enter = vui_menu_item_on_mouse_enter;
	item->ops.default_on_mouse_exit = vui_menu_item_on_mouse_exit;
	item->ops.default_on_mouse_down = vui_menu_item_on_mouse_down;
	item->ops.default_on_mouse_up = vui_menu_item_on_mouse_up;
}

void vui_menu_item_draw( vui_handle H ) {
	vui_menu_item_draw_from_struct( vui_get_handle_data(H) );
}

void vui_menu_item_draw_from_struct( vui_menu_item *item ) {
	vui_theme *theme = vui_get_active_theme();

	uint32_t bg_color = item->is_hover ? theme->menubar_select : theme->menubar_background;

	vui_draw_rect( item->absolute_x, item->absolute_y, item->width, item->height, bg_color );
	vui_draw_string_ttf( item->text, item->x + 5, item->y + 5, theme->menubar_foreground, bg_color, vui_font_get_font("noto-sans-bold"), 13, VUI_DRAW_FLAGS_NONE );
}

void vui_menu_item_on_mouse_enter( vui_event *e ) {
	vui_menu_item *item = vui_get_handle_data( e->H );

	item->is_hover = true;

	vui_menu_item_draw_from_struct(item);
	vui_refresh_handle(e->H);
}

void vui_menu_item_on_mouse_exit( vui_event *e ) {
	vui_menu_item *item = vui_get_handle_data( e->H );

	item->is_hover = false;

	vui_menu_item_draw_from_struct(item);
	vui_refresh_handle(e->H);
}

void vui_menu_item_on_mouse_down( vui_event *e ) {
	vui_menu_item *item = vui_get_handle_data( e->H );

	item->is_hover = true;

	vui_menu_item_draw_from_struct(item);
	vui_refresh_handle(e->H);
}

void vui_menu_item_on_mouse_up( vui_event *e ) {
	vui_menu_item *item = vui_get_handle_data( e->H );

	item->is_hover = false;

	vui_menu_item_draw_from_struct(item);
	vui_refresh_handle(e->H);

	vui_event menu_title_event;
	vui_menu *m = item->menu;
	vui_menu_title *title = m->menu_title;
	menu_title_event.H = title->handle;

	vui_menu_title_on_mouse_up(&menu_title_event);
}