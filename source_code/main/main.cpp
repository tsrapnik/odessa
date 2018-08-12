#include "mailbox_framebuffer.h"
#include "boot.h"

int main( void )
{
	boot();
	return 0;
}

//dummy functions to avoid linker complaints.
extern "C" void __cxa_pure_virtual()
{
	while( 1 );
}

extern "C" void __aeabi_atexit( void )
{
	while( 1 );
}

extern "C" void __dso_handle( void )
{
	while( 1 );
}

extern "C" void __cxa_guard_acquire( void )
{}

extern "C" void __cxa_guard_release( void )
{}

extern "C" void __cxa_atexit( void )
{}