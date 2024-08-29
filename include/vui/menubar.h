#ifndef VIOS_VUI_MENUBAR_INCLUDED
#define VIOS_VUI_MENUBAR_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "vui/vui.h"
#include "vui/menu.h"

typedef struct {
	INSERT_VUI_COMMON

	vui_menu *menus;

	uint16_t next_menu_x;
	uint16_t next_menu_y;
} vui_menubar;

vui_handle vui_menubar_create( void );
void vui_menubar_draw( vui_handle H );
void vui_menubar_draw_from_struct( vui_menubar *menubar );
void vui_menubar_add_menu( vui_handle menubar, vui_handle menu );

#ifdef __cplusplus
}
#endif
#endif