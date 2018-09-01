#ifdef __cplusplus
extern "C" {
#endif

void *__dso_handle;

void __cxa_atexit (void *pThis, void (*pFunc)(void *pThis), void *pHandle)
{
	// TODO
}
void sysinit (void)
{
	// clear BSS
	extern unsigned char __bss_start;
	extern unsigned char _end;
	for (unsigned char *pBSS = &__bss_start; pBSS < &_end; pBSS++)
	{
		*pBSS = 0;
	}

	// call construtors of static objects
	extern void (*__init_start) (void);
	extern void (*__init_end) (void);
	for (void (**pFunc) (void) = &__init_start; pFunc < &__init_end; pFunc++)
	{
		(**pFunc) ();
	}

	extern int main (void);
	main();
}

#ifdef ARM_ALLOW_MULTI_CORE

void sysinit_secondary (void)
{
	main_secondary ();

	//halt ();
}

#endif

#ifdef __cplusplus
}
#endif