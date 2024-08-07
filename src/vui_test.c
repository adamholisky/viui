#include "vit.h"
#include "vui_test.h"
#include "vui/vui.h"
#include "vui/font.h"
#include "vui/desktop.h"
#include "vui/window.h"
#include "vui/label.h"
#include "vui/console.h"
#include "vui/button.h"

vui_handle main_con;

extern vui_core vui;

/**
 * @brief Test main loop for VUI development. Works on dev and OS.
 * 
 */
void vui_main_test_loop( void ) {
	vui_font_initalize();

	#ifdef VI_ENV_OS
	vui_font_load( VUI_FONT_TYPE_PSF, "Zap Light", "/usr/share/fonts/zap-light20.psf" );
	vui_font_load( VUI_FONT_TYPE_PSF, "Zap VGA", "/usr/share/fonts/zap-ext-vga16.psf" );
	#else
	vui_font_load( VUI_FONT_TYPE_PSF, "Zap Light", "zap-light20.psf" );
	vui_font_load( VUI_FONT_TYPE_PSF, "Zap VGA", "zap-ext-vga16.psf" );
	#endif

	vui_theme *theme = vui_get_active_theme();

	vui_handle desktop = vui_desktop_create( 0, 0, vui.width, vui.height, VUI_DESKTOP_FLAG_NONE );
	vui_handle smooth_text = vui_label_create( 5, 768 - 25, "Adam Holisky Versions OS 6.0.0.1", VUI_LABEL_FLAG_NONE, desktop );

	vui_label_set_color( smooth_text, COLOR_RGB_WHITE, theme->window_background );


	vui_handle win = vui_window_create( 25, 25, 400, 300, VUI_WINDOW_FLAG_NONE );
	vui_window_set_title( win, "ViOS 6" );
	vui_window *win_s = vui_get_handle_data(win);
	 main_con = vui_console_create( win_s->inner_x, win_s->inner_y, win_s->inner_width, win_s->inner_height, win );

	vui_handle win2 = vui_window_create( 450, 25, 400, 200, VUI_WINDOW_FLAG_NONE );
	vui_window_set_title( win2, "ViOS Control Tests" );
	vui_window *win2_s = vui_get_handle_data(win2);

	vui_handle btn1 = vui_button_create( "Test 1", win2_s->inner_x + 10, win2_s->inner_y + 10, 75, 30, win2 );
	vui_set_event_hanlder( btn1, VUI_EVENT_MOUSE_UP, button1_on_mouse_up );

	vui_handle btn2 = vui_button_create( "Test 2", win2_s->inner_x + 10, win2_s->inner_y + 10 + 55, 75, 30, win2 );
	vui_set_event_hanlder( btn2, VUI_EVENT_MOUSE_UP, button2_on_mouse_up );

	vui_draw( desktop );
	vui_draw( win );
	vui_draw( win2 );

	vui_console_tests( main_con );
}

void button1_on_mouse_up( vui_event *e ) {
	vui_console *con_st = vui_get_handle_data(main_con);

	vui_console_put_string( con_st, "Test 1 was clicked!\n" );
	vui_draw( main_con );
}

void button2_on_mouse_up( vui_event *e ) {
	vui_console *con_st = vui_get_handle_data(main_con);

	vui_console_put_string( con_st, "Test 2 was clicked!\n" );
	vui_draw( main_con );
}