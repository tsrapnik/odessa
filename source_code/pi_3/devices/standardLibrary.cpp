#include "standardLibrary.h"
#include "buddyHeap.h"

void* operator new( long unsigned int size )
{
	return buddyHeap::allocate( size );
}
void* operator new[]( long unsigned int size )
{
	return buddyHeap::allocate( size );
}
void operator delete( void* pointer )
{
	buddyHeap::free( pointer );
}
void operator delete[]( void* pointer )
{
	buddyHeap::free( pointer );
}
void operator delete( void* pointer, long unsigned int size )
{
	buddyHeap::free( pointer );
}
void operator delete[]( void* pointer, long unsigned int size )
{
	buddyHeap::free( pointer );
}
