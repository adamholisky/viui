#include "vit.h"
#include "vui/vui.h"
#include "vui/layout.h"
#include "vui/event.h"
#include "vui/font.h"
#include "vui/desktop.h"
#include "vui/window.h"
#include "vui/label.h"
#include "vui/button.h"
#include "vui/alert.h"

extern vui_core vui;

void vui_simple_alert( char *text ) {
	vui_alert( text, "Ok", NULL );
}

int vui_alert( char *text, char *button_1_text, char *button_2_text ) {
	uint16_t text_width = 400;
	uint16_t text_height = 150;
	uint16_t win_width = text_width + 50;
	uint16_t win_height = text_height + 50 + 100;
	uint16_t win_x = (vui.width / 2) - (win_width / 2);
	uint16_t win_y = (vui.height / 2) - (win_height / 2);

	vui_handle alert_window = vui_window_create( win_x, win_y, win_width, win_height, VUI_WINDOW_FLAG_NO_TITLE_BAR );

	vui_handle alert_text = vui_label_create( win_x + 25, win_y + 25, text, VUI_LABEL_FLAG_TRANSPARENT, alert_window );
	vui_label_set_color( alert_text, 0xFFFFFF, 0x000000 );

	vui_window_set_priority( alert_window, 10 );
	
	vui_set_visible( alert_window, true );
	vui_draw_parents();
}