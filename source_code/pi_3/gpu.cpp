// #include <stdint.h>		// Needed for int and 1/0
// #include <stdint.h>			// Needed for u8, u32, etc
//#include "rpi-SmartStart.h" // Need for mailbox
#include "gpu.h"
// #include "type_definitions.h"

/* REFERENCES */
/* https://docs.broadcom.com/docs/12358545 */
/* http://latchup.blogspot.com.au/2016/02/life-of-triangle.html */
/* https://cgit.freedesktop.org/mesa/mesa/tree/src/gallium/drivers/vc4/vc4_draw.c */

#define v3d_macro ((volatile __attribute__((aligned(4))) u32*)(usize)(0x3F000000 + 0xc00000))

/* Registers shamelessly copied from Eric AnHolt */

// Defines for v3d register offsets
#define V3D_IDENT0 (0x000 >> 2) // V3D Identification 0 (V3D block identity)
#define V3D_IDENT1 (0x004 >> 2) // V3D Identification 1 (V3D Configuration A)
#define V3D_IDENT2 (0x008 >> 2) // V3D Identification 1 (V3D Configuration B)

#define V3D_SCRATCH (0x010 >> 2) // Scratch Register

#define V3D_L2CACTL (0x020 >> 2) // 2 Cache Control
#define V3D_SLCACTL (0x024 >> 2) // Slices Cache Control

#define V3D_INTCTL (0x030 >> 2) // Interrupt Control
#define V3D_INTENA (0x034 >> 2) // Interrupt Enables
#define V3D_INTDIS (0x038 >> 2) // Interrupt Disables

#define V3D_CT0CS (0x100 >> 2) // Control List Executor Thread 0 Control and Status.
#define V3D_CT1CS (0x104 >> 2) // Control List Executor Thread 1 Control and Status.
#define V3D_CT0EA (0x108 >> 2) // Control List Executor Thread 0 End Address.
#define V3D_CT1EA (0x10c >> 2) // Control List Executor Thread 1 End Address.
#define V3D_CT0CA (0x110 >> 2) // Control List Executor Thread 0 Current Address.
#define V3D_CT1CA (0x114 >> 2) // Control List Executor Thread 1 Current Address.
#define V3D_CT00RA0 (0x118 >> 2) // Control List Executor Thread 0 Return Address.
#define V3D_CT01RA0 (0x11c >> 2) // Control List Executor Thread 1 Return Address.
#define V3D_CT0LC (0x120 >> 2) // Control List Executor Thread 0 List Counter
#define V3D_CT1LC (0x124 >> 2) // Control List Executor Thread 1 List Counter
#define V3D_CT0PC (0x128 >> 2) // Control List Executor Thread 0 Primitive List Counter
#define V3D_CT1PC (0x12c >> 2) // Control List Executor Thread 1 Primitive List Counter

#define V3D_PCS (0x130 >> 2) // V3D Pipeline Control and Status
#define V3D_BFC (0x134 >> 2) // Binning Mode Flush Count
#define V3D_RFC (0x138 >> 2) // Rendering Mode Frame Count

#define V3D_BPCA (0x300 >> 2) // Current Address of Binning Memory Pool
#define V3D_BPCS (0x304 >> 2) // Remaining Size of Binning Memory Pool
#define V3D_BPOA (0x308 >> 2) // Address of Overspill Binning Memory Block
#define V3D_BPOS (0x30c >> 2) // Size of Overspill Binning Memory Block
#define V3D_BXCF (0x310 >> 2) // Binner Debug

#define V3D_SQRSV0 (0x410 >> 2) // Reserve QPUs 0-7
#define V3D_SQRSV1 (0x414 >> 2) // Reserve QPUs 8-15
#define V3D_SQCNTL (0x418 >> 2) // QPU Scheduler Control

#define V3D_SRQPC (0x430 >> 2) // QPU User Program Request Program Address
#define V3D_SRQUA (0x434 >> 2) // QPU User Program Request Uniforms Address
#define V3D_SRQUL (0x438 >> 2) // QPU User Program Request Uniforms Length
#define V3D_SRQCS (0x43c >> 2) // QPU User Program Request Control and Status

#define V3D_VPACNTL (0x500 >> 2) // VPM Allocator Control
#define V3D_VPMBASE (0x504 >> 2) // VPM base (user) memory reservation

#define V3D_PCTRC (0x670 >> 2) // Performance Counter Clear
#define V3D_PCTRE (0x674 >> 2) // Performance Counter Enables

#define V3D_PCTR0 (0x680 >> 2) // Performance Counter Count 0
#define V3D_PCTRS0 (0x684 >> 2) // Performance Counter Mapping 0
#define V3D_PCTR1 (0x688 >> 2) // Performance Counter Count 1
#define V3D_PCTRS1 (0x68c >> 2) // Performance Counter Mapping 1
#define V3D_PCTR2 (0x690 >> 2) // Performance Counter Count 2
#define V3D_PCTRS2 (0x694 >> 2) // Performance Counter Mapping 2
#define V3D_PCTR3 (0x698 >> 2) // Performance Counter Count 3
#define V3D_PCTRS3 (0x69c >> 2) // Performance Counter Mapping 3
#define V3D_PCTR4 (0x6a0 >> 2) // Performance Counter Count 4
#define V3D_PCTRS4 (0x6a4 >> 2) // Performance Counter Mapping 4
#define V3D_PCTR5 (0x6a8 >> 2) // Performance Counter Count 5
#define V3D_PCTRS5 (0x6ac >> 2) // Performance Counter Mapping 5
#define V3D_PCTR6 (0x6b0 >> 2) // Performance Counter Count 6
#define V3D_PCTRS6 (0x6b4 >> 2) // Performance Counter Mapping 6
#define V3D_PCTR7 (0x6b8 >> 2) // Performance Counter Count 7
#define V3D_PCTRS7 (0x6bc >> 2) // Performance Counter Mapping 7
#define V3D_PCTR8 (0x6c0 >> 2) // Performance Counter Count 8
#define V3D_PCTRS8 (0x6c4 >> 2) // Performance Counter Mapping 8
#define V3D_PCTR9 (0x6c8 >> 2) // Performance Counter Count 9
#define V3D_PCTRS9 (0x6cc >> 2) // Performance Counter Mapping 9
#define V3D_PCTR10 (0x6d0 >> 2) // Performance Counter Count 10
#define V3D_PCTRS10 (0x6d4 >> 2) // Performance Counter Mapping 10
#define V3D_PCTR11 (0x6d8 >> 2) // Performance Counter Count 11
#define V3D_PCTRS11 (0x6dc >> 2) // Performance Counter Mapping 11
#define V3D_PCTR12 (0x6e0 >> 2) // Performance Counter Count 12
#define V3D_PCTRS12 (0x6e4 >> 2) // Performance Counter Mapping 12
#define V3D_PCTR13 (0x6e8 >> 2) // Performance Counter Count 13
#define V3D_PCTRS13 (0x6ec >> 2) // Performance Counter Mapping 13
#define V3D_PCTR14 (0x6f0 >> 2) // Performance Counter Count 14
#define V3D_PCTRS14 (0x6f4 >> 2) // Performance Counter Mapping 14
#define V3D_PCTR15 (0x6f8 >> 2) // Performance Counter Count 15
#define V3D_PCTRS15 (0x6fc >> 2) // Performance Counter Mapping 15

#define V3D_DBGE (0xf00 >> 2) // PSE Error Signals
#define V3D_FDBGO (0xf04 >> 2) // FEP Overrun Error Signals
#define V3D_FDBGB (0xf08 >> 2) // FEP Interface Ready and Stall Signals, FEP Busy Signals
#define V3D_FDBGR (0xf0c >> 2) // FEP Internal Ready Signals
#define V3D_FDBGS (0xf10 >> 2) // FEP Internal Stall Input Signals

#define V3D_ERRSTAT (0xf20 >> 2) // Miscellaneous Error Signals (VPM, VDW, VCD, VCM, L2C)

#define ALIGN_128BIT_MASK 0xFFFFFF80

typedef enum
{
    PRIM_POINT = 0,
    PRIM_LINE = 1,
    PRIM_LINE_LOOP = 2,
    PRIM_LINE_STRIP = 3,
    PRIM_TRIANGLE = 4,
    PRIM_TRIANGLE_STRIP = 5,
    PRIM_TRIANGLE_FAN = 6,
} PRIMITIVE;

typedef enum
{
    GL_HALT = 0,
    GL_NOP = 1,
    GL_FLUSH = 4,
    GL_FLUSH_ALL_STATE = 5,
    GL_START_TILE_BINNING = 6,
    GL_INCREMENT_SEMAPHORE = 7,
    GL_WAIT_ON_SEMAPHORE = 8,
    GL_BRANCH = 16,
    GL_BRANCH_TO_SUBLIST = 17,
    GL_RETURN_FROM_SUBLIST = 18,
    GL_STORE_MULTISAMPLE = 24,
    GL_STORE_MULTISAMPLE_END = 25,
    GL_STORE_FULL_TILE_BUFFER = 26,
    GL_RELOAD_FULL_TILE_BUFFER = 27,
    GL_STORE_TILE_BUFFER = 28,
    GL_LOAD_TILE_BUFFER = 29,
    GL_INDEXED_PRIMITIVE_LIST = 32,
    GL_VERTEX_ARRAY_PRIMITIVES = 33,
    GL_VG_COORDINATE_ARRAY_PRIMITIVES = 41,
    GL_COMPRESSED_PRIMITIVE_LIST = 48,
    GL_CLIP_COMPRESSD_PRIMITIVE_LIST = 49,
    GL_PRIMITIVE_LIST_FORMAT = 56,
    GL_SHADER_STATE = 64,
    GL_NV_SHADER_STATE = 65,
    GL_VG_SHADER_STATE = 66,
    GL_VG_INLINE_SHADER_RECORD = 67,
    GL_CONFIG_STATE = 96,
    GL_FLAT_SHADE_FLAGS = 97,
    GL_POINTS_SIZE = 98,
    GL_LINE_WIDTH = 99,
    GL_RHT_X_BOUNDARY = 100,
    GL_DEPTH_OFFSET = 101,
    GL_CLIP_WINDOW = 102,
    GL_VIEWPORT_OFFSET = 103,
    GL_Z_CLIPPING_PLANES = 104,
    GL_CLIPPER_XY_SCALING = 105,
    GL_CLIPPER_Z_ZSCALE_OFFSET = 106,
    GL_TILE_BINNING_CONFIG = 112,
    GL_TILE_RENDER_CONFIG = 113,
    GL_CLEAR_COLORS = 114,
    GL_TILE_COORDINATES = 115
} GL_CONTROL;

struct __attribute__((__packed__, aligned(1))) EMITDATA
{
    u8 byte1;
    u8 byte2;
    u8 byte3;
    u8 byte4;
};

gpu::gpu() :
    a_mailbox_property_tags(mailbox_property_tags::get_handle())
{
    a_mailbox_property_tags.set_clock_rate(mailbox_property_tags::clock_id::v3d, 250000000);
    a_mailbox_property_tags.enable_qpu(true);
    //todo: exception if fails.
}

gpu::~gpu()
{
    //todo: add release, unlock and disable functions.
}

static void emit_u8(u8** list, u8 d)
{
    *((*list)++) = d;
}

static void emit_u16(u8** list, u16 d)
{
    struct EMITDATA* data = (struct EMITDATA*)&d;
    *((*list)++) = (*data).byte1;
    *((*list)++) = (*data).byte2;
}

static void emit_u32(u8** list, u32 d)
{
    struct EMITDATA* data = (struct EMITDATA*)&d;
    *((*list)++) = (*data).byte1;
    *((*list)++) = (*data).byte2;
    *((*list)++) = (*data).byte3;
    *((*list)++) = (*data).byte4;
}

static void emit_float(u8** list, float f)
{
    struct EMITDATA* data = (struct EMITDATA*)&f;
    *((*list)++) = (*data).byte1;
    *((*list)++) = (*data).byte2;
    *((*list)++) = (*data).byte3;
    *((*list)++) = (*data).byte4;
}

void gpu::V3D_InitializeScene(u32 renderWth, u32 renderHt)
{
        a_render_struct.rendererHandle = a_mailbox_property_tags.allocate_memory(0x10000,
                                                                        0x1000,
                                                                        mailbox_property_tags::allocate_memory_flag::coherent &
                                                                            mailbox_property_tags::allocate_memory_flag::zero);
        a_render_struct.rendererDataVC4 = a_mailbox_property_tags.lock_memory(a_render_struct.rendererHandle);
        a_render_struct.loadpos = a_render_struct.rendererDataVC4; // VC4 load from start of memory

        a_render_struct.renderWth = renderWth; // Render width
        a_render_struct.renderHt = renderHt; // Render height
        a_render_struct.binWth = (renderWth + 63) / 64; // Tiles across
        a_render_struct.binHt = (renderHt + 63) / 64; // Tiles down

        a_render_struct.tileMemSize = 0x4000;
        a_render_struct.tileHandle = a_mailbox_property_tags.allocate_memory(a_render_struct.tileMemSize + 0x4000,
                                                                    0x1000,
                                                                    mailbox_property_tags::allocate_memory_flag::coherent &
                                                                        mailbox_property_tags::allocate_memory_flag::zero);
        a_render_struct.tileStateDataVC4 = a_mailbox_property_tags.lock_memory(a_render_struct.tileHandle);
        a_render_struct.tileDataBufferVC4 = a_render_struct.tileStateDataVC4 + 0x4000;

        a_render_struct.binningHandle = a_mailbox_property_tags.allocate_memory(0x10000,
                                                                       0x1000,
                                                                       mailbox_property_tags::allocate_memory_flag::coherent &
                                                                           mailbox_property_tags::allocate_memory_flag::zero);
        a_render_struct.binningDataVC4 = a_mailbox_property_tags.lock_memory(a_render_struct.binningHandle);
}

u32 GPUaddrToARMaddr2(u32 GPUaddress)
{
    return GPUaddress & ~0xc0000000;
}

void gpu::V3D_AddVertexesToScene()
{
        a_render_struct.vertexVC4 = (a_render_struct.loadpos + 127) & ALIGN_128BIT_MASK; // Hold vertex start adderss .. aligned to 128bits
        u8* p = static_cast<u8*>(mailbox::translate_vc_to_arm(a_render_struct.vertexVC4));
        u8* q = p;

        /* Setup triangle vertices from OpenGL tutorial which used this */
        // fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
        // fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
        // fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;
        u32 centreX = a_render_struct.renderWth / 2; // triangle centre x
        u32 centreY = (u32)(0.4f * (a_render_struct.renderHt / 2)); // triangle centre y
        u32 half_shape_wth = (u32)(0.4f * (a_render_struct.renderWth / 2)); // Half width of triangle
        u32 half_shape_ht = (u32)(0.3f * (a_render_struct.renderHt / 2)); // half height of tringle

        // Vertex Data

        // Vertex: Top, vary red
        emit_u16(&p, (centreX) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY - half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 0.0f); // Varying 0 (Red)
        emit_float(&p, 0.0f); // Varying 1 (Green)
        emit_float(&p, 1.0f); // Varying 2 (Blue)

        // Vertex: bottom left, vary blue
        emit_u16(&p, (centreX - half_shape_wth) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY + half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 0.0f); // Varying 0 (Red)
        emit_float(&p, 0.0f); // Varying 1 (Green)
        emit_float(&p, 1.0f); // Varying 2 (Blue)

        // Vertex: bottom right, vary green
        emit_u16(&p, (centreX + half_shape_wth) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY + half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 0.0f); // Varying 0 (Red)
        emit_float(&p, 1.0f); // Varying 1 (Green)
        emit_float(&p, 0.0f); // Varying 2 (Blue)

        /* Setup triangle vertices from OpenGL tutorial which used this */
        // fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
        // fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
        // fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
        // fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;
        centreY = (u32)(1.35f * (a_render_struct.renderHt / 2)); // quad centre y

        // Vertex: Top, left  vary blue
        emit_u16(&p, (centreX - half_shape_wth) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY - half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 0.0f); // Varying 0 (Red)
        emit_float(&p, 0.0f); // Varying 1 (Green)
        emit_float(&p, 1.0f); // Varying 2 (Blue)

        // Vertex: bottom left, vary Green
        emit_u16(&p, (centreX - half_shape_wth) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY + half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 0.0f); // Varying 0 (Red)
        emit_float(&p, 1.0f); // Varying 1 (Green)
        emit_float(&p, 0.0f); // Varying 2 (Blue)

        // Vertex: top right, vary red
        emit_u16(&p, (centreX + half_shape_wth) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY - half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 1.0f); // Varying 0 (Red)
        emit_float(&p, 0.0f); // Varying 1 (Green)
        emit_float(&p, 0.0f); // Varying 2 (Blue)

        // Vertex: bottom right, vary yellow
        emit_u16(&p, (centreX + half_shape_wth) << 4); // X in 12.4 fixed point
        emit_u16(&p, (centreY + half_shape_ht) << 4); // Y in 12.4 fixed point
        emit_float(&p, 1.0f); // Z
        emit_float(&p, 1.0f); // 1/W
        emit_float(&p, 0.0f); // Varying 0 (Red)
        emit_float(&p, 1.0f); // Varying 1 (Green)
        emit_float(&p, 1.0f); // Varying 2 (Blue)

        a_render_struct.num_verts = 7;
        a_render_struct.loadpos = a_render_struct.vertexVC4 + (p - q); // Update load position

        a_render_struct.indexVertexVC4 = (a_render_struct.loadpos + 127) & ALIGN_128BIT_MASK; // Hold index vertex start adderss .. align it to 128 bits
        p = (u8*)(usize)GPUaddrToARMaddr2(a_render_struct.indexVertexVC4);
        q = p;

        emit_u8(&p, 0); // tri - top
        emit_u8(&p, 1); // tri - bottom left
        emit_u8(&p, 2); // tri - bottom right

        emit_u8(&p, 3); // quad - top left
        emit_u8(&p, 4); // quad - bottom left
        emit_u8(&p, 5); // quad - top right

        emit_u8(&p, 4); // quad - bottom left
        emit_u8(&p, 6); // quad - bottom right
        emit_u8(&p, 5); // quad - top right
        a_render_struct.IndexVertexCt = 9;
        a_render_struct.MaxIndexVertex = 6;

        a_render_struct.loadpos = a_render_struct.indexVertexVC4 + (p - q); // Move loaad pos to new position
}

void gpu::V3D_AddShadderToScene(u32* frag_shader, u32 frag_shader_emits)
{
        a_render_struct.shaderStart = (a_render_struct.loadpos + 127) & ALIGN_128BIT_MASK; // Hold shader start adderss .. aligned to 127 bits
        u8* p = (u8*)(usize)GPUaddrToARMaddr2(a_render_struct.shaderStart); // ARM address for load
        u8* q = p; // Hold start address

        for(u32 i = 0; i < frag_shader_emits; i++) // For the number of fragment shader emits
            emit_u32(&p, frag_shader[i]); // Emit fragment shader into our allocated memory

        a_render_struct.loadpos = a_render_struct.shaderStart + (p - q); // Update load position

        a_render_struct.fragShaderRecStart = (a_render_struct.loadpos + 127) & ALIGN_128BIT_MASK; // Hold frag shader start adderss .. .aligned to 128bits
        p = (u8*)(usize)GPUaddrToARMaddr2(a_render_struct.fragShaderRecStart);
        q = p;

        // Okay now we need Shader Record to buffer
        emit_u8(&p, 0x01); // flags
        emit_u8(&p, 6 * 4); // stride
        emit_u8(&p, 0xcc); // num uniforms (not used)
        emit_u8(&p, 3); // num varyings
        emit_u32(&p, a_render_struct.shaderStart); // Shader code address
        emit_u32(&p, 0); // Fragment shader uniforms (not in use)
        emit_u32(&p, a_render_struct.vertexVC4); // Vertex Data

        a_render_struct.loadpos = a_render_struct.fragShaderRecStart + (p - q); // Adjust VC4 load poistion
}

void gpu::V3D_SetupRenderControl(u32 renderBufferAddr)
{
        a_render_struct.renderControlVC4 = (a_render_struct.loadpos + 127) & ALIGN_128BIT_MASK; // Hold render control start adderss .. aligned to 128 bits
        u8* p = (u8*)(usize)GPUaddrToARMaddr2(a_render_struct.renderControlVC4); // ARM address for load
        u8* q = p; // Hold start address

        // Clear colors
        emit_u8(&p, GL_CLEAR_COLORS);
        emit_u32(&p, 0xff000000); // Opaque Black
        emit_u32(&p, 0xff000000); // 32 bit clear colours need to be repeated twice
        emit_u32(&p, 0);
        emit_u8(&p, 0);

        // Tile Rendering Mode Configuration
        emit_u8(&p, GL_TILE_RENDER_CONFIG);

        emit_u32(&p, renderBufferAddr); // render address (will be framebuffer)

        emit_u16(&p, a_render_struct.renderWth); // render width
        emit_u16(&p, a_render_struct.renderHt); // render height
        emit_u8(&p, 0x04); // framebuffer mode (linear rgba8888)
        emit_u8(&p, 0x00);

        // Do a store of the first tile to force the tile buffer to be cleared
        // Tile Coordinates
        emit_u8(&p, GL_TILE_COORDINATES);
        emit_u8(&p, 0);
        emit_u8(&p, 0);

        // Store Tile Buffer General
        emit_u8(&p, GL_STORE_TILE_BUFFER);
        emit_u16(&p, 0); // Store nothing (just clear)
        emit_u32(&p, 0); // no address is needed

        // Link all binned lists together
        for(u32 x = 0; x < a_render_struct.binWth; x++)
        {
            for(u32 y = 0; y < a_render_struct.binHt; y++)
            {

                // Tile Coordinates
                emit_u8(&p, GL_TILE_COORDINATES);
                emit_u8(&p, x);
                emit_u8(&p, y);

                // Call Tile sublist
                emit_u8(&p, GL_BRANCH_TO_SUBLIST);
                emit_u32(&p, a_render_struct.tileDataBufferVC4 + (y * a_render_struct.binWth + x) * 32);

                // Last tile needs a special store instruction
                if(x == (a_render_struct.binWth - 1) && (y == a_render_struct.binHt - 1))
                {
                    // Store resolved tile color buffer and signal end of frame
                    emit_u8(&p, GL_STORE_MULTISAMPLE_END);
                }
                else
                {
                    // Store resolved tile color buffer
                    emit_u8(&p, GL_STORE_MULTISAMPLE);
                }
            }
        }

        a_render_struct.loadpos = a_render_struct.renderControlVC4 + (p - q); // Adjust VC4 load poistion
        a_render_struct.renderControlEndVC4 = a_render_struct.loadpos; // Hold end of render control data
}

void gpu::V3D_SetupBinningConfig()
{
        u8* p = (u8*)(usize)GPUaddrToARMaddr2(a_render_struct.binningDataVC4); // ARM address for binning data load
        u8* list = p; // Hold start address

        emit_u8(&p, GL_TILE_BINNING_CONFIG); // tile binning config control
        emit_u32(&p, a_render_struct.tileDataBufferVC4); // tile allocation memory address
        emit_u32(&p, a_render_struct.tileMemSize); // tile allocation memory size
        emit_u32(&p, a_render_struct.tileStateDataVC4); // Tile state data address
        emit_u8(&p, a_render_struct.binWth); // renderWidth/64
        emit_u8(&p, a_render_struct.binHt); // renderHt/64
        emit_u8(&p, 0x04); // config

        // Start tile binning.
        emit_u8(&p, GL_START_TILE_BINNING); // Start binning command

        // Primitive type
        emit_u8(&p, GL_PRIMITIVE_LIST_FORMAT);
        emit_u8(&p, 0x32); // 16 bit triangle

        // Clip Window
        emit_u8(&p, GL_CLIP_WINDOW); // Clip window
        emit_u16(&p, 0); // 0
        emit_u16(&p, 0); // 0
        emit_u16(&p, a_render_struct.renderWth); // width
        emit_u16(&p, a_render_struct.renderHt); // height

        // GL State
        emit_u8(&p, GL_CONFIG_STATE);
        emit_u8(&p, 0x03); // enable both foward and back facing polygons
        emit_u8(&p, 0x00); // depth testing disabled
        emit_u8(&p, 0x02); // enable early depth write

        // Viewport offset
        emit_u8(&p, GL_VIEWPORT_OFFSET); // Viewport offset
        emit_u16(&p, 0); // 0
        emit_u16(&p, 0); // 0

        // The triangle
        // No Vertex Shader state (takes pre-transformed vertexes so we don't have to supply a working coordinate shader.)
        emit_u8(&p, GL_NV_SHADER_STATE);
        emit_u32(&p, a_render_struct.fragShaderRecStart); // Shader Record

        // primitive index list
        emit_u8(&p, GL_INDEXED_PRIMITIVE_LIST); // Indexed primitive list command
        emit_u8(&p, PRIM_TRIANGLE); // 8bit index, triangles
        emit_u32(&p, a_render_struct.IndexVertexCt); // Number of index vertex
        emit_u32(&p, a_render_struct.indexVertexVC4); // Address of index vertex data
        emit_u32(&p, a_render_struct.MaxIndexVertex); // Maximum index

        // End of bin list
        // So Flush
        emit_u8(&p, GL_FLUSH_ALL_STATE);
        // Nop
        emit_u8(&p, GL_NOP);
        // Halt
        emit_u8(&p, GL_HALT);
        a_render_struct.binningCfgEnd = a_render_struct.binningDataVC4 + (p - list); // Hold binning data end address
}

void gpu::V3D_RenderScene()
{
        // clear caches
        v3d_macro[V3D_L2CACTL] = 4;
        v3d_macro[V3D_SLCACTL] = 0x0F0F0F0F;

        // stop the thread
        v3d_macro[V3D_CT0CS] = 0x20;
        // wait for it to stop
        while(v3d_macro[V3D_CT0CS] & 0x20)
            ;

        // Run our control list
        v3d_macro[V3D_BFC] = 1; // reset binning frame count
        v3d_macro[V3D_CT0CA] = a_render_struct.binningDataVC4; // Start binning config address
        v3d_macro[V3D_CT0EA] = a_render_struct.binningCfgEnd; // End binning config address is at render control start

        // wait for binning to finish
        while(v3d_macro[V3D_BFC] == 0)
        {
        }

        // stop the thread
        v3d_macro[V3D_CT1CS] = 0x20;

        // Wait for thread to stop
        while(v3d_macro[V3D_CT1CS] & 0x20)
            ;

        // Run our render
        v3d_macro[V3D_RFC] = 1; // reset rendering frame count
        v3d_macro[V3D_CT1CA] = a_render_struct.renderControlVC4; // Start address for render control
        v3d_macro[V3D_CT1EA] = a_render_struct.renderControlEndVC4; // End address for render control

        // wait for render to finish
        while(v3d_macro[V3D_RFC] == 0)
        {
        }
}