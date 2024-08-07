#include <vui/vui.h>
#include <vui/event.h>

extern vui_core vui;

/**
 * @brief 
 * 
 * @param x 
 * @param y 
 * @param lmb 
 * @param rmb 
 */
void vui_external_event_handler_click( uint16_t x, uint16_t y, bool lmb, bool rmb ) {
	vui_event event;
	memset( &event, 0, sizeof(event) );

	event.x = x;
	event.y = y;
	event.type = VUI_EVENT_MOUSE_UP;
	event.flags = (lmb == true ? event.flags | VUI_EVENT_FLAG_LMB : event.flags);
	event.flags = (rmb == true ? event.flags | VUI_EVENT_FLAG_RMB : event.flags);

	// Step 1: check if it's in the active dispatcher (window, alert, etc...)

	// Step 2: If not, search all dispatchers
	vui_handle_list *top = &vui.dispatchers;
	vui_handle dispatcher = vui_find_handler_for_event( &vui.dispatchers, &event );

	if( dispatcher != 0 ) {
		vui_common *handler_st = vui_get_handle_data(dispatcher);
		vui_handle handler = vui_find_handler_for_event( &handler_st->children, &event );

		if( handler != 0 ) {
			vui_send_event( handler, &event );
		}
	}
}

/**
 * @brief 
 * 
 * @param list 
 * @param e 
 * @return vui_handle 
 */
vui_handle vui_find_handler_for_event( vui_handle_list *list, vui_event *e ) {
	vui_handle_list *top = list;
	bool found_dispatcher = false;
	vui_common *element;

	do {
		if( top->H == 0 ) {
			// End of list reached, just return, we're not finding anything to send to
			return 0;
		}

		element = vui_get_handle_data(top->H);

		if( (e->x >= element->absolute_x) && (e->x <= element->absolute_x + element->width) ) {
			if( (e->y >= element->absolute_y) && (e->y <= element->absolute_y + element->height) ) {
				found_dispatcher = true;
			}
		}

		top = top->next;
	} while( top != NULL && !found_dispatcher );

	if( found_dispatcher ) {
		//vdf( "Found handler:  H = %d    Type: %d\n", element->handle, element->type );
		return element->handle;
	}

	return 0;
}

/**
 * @brief 
 * 
 * @param H 
 * @param e 
 */
void vui_send_event( vui_handle H, vui_event *e ) {
	//vdf( "Handler got event. H = %d    e.flags = %d    e.type = %d\n", H, e->flags, e->type );

	vui_common *element = vui_get_handle_data(H);

	void (*handler_to_call)(vui_event *) = NULL;

	switch( e->type ) {
		case VUI_EVENT_MOUSE_UP:
			//vdf( "calling on mouse up\n" );
			handler_to_call = element->ops.on_mouse_up;
			break;
	}

	if( handler_to_call != NULL ) {
		handler_to_call(e);
	}
}

/**
 * @brief 
 * 
 * @param H 
 * @param event_type 
 * @param handler 
 */
void vui_set_event_hanlder( vui_handle H, uint8_t event_type, void (*handler)(vui_event *) ) {
	vui_common *element = vui_get_handle_data(H);

	if( element == NULL ) {
		vdf( "Element was NULL in set_event_handler. Aborting.\n" );
		return;
	}

	switch( event_type ) {
		case VUI_EVENT_MOUSE_UP:
			element->ops.on_mouse_up = handler;
			vdf( "set on mouse up\n" );
			break;
	}
}

/**
 * @brief 
 * 
 * @param type 
 * @return true 
 * @return false 
 */
bool vui_is_dispatcher( uint16_t type ) {
	switch( type ) {
		case VUI_HANDLE_TYPE_DESKTOP:
		case VUI_HANDLE_TYPE_WINDOW:
			return true;
			break;
	}

	return false;
}