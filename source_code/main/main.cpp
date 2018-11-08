#include "buddy_heap.h"
#include "effect_chorus.h"
#include "effect_graph.h"
#include "gpu.h"
#include "mailbox_framebuffer.h"
#include "memory.h"
#include "screen.h"
#include "string.h"

//todo: all mailbox and hardware accessing classes should use struct of volatile enum.
//todo: figure out why use of d0 registers lets gpu fail and seperate memory assignment for triangle indices gives strange black corner artifact.

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

// static RENDER_STRUCT scene = {0};
u32 GPUaddrToARMaddr3(u32 GPUaddress)
{
    return GPUaddress & ~0xc0000000;
}
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
    effect_chorus a_effect_chorus(rectangle(vector_2_u32(50, 50), vector_2_u32(100, 50)), color(127, 0, 0, 255));
    a_effect_graph.add_effect(&a_effect_chorus);

    gpu a_gpu;
    a_gpu.a_screen = &a_screen;
    a_gpu.V3D_InitializeScene(800, 480);

    u32 vertex_buffer_size = 4;
    gpu::vertex vertex_buffer[vertex_buffer_size];
    vertex_buffer[0].x = (0) << 4;
    vertex_buffer[0].y = (0) << 4;
    vertex_buffer[0].z = 1.0;
    vertex_buffer[0].w = 1.0;
    vertex_buffer[0].r = 1.0;
    vertex_buffer[0].g = 1.0;
    vertex_buffer[0].b = 1.0;

    vertex_buffer[1].x = (200) << 4;
    vertex_buffer[1].y = (0) << 4;
    vertex_buffer[1].z = 1.0;
    vertex_buffer[1].w = 1.0;
    vertex_buffer[1].r = 1.0;
    vertex_buffer[1].g = 1.0;
    vertex_buffer[1].b = 1.0;

    vertex_buffer[2].x = (0) << 4;
    vertex_buffer[2].y = (200) << 4;
    vertex_buffer[2].z = 1.0;
    vertex_buffer[2].w = 1.0;
    vertex_buffer[2].r = 1.0;
    vertex_buffer[2].g = 1.0;
    vertex_buffer[2].b = 1.0;

    vertex_buffer[3].x = (200) << 4;
    vertex_buffer[3].y = (200) << 4;
    vertex_buffer[3].z = 1.0;
    vertex_buffer[3].w = 1.0;
    vertex_buffer[3].r = 1.0;
    vertex_buffer[3].g = 1.0;
    vertex_buffer[3].b = 1.0;

    u32 triangle_buffer_size = 2;
    gpu::triangle triangle_buffer[triangle_buffer_size];
    triangle_buffer[0].index_0 = 0;
    triangle_buffer[0].index_1 = 1;
    triangle_buffer[0].index_2 = 2;

    triangle_buffer[1].index_0 = 0;
    triangle_buffer[1].index_1 = 1;
    triangle_buffer[1].index_2 = 3;

    a_gpu.V3D_AddVertexesToScene(vertex_buffer, triangle_buffer, vertex_buffer_size, triangle_buffer_size);
    a_gpu.V3D_AddShadderToScene(&shader1[0], 18);
    a_gpu.V3D_SetupRenderControl(static_cast<u32>(reinterpret_cast<u64>(a_mailbox_framebuffer.get_framebuffer())));
    a_gpu.V3D_SetupBinningConfig();

    while(true)
    {
        a_gpu.update_vertices(vertex_buffer, triangle_buffer);
        a_gpu.V3D_RenderScene();
        vertex_buffer[3].x += 100 << 4;
        // a_screen.draw_text("works.", vector_2_u32(0, 0));
        // for(int i = 0; i < 100; i++)
        //     for(int j = 0; j < 100; j++)
        //         framebuffer[j * 800 + i] = color(255,255,255,255);
        // a_effect_graph.draw();
        // a_screen.draw_text("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",vector_2_u32(0,63));
        // a_effect_chorus.move(vector_2_u32(0,5));
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

// extern "C" void memcpy(void* destination, void* source, usize size)
// {
// u32 size_u64 = size / 8;
// u32 size_u8 = size % 8;

// u64* destination_u64 = reinterpret_cast<u64*>(destination);
// u64* source_u64 = reinterpret_cast<u64*>(source);
// for(u32 i = 0; i < size_u64; i++)
// {
//     *destination_u64 = *source_u64;
//     destination_u64++;
//     source_u64++;
// }

// u8* destination_u8 = reinterpret_cast<u8*>(destination_u64);
// u8* source_u8 = reinterpret_cast<u8*>(source_u64);
// for(u32 i = 0; i < size_u8; i++)
// {
//      *destination_u8 = *source_u8;
//      destination_u8++;
//      source_u8++;
// }
// u8* destination_u8 = reinterpret_cast<u8*>(destination);
// u8* source_u8 = reinterpret_cast<u8*>(source);
// for(u32 i = 0; i < size; i++)
// {
//      *destination_u8 = *source_u8;
//      destination_u8++;
//      source_u8++;
// }
// }