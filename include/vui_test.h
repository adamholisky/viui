#ifndef VIOS_VUI_TEST_INCLUDED
#define VIOS_VUI_TEST_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "vui/vui.h"

void vui_main_test_loop( void );
void button1_on_mouse_up( vui_event *e );
void button2_on_mouse_up( vui_event *e );

#ifdef __cplusplus
}
#endif
#endif