#include <vui/vui.h>
#include <vui/menubar.h>

extern vui_core vui;

vui_menubar main_menubar;
vui_handle main_menubar_handle;

vui_handle vui_menubar_create( void ) {
	main_menubar_handle = vui_allocate_handle( VUI_HANDLE_TYPE_MENUBAR );

	memset( &main_menubar, 0, sizeof(vui_menubar) );
	main_menubar.type = VUI_HANDLE_TYPE_MENUBAR;
	main_menubar.handle = main_menubar_handle;
	main_menubar.priority = 0xFFFFFFFE;
	main_menubar.is_visible = true;

	main_menubar.x = 0;
	main_menubar.y = 0;
	main_menubar.width = vui.width;
	main_menubar.height = 25;

	main_menubar.next_menu_x = 5;
	main_menubar.next_menu_y = 5;

	vui_set_handle_data( main_menubar_handle, &main_menubar );

	vui_create_cleanup( main_menubar_handle );
	return main_menubar_handle;
}

void vui_menubar_draw( vui_handle H ) {
	vui_theme *theme = vui_get_active_theme();
	vui_menubar *mb = &main_menubar;
	vui_menu *head = mb->menus;

	vui_draw_rect( 0, 0, vui.width, 25, theme->menubar_background );
	
	while( head != NULL ) {
		vui_menu_title *title = head->menu_title;

		vui_menu_title_draw_from_struct( title );

		head = head->next;
	}
}

void vui_menubar_draw_from_struct( vui_menubar *menubar ) {
	vui_menubar_draw(0);
}

void vui_menubar_add_menu( vui_handle menubar, vui_handle menu ) {
	vui_menubar *mb = vui_get_handle_data( menubar );
	vui_menu *m = vui_get_handle_data( menu );

	vui_menu *head = mb->menus;

	bool found = false;

	if( head == NULL ) {
		mb->menus = m;
	} else {
		do {
			if( head->next == NULL ) {
				head->next = m;
				found = true;
			} else {
				head = head->next;
			}
		} while( !found );

		if( head == NULL ) {
			vdf( "MB Insert head is NULL. Failing.\n" );
			return;
		}
	}

	uint16_t box_w = 0;
	uint16_t box_h = 0;

	// Next setup the menu title

	vui_handle menu_title_handle = vui_menu_title_create( m->text, menu );
	vui_menu_title *mt = vui_get_handle_data( menu_title_handle );

	vui_string_ttf_get_box( mt->text, vui_font_get_font("noto-sans-bold"), 13, &box_w, &box_h );
	vui_menu_title_set_box( menu_title_handle, mb->next_menu_x, 0, box_w, 25 );

	mb->next_menu_x = mb->next_menu_x + box_w + 17;

	vui_add_to_parent( menubar, menu_title_handle );

	return;	
}

