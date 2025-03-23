#include <string.h>

/* REVIEW: Change function to only one return */
int arrFind ( char *elem, char *array [], int size )
{
	for ( int i = 0; i < size; i++ ) {
		if ( strcmp ( elem, array[i] ) == 0 ) {
			return i;
		}
	}

	return -1;
}
