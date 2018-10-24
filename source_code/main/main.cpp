#include "memory.h"
#include "buddy_heap.h"
#include "mailbox_framebuffer.h"
#include "screen.h"
#include "effect_graph.h"
#include "effect_chorus.h"

extern "C" i32 main(void)
{
    //clear bss.
    extern u8 __bss_start;
    extern u8 _end;
    for(u8* pBSS = &__bss_start; pBSS < &_end; pBSS++)
    {
        *pBSS = 0;
    }

    //call construtors of static objects.
    extern void (*__init_start)(void);
    extern void (*__init_end)(void);
    for(void (**constructor)(void) = &__init_start; constructor < &__init_end; constructor++)
    {
        (**constructor)();
    }

    memory::enable_mmu();
    buddy_heap::initialize();
    mailbox_framebuffer a_mailbox_framebuffer;
    screen a_screen(vector_2_u32(800, 480), reinterpret_cast<color*>(a_mailbox_framebuffer.get_framebuffer()));
    effect_graph a_effect_graph(&a_screen);
    effect_chorus a_effect_chorus(rectangle(vector_2_u32(50,50),vector_2_u32(100,50)),color(127,0,0,255));
    a_effect_graph.add_effect(&a_effect_chorus);
    while(true)
    {
        a_effect_graph.draw();
        a_screen.draw_text("c",vector_2_u32(0,63));
        a_effect_chorus.move(vector_2_u32(0,1));
        a_screen.refresh();
    }
    return 0;
}

//dummy functions to avoid linker complaints.
extern "C" void __cxa_pure_virtual()
{
    while(1)
        ;
}

extern "C" void __aeabi_atexit(void)
{
    while(1)
        ;
}

extern "C" void __dso_handle(void)
{
    while(1)
        ;
}

extern "C" void __cxa_guard_acquire(void)
{
}

extern "C" void __cxa_guard_release(void)
{
}

extern "C" void __cxa_atexit(void)
{
}