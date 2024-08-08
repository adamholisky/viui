#ifndef VIOS_VUI_TEST_INCLUDED
#define VIOS_VUI_TEST_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "vui/vui.h"

void vui_main_test_loop( void );

void example_create_with_layout_engine( void );
void test_window_event_handler( vui_event *e );

void example_create_without_layout_engine( void );
void button1_on_mouse_up( vui_event *e );
void button2_on_mouse_up( vui_event *e );

#ifdef __cplusplus
}
#endif
#endif