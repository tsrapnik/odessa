#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_graph.h"
#include "mailbox_framebuffer.h"
#include "memory.h"
#include "rpi-GLES.h"
#include "screen.h"

static u32 shader1[18] = {
    // Vertex Color Shader
    0x958e0dbf,
    0xd1724823, /* mov r0, vary; mov r3.8d, 1.0 */
    0x818e7176,
    0x40024821, /* fadd r0, r0, r5; mov r1, vary */
    0x818e7376,
    0x10024862, /* fadd r1, r1, r5; mov r2, vary */
    0x819e7540,
    0x114248a3, /* fadd r2, r2, r5; mov r3.8a, r0 */
    0x809e7009,
    0x115049e3, /* nop; mov r3.8b, r1 */
    0x809e7012,
    0x116049e3, /* nop; mov r3.8c, r2 */
    0x159e76c0,
    0x30020ba7, /* mov tlbc, r3; nop; thrend */
    0x009e7000,
    0x100009e7, /* nop; nop; nop */
    0x009e7000,
    0x500009e7, /* nop; nop; sbdone */
};

// static u32 shader2[12] = {
//     // Fill Color Shader
//     0x009E7000,
//     0x100009E7, // nop; nop; nop
//     0xFFFFFFFF,
//     0xE0020BA7, // ldi tlbc, RGBA White
//     0x009E7000,
//     0x500009E7, // nop; nop; sbdone
//     0x009E7000,
//     0x300009E7, // nop; nop; thrend
//     0x009E7000,
//     0x100009E7, // nop; nop; nop
//     0x009E7000,
//     0x100009E7, // nop; nop; nop
// };

static RENDER_STRUCT scene = {0};

extern "C" i32 main(void)
{
    //clear bss.
    // extern u8 __bss_start;
    // extern u8 __bss_end;
    // for(u8* pBSS = &__bss_start; pBSS < &__bss_end; pBSS++)
    // {
    //     *pBSS = 0;
    // }

    // //call construtors of static objects.
    // extern void (*__init_start)(void);
    // extern void (*__init_end)(void);
    // for(void (**constructor)(void) = &__init_start; constructor < &__init_end; constructor++)
    // {
    //     (**constructor)();
    // }

    // memory::enable_mmu();
    buddy_heap::initialize();
    mailbox_framebuffer a_mailbox_framebuffer;
    color* framebuffer = a_mailbox_framebuffer.get_framebuffer();
    screen a_screen(vector_2_u32(800, 480), framebuffer);
    effect_graph a_effect_graph(&a_screen);
    effect_chorus a_effect_chorus(rectangle(vector_2_u32(50,50),vector_2_u32(100,50)),color(127,0,0,255));
    a_effect_graph.add_effect(&a_effect_chorus);

    gpu a_gpu;
    a_gpu.InitV3D();
    a_gpu.V3D_InitializeScene(&scene, 800, 480);
    a_gpu.V3D_AddVertexesToScene(&scene);
    a_gpu.V3D_AddShadderToScene(&scene, &shader1[0], 18);
    a_gpu.V3D_SetupRenderControl(&scene, static_cast<u32>(reinterpret_cast<u64>(a_mailbox_framebuffer.get_framebuffer())));
    a_gpu.V3D_SetupBinningConfig(&scene);
    a_gpu.V3D_RenderScene(&scene);

    while(true)
    {
        // for(int i = 0; i < 100; i++)
        //     for(int j = 0; j < 100; j++)
        //         framebuffer[j * 800 + i] = color(255,255,255,255);
        // a_effect_graph.draw();
        a_screen.draw_text("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",vector_2_u32(0,63));
        a_effect_chorus.move(vector_2_u32(0,5));
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