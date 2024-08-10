#include "vit.h"

/**
 * @brief Dictionary tests
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int vit_dictionary_test( int argc, char *argv[] ) {
	vdf( "in dict tests\n" );

	vi_dictionary *d = vmalloc(sizeof(vi_dictionary));

	vi_dictionary_create(d);

	vi_dict_add( d, "Picard", "Enterprise D" );
	vi_dict_add( d, "Kirk", "Enterprise" );
	vi_dict_add( d, "Janeway", "Voyager" );
	vi_dict_add( d, "Archer", "Enterprise NX-01" );
	vi_dict_add( d, "Sisko", "Defiant" );
	vi_dict_add( d, "Picard", "Enterprise E" );

	vi_dictionary_dump_diagnostic(d);
	
}

/**
 * @brief Create the dictionary
 * 
 * @param d 
 * @return int 
 */
int vi_dictionary_create( vi_dictionary *d ) {
	d->head = NULL;
	d->tail = NULL;
	d->count = 0;
}

/**
 * @brief Destory the dictionary
 * 
 * @param d 
 * @return int 
 */
int vi_dictionary_destroy( vi_dictionary *d ) {
	if( d->head == NULL ) {
		return 0;
	}

	do {
		vi_dictionary_item *item = d->head;
		d->head = d->head->next;

		free(item->key);
		free(item->value);
		free(item);

		d->count--;
	} while( d->head != NULL );
}

/**
 * @brief Add key => value to the dictionary
 * 
 * @param d 
 * @param key 
 * @param value 
 * @return int 
 */
int vi_dictionary_add( vi_dictionary *d, char *key, void *value ) {
	// Check if there's a key already present
	vi_dictionary_item *old_item = vi_dictionary_get_item( d, key );

	if( old_item != NULL ) {
		uint32_t hash = vi_hash( value, strlen(value) );
		if( old_item->hash == hash ) {
			return 0;
		}

		old_item->value = value;
		old_item->hash = hash;
		old_item->modified++;

		return 0;
	}

	// Setup the values for the item
	vi_dictionary_item *item = vmalloc(sizeof(vi_dictionary_item));

	item->key = vmalloc(strlen(key));
	strcpy( item->key, key );
	item->value = value;
	item->hash = vi_hash( item->key, strlen(item->key) );
	item->next = NULL;

	// If first item, set appropriately and bail
	if( d->tail == NULL ) {
		d->tail = item;
		d->head = item;
		d->count = 1;

		return 0;
	}

	// Otherwise link the item in
	d->tail->next = item;
	d->tail = item;
	d->count++;
}

/**
 * @brief Returns the given value from the key
 * 
 * @param d 
 * @param key 
 * @return int 
 */
void *vi_dictionary_get_value( vi_dictionary *d, char *key ) {
	// Bail if dictionary is empty
	if( d->head == NULL ) {
		return NULL;
	}

	vi_dictionary_item *item = vi_dictionary_get_item( d, key );
	if( item == NULL ) {
		return NULL;
	}

	return item->value;
}

vi_dictionary_item *vi_dictionary_get_item( vi_dictionary *d, char *key ) {
	// Bail if dictionary is empty
	if( d->head == NULL ) {
		return NULL;
	}

	// Setup needed variables
	vi_dictionary_item *item = d->head;
	uint32_t hash = vi_hash( key, strlen(key) );

	do {
		if( item->hash == hash ) {
			return item;
		}

		item = item->next;
	} while( item != NULL );

	return NULL;
}

int vi_dictionary_remove_key( vi_dictionary *d, char *key ) {

}

bool vi_dictionary_has_key( vi_dictionary *d, void *value ) {

}

int vi_dictionary_for_each( vi_dictionary *d, vi_dictionary_for_each_callback callback ) {

}

void vi_dictionary_dump_diagnostic( vi_dictionary *d ) {
	vdf( "VI Dictionary Diagnostic Data\n" );
	vdf( "-----------------------------\n" );

	if( d == NULL ) {
		vdf( "Dictionary is NULL.\n" );
		return;
	}

	vdf( "d->head:    0x%016llX\n", d->head );
	vdf( "d->tail:    0x%016llX\n", d->tail );
	vdf( "d->count:   %d\n\n", d->count );

	vi_dictionary_item *top = d->head;

	if( top == NULL ) {
		vdf( "No items.\n" );
		return;
	}

	for( uint32_t i = 0; i < d->count; i++ ) {
		vdf( "Key: %s\n", top->key );
		vdf( "    Value:    %s\n", top->value );
		vdf( "    Hash:     0x%08X\n", top->hash );
		vdf( "    Modified: %d\n", top->modified );

		top = top->next;
	}
}