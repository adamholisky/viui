#include "vit.h"
#include "vui_test.h"
#include "vui/vui.h"
#include "vui/layout.h"
#include "vui/event.h"
#include "vui/font.h"
#include "vui/desktop.h"
#include "vui/window.h"
#include "vui/label.h"
#include "vui/console.h"
#include "vui/button.h"
#include <vui/menubar.h>

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

/* 	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 'V' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 'e' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 'r' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 's' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 'i' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 'o' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), 'n' );
	vui_font_create_aa_mask( vui_font_get_font("Zap VGA"), '6' ); */

	vui_draw_string( "Version", 10, 10, COLOR_RGB_BLACK, COLOR_RGB_WHITE, vui_font_get_font("Zap VGA"), VUI_DRAW_FLAGS_IMMEDIATE );
	

	vui_theme *theme = vui_get_active_theme();

	vui_handle menubar = vui_menubar_create();
	vui_handle_set_name( menubar, "main_menubar" );

	vui_handle desktop = vui_desktop_create( 0, 25, vui.width, vui.height - 25, VUI_DESKTOP_FLAG_NONE );
	vui_handle smooth_text = vui_label_create( 5, 768 - 25, "Versions OS 6.0.0.1", VUI_LABEL_FLAG_TRANSPARENT, desktop );
	vui_label_set_color( smooth_text, COLOR_RGB_WHITE, theme->window_background );
	vui_handle_set_name( desktop, "desktop" );



	vui_handle win = vui_window_create( 25, 40, 400, 300, VUI_WINDOW_FLAG_NONE );
	vui_window_set_title( win, "ViOS 6" );
	vui_window *win_s = vui_get_handle_data(win);
	main_con = vui_console_create( win_s->inner_x, win_s->inner_y, win_s->inner_width, win_s->inner_height, win );
	vui_handle_set_name( win, "window_console" );

	vui_draw( menubar );
	vui_draw( desktop );
	vui_draw( win );

	vui_set_event_handler( desktop, VUI_EVENT_ALL, test_window_event_handler );
	vui_set_event_handler( win, VUI_EVENT_ALL, test_window_event_handler );

	example_create_with_layout_engine();

	vui_console_tests( main_con );
}

void example_create_with_layout_engine( void ) {
	vui_handle win = vui_window_create( 450, 40, 400, 200, VUI_WINDOW_FLAG_NONE );
	vui_window_set_title( win, "ViOS Control Tests -- Layout Engine" );
	vui_set_event_handler( win, VUI_EVENT_ALL, test_window_event_handler );
	vui_handle_set_name( win, "window_ctest" );	

	vui_layout *layout = vui_layout_init( VUI_LAYOUT_GRID_3x3, win );
	
	//vui_layout_dump_diagnostic( layout );

	for( int i = 0; i < 3*3; i++ ) {
		char text[50];
		char name[50];

		sprintf( text, "My Button %d", i + 1 );
		sprintf( name, "my_button_%d", i + 1 );

		vui_layout_add_button( vui_layout_get_next_free(layout), name, text );
	}

	vui_draw( win );
}

void test_window_event_handler( vui_event *e ) {
	char str[50];

	if( e->type != VUI_EVENT_MOUSE_MOVE ) {
		sprintf( str, "Event: %s    name=%s\n", vui_event_type_to_string(e->type), e->name );
		vui_console_put_string( vui_get_handle_data(main_con), str );
	}
}

void example_create_without_layout_engine( void ) {
	vui_handle win2 = vui_window_create( 450, 25, 400, 200, VUI_WINDOW_FLAG_NONE );
	vui_window_set_title( win2, "ViOS Control Tests -- Free Placement" );
	vui_window *win2_s = vui_get_handle_data(win2);

	vui_handle btn1 = vui_button_create( "Test 1", win2_s->inner_x + 10, win2_s->inner_y + 10, 75, 30, win2 );
	vui_set_event_handler( btn1, VUI_EVENT_MOUSE_UP, button1_on_mouse_up );

	vui_handle btn2 = vui_button_create( "Test 2", win2_s->inner_x + 10, win2_s->inner_y + 10 + 55, 75, 30, win2 );
	vui_set_event_handler( btn2, VUI_EVENT_MOUSE_UP, button2_on_mouse_up );
}



void button1_on_mouse_up( vui_event *e ) {
	static int num = 0;
	char string[250];

	vui_console *con_st = vui_get_handle_data(main_con);

	sprintf( string, "Test 1 was clicked. Number of clicks: %d\n", ++num );

	vui_console_put_string( con_st, string );
}

void button2_on_mouse_up( vui_event *e ) {
	static int num = 0;
	char string[250];

	vui_console *con_st = vui_get_handle_data(main_con);

	sprintf( string, "Test 2 was clicked. Number of clicks: %d", ++num );

	vui_console_put_string( con_st, string );
}