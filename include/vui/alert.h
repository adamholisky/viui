#ifndef VIOS_VUI_ALERT_INCLUDED
#define VIOS_VUI_ALERT_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "vui/vui.h"

#define VUI_ALERT_FLAG_NONE 0

#define VUI_ALERT_BUTTON_1 1
#define VUI_ALERT_BUTTON_2 2

void vui_simple_alert( char *text );
int vui_alert( char *text, char *button_1_text, char *button_2_text );

#ifdef __cplusplus
}
#endif
#endif