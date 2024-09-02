#include <vui/vui.h>
#include <vui/menubar.h>
#include <vui/menu.h>

vui_handle vui_menu_create( char *name, char *text ) {
	vui_handle H = vui_allocate_handle( VUI_HANDLE_TYPE_MENU );
	vui_menu *menu = vmalloc( sizeof(vui_menu) );
	memset( menu, 0, sizeof(vui_menu) );

	menu->type = VUI_HANDLE_TYPE_MENU;
	menu->handle = H;
	menu->priority = 0x1;
	menu->next = NULL;
	strcpy( menu->text, text );
	strcpy( menu->name, name );

	vui_set_handle_data( H, menu );
	vui_create_cleanup( H );
	return H;
}

void vui_menu_draw( vui_handle H ) {
	vui_menu_draw_from_struct( vui_get_handle_data(H) );
}

void vui_menu_draw_from_struct( vui_menu *menu ) {
	// only draw if we're visible
	if( !menu->is_visible ) {
		return;
	}

	vui_theme *theme = vui_get_active_theme();
	
	vui_menu_calculate_dimensions( menu );

	vui_draw_rect( menu->x, menu->y, menu->width, menu->height, theme->border );
	vui_draw_rect( menu->inner_x, menu->inner_y, menu->inner_width, menu->inner_height, theme->menubar_background ); 

	vui_menu_item *item = menu->items;

	for( int i = 0; i < menu->num_items; i++ ) {
		vui_menu_item_set_box( item->handle, menu->inner_x, menu->inner_y + (i * menu->item_height), menu->item_width_max, menu->item_height );
		vui_draw(item->handle);

		item = item->next;
	}
}

void vui_menu_set_display_location( vui_menu *menu, uint16_t x, uint16_t y ) {
	menu->absolute_x = x;
	menu->absolute_y = y;
	menu->x = x;
	menu->y = y;
}

void vui_menu_calculate_dimensions( vui_menu *menu ) {
	uint16_t width_max = 0;

	vui_menu_item *item = menu->items;

	for( int i = 0; i < menu->num_items; i++ ) {
		if( item != NULL ) {
			uint16_t width = 0;
			uint16_t height = 0;

			vui_string_ttf_get_box( item->text, vui_font_get_font("noto-sans-bold"), 13, &width, &height );

			width_max = width > width_max ? width : width_max;

			item = item->next;
		}		
	}

	menu->item_width_max = width_max;
	menu->item_height = 25;
	
	menu->width = width_max + 10 + 2; // 50 content, 10 = 5 padding on right left, 2 = border
	menu->height = (menu->num_items * 25) + 2; // 25 per item, 5 padding on top bottom, 2 = border

	menu->inner_x = menu->x + 1;
	menu->inner_y = menu->y + 1;
	menu->inner_width = menu->width - 2;
	menu->inner_height = menu->height - 2;
}

void vui_menu_add_item( vui_handle menu_handle, char *name, char *text ) {
	vui_menu *menu = vui_get_handle_data(menu_handle);

	vui_menu_item *head = menu->items;
	vui_handle mi_handle = vui_menu_item_create( text, name, menu_handle );
	vui_menu_item *item = vui_get_handle_data( mi_handle );

	vui_add_to_parent( menu_handle, mi_handle );

	if( head == NULL ) {
		menu->items = item;
	} else {
		while( head->next != NULL ) {
			head = head->next;
		}

		head->next = item;
	}

	menu->num_items++;
}

void vui_menu_hide( vui_handle menu_handle ) {
	vui_menu_visible( menu_handle, false );
	vui_refresh_handle(menu_handle);
	vui_draw_parents();
}

void vui_menu_show( vui_handle menu_handle ) {
	vui_menu_visible( menu_handle, true );
	vui_refresh_handle(menu_handle);
}

void vui_menu_visible( vui_handle menu_handle, bool visible ) {
	vui_menu *menu = vui_get_handle_data( menu_handle );

	menu->is_visible = visible;

	vui_menu_item *head = menu->items;
	while( head != NULL ) {
		head->is_visible = visible;

		head = head->next;
	}

	vui_draw( menu_handle );
}