#include "vit.h"
#include "vui_test.h"
#include "vui/event.h"
#include <SDL.h>
#include <SDL_render.h>

#ifndef VI_ENV_OS

int main( int argc, char *argv[] ) {
	printf( "VI Tests\n" );

	//vit_bitmap_tests( argc, argv );
	vit_dictionary_test( argc, argv );

	//vit_gui_test();

	printf( "Done.\n" );

	return 0;
}

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024

SDL_Window *window = NULL;

/**
 * @brief Run the gui tests with SDL providing the interfaces
 * 
 */
void vit_gui_test( void ) {
	// Setup the SDL environment
	SDL_Surface *screen_surface = NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL_Init failed: %s\n", SDL_GetError() );
		return;
	}

	window = SDL_CreateWindow( "VIOS UI Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	if( window == NULL ) {
		printf( "SDL Window error: %s\n", SDL_GetError() );
		return;
	}

	screen_surface = SDL_GetWindowSurface( window );
	SDL_FillRect( screen_surface, NULL, 0x00374760 );
	SDL_UpdateWindowSurface( window );

	// Setup the framebuffer
	uint32_t *fb = (uint32_t *)screen_surface->pixels;
	/* fb[ (10*SCREEN_WIDTH) + 10 ] = 0x00FFFFFF;
	fb[ (10*SCREEN_WIDTH) + 11 ] = 0x00FFFFFF;
	SDL_UpdateWindowSurface( window ); */

	// Enter VUI's GUI
	vui_init( fb, SCREEN_WIDTH, SCREEN_HEIGHT );

	vui_main_test_loop();

	// Main SDL loop
	SDL_Event e; 
	bool quit = false; 

	while( quit == false ) {
		while( SDL_PollEvent( &e ) ) { 
			vui_event vevent;
			int x, y;

			switch( e.type ) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_MOUSEMOTION:
					SDL_GetMouseState( &x, &y );

					vevent.type = VUI_EVENT_MOUSE_MOVE;
					vevent.x = x;
					vevent.y = y;

					vui_external_event_handler( VUI_EVENT_MOUSE_MOVE, x, y, false, false );
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					SDL_GetMouseState( &x, &y );

					//vdf( "mb: %d\n", e.button.button );

					bool lmb = (e.button.button == 1 ? true : false );
					bool rmb = (e.button.button == 3 ? true : false );
					
					if( e.type == SDL_MOUSEBUTTONDOWN ) {
						vui_external_event_handler( VUI_EVENT_MOUSE_DOWN, x, y, lmb, rmb );
					} else {
						vui_external_event_handler( VUI_EVENT_MOUSE_UP, x, y, lmb, rmb );
					}
					
					break;
			}
		}
	}

	// Clean up
	SDL_DestroyWindow( window );
	SDL_Quit();
}

/**
 * @brief This has to be run whenver we want to update the SDL window
 * 
 */
void vit_gui_update( void ) {
	SDL_UpdateWindowSurface( window );
}

#endif