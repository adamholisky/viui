#ifndef VIOS_VUI_MENU_INCLUDED
#define VIOS_VUI_MENU_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "vui/vui.h"
#include "vui/menubar.h"

typedef struct {
	INSERT_VUI_COMMON

	char text[50];
	void *menu;

	void *next;
} vui_menu_item;

typedef struct {
	INSERT_VUI_COMMON

	char text[50];

	vui_menu_item *items;
	void *menu_title;
	uint8_t num_items;

	uint16_t item_width_max;
	uint16_t item_height;

	void *next;
} vui_menu;

typedef struct {
	INSERT_VUI_COMMON

	char text[50];
	bool is_showing_menu;

	vui_menu *menu;
} vui_menu_title;

vui_handle vui_menu_create( char *name, char *text );
void vui_menu_draw( vui_handle H );
void vui_menu_draw_from_struct( vui_menu *menu );
void vui_menu_add_item( vui_handle menu_handle, char *name, char *text );
void vui_menu_set_display_location( vui_menu *menu, uint16_t x, uint16_t y );
void vui_menu_calculate_dimensions( vui_menu *menu );
void vui_menu_hide( vui_handle menu_handle );
void vui_menu_show( vui_handle menu_handle );
void vui_menu_visible( vui_handle menu_handle, bool visible );


vui_handle vui_menu_title_create( char *text, vui_handle menu );
void vui_menu_title_set_box( vui_handle H, uint16_t x, uint16_t y, uint16_t width, uint16_t height );
void vui_menu_title_draw_from_struct( vui_menu_title *title );
void vui_menu_title_draw( vui_handle H );
void vui_menu_title_on_mouse_enter( vui_event *e );
void vui_menu_title_on_mouse_exit( vui_event *e );
void vui_menu_title_on_mouse_down( vui_event *e );
void vui_menu_title_on_mouse_up( vui_event *e );


vui_handle vui_menu_item_create( char *text, char *name, vui_handle menu );
void vui_menu_item_set_box( vui_handle H, uint16_t x, uint16_t y, uint16_t width, uint16_t height );
void vui_menu_item_draw( vui_handle H );
void vui_menu_item_draw_from_struct( vui_menu_item *item );
void vui_menu_item_on_mouse_enter( vui_event *e );
void vui_menu_item_on_mouse_exit( vui_event *e );
void vui_menu_item_on_mouse_down( vui_event *e );
void vui_menu_item_on_mouse_up( vui_event *e );

#ifdef __cplusplus
}
#endif
#endif