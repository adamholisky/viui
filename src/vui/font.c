#include "vit.h"
#include "vui/vui.h"
#include "vui/font.h"

font_bitmap *main_font_bitmaps = NULL;
font_info main_font_info;

/**
 * @brief Returns the active font's bitmap
 * 
 * @return font_bitmap* pointer to the bitmap array for the given font, NULL if none were loaded 
 */
font_bitmap *vui_font_get_main_bitmap( void ) {
    return main_font_bitmaps;
}

/**
 * @brief Returns the main font's info
 * 
 * @return font_info* point to the font_info struct for the main font
 */
font_info *vui_font_get_main_info( void ) {
    return &main_font_info;
}

/**
 * @brief Loads the given PSF font, sets it as the primary font.
 * 
 * @param font_path Path to font on disk
 * @return true Font was loaded successfully
 * @return false Font failed to load
 */
#undef KDEBUG_FONT_LOAD_PSF
bool vui_font_load_psf( char *font_path ) {

	#ifdef VI_ENV_OS
	vfs_stat_data stats;
	uint32_t file_size;

	int stat_error = vfs_stat( vfs_lookup_inode(font_path), &stats );
	if( stat_error != VFS_ERROR_NONE ) {
		debugf( "Error: %d\n", stat_error );
		return false;
	}

	file_size = stats.size;

	uint8_t *data = vmalloc( file_size );
	int read_err = vfs_read( vfs_lookup_inode(font_path), data, file_size, 0 );
	if( read_err < VFS_ERROR_NONE ) {
		debugf( "Error when reading: %d\n", read_err );
		return false;
	}
	#else
	FILE *f = fopen( font_path, "r" );

	if( f == NULL ) {
		vdf( "Cannot open %s, f returned NULL.\n", font_path );
		return false;
	}

	struct stat file_meta;
	stat( font_path, &file_meta );

	uint32_t file_size = file_meta.st_size;

	uint8_t *data = vmalloc( file_size );

	if( fread( data, file_size, 1, f ) != 1 ) {
		vdf( "Read failed on %s, fread returned not 1.\n", font_path );
		return false;
	}
	#endif

	data[ file_size ] = 0;

	psf_font *header = (psf_font *)data;

	main_font_bitmaps = vmalloc( sizeof(font_bitmap) * header->numglyph);

	uint16_t *start = (uint16_t *)(data + header->headersize);

    #ifdef KDEBUG_FONT_LOAD_PSF
	vdf( "Number glyphs: %d\n", header->numglyph );
	vdf( "Bytes per glyph: %d\n", header->bytesperglyph );
	vdf( "Flags: %X\n", header->flags );
	vdf( "Height: %d\n", header->height );
	vdf( "Width: %d\n", header->width );
	vdf( "uint16: %d\n", sizeof(uint16_t) );
	vdf( "Header size: 0x%X\n", header->headersize );
	vdf( "Data Start: 0x%016llx\n", data );
	vdf( "Glyp Start: 0x%016llx\n", start );
    #endif

    main_font_info.height = header->height;
    main_font_info.width = header->width;
    main_font_info.num_glyphs = header->numglyph;

	for( int i = 0; i < header->numglyph; i++ ) {
		main_font_bitmaps[i].num = i;

		for( int j = 0; j < 20; j++ ) {
			main_font_bitmaps[i].pixel_row[j] = (*start << 8) | (*start  >> 8);

			*start++;
		}
	}

    return true;
}

/**
 * @brief Loads the provided bdf font, sets as primary font
 * 
 * @param font_path Path to the font on disk
 * @return true Font was loaded successfully
 * @return false Loading the font failed.
 * 
 * Test font: "/usr/share/fonts/gomme10x20n.bdf"
 */
/* bool font_load_bdf( char *font_path ) {
	vfs_stat_data stats;

	int stat_error = vfs_stat( vfs_lookup_inode(font_path), &stats );
	if( stat_error != VFS_ERROR_NONE ) {
		debugf( "Error: %d\n", stat_error );
		return false;
	}

	uint8_t *data = vfs_malloc( stats.size );
	int read_err = vfs_read( vfs_lookup_inode(font_path), data, stats.size, 0 );
	if( read_err < VFS_ERROR_NONE ) {
		debugf( "Error when reading: %d\n", read_err );
		return false;
	}

	data[ stats.size ] = 0;

	bool keep_going = true;
	bool in_char = false;
	uint16_t current_char = 0;
	bool in_bitmap = false;
	int bitmap_line = 0;
	int j = 0;

	do {
		char line[250];
		
		memset( line, 0, 250 );

		for( int i = 0; i < 250; i++ ) {
			if( *data == '\n' ) {
				data++;
				i = 250;
			} else {
				line[i] = *data++;
			}
		}

		if( in_char ) {
			if( in_bitmap ) {
				if( strncmp(line, "ENDCHAR", sizeof("ENDCHAR") - 1) == 0 ) {
					in_char = false;
					in_bitmap = false;
					bitmap_line = 0;
					j++;
				} else {
					main_font_bitmaps[j].pixel_row[bitmap_line] = strtol(line, NULL, 16);

					bitmap_line++;
				}
			} else {
				if( strncmp(line, "ENCODING ", sizeof("ENCODING ") - 1) == 0 ) {
					char *encoding_line = line;
					encoding_line = encoding_line + sizeof("ENCODING ") - 1;
					current_char = atoi(encoding_line);
					main_font_bitmaps[j].num = current_char;
					//debugf( "Found 0x%X (%d) '%c'\n", current_char, current_char, current_char );
				} else if( strncmp(line, "BITMAP", sizeof("BITMAP") - 1) == 0 ) {
					in_bitmap = true;
				}
			}
		} else {
			if( strncmp(line, "CHARS ", 6) == 0 ) {
				char *chars_line = line;
				chars_line = chars_line + 6;
				uint16_t num_chars = atoi(chars_line);
				main_font_bitmaps = kmalloc( sizeof(font_bitmap) * num_chars );
				//debugf( "Num Chars: %d\n", num_chars );
			} else if( strncmp(line, "STARTCHAR ", sizeof("STARTCHAR ") - 1) == 0 ) {
				in_char = true;
			}
		}

		if( *data == 0 ) {
			keep_going = false;
		}
	} while( keep_going );

	//debugf( "added %d chars\n", j );

    return true;
}
 */