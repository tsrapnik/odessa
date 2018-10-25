#pragma once

#include "type_definitions.h"

typedef enum
{
    MEM_FLAG_DISCARDABLE = 1 << 0, /* can be resized to 0 at any time. Use for cached data */
    MEM_FLAG_NORMAL = 0 << 2, /* normal allocating alias. Don't use from ARM			*/
    MEM_FLAG_DIRECT = 1 << 2, /* 0xC alias uncached									*/
    MEM_FLAG_COHERENT = 2 << 2, /* 0x8 alias. Non-allocating in L2 but coherent			*/
    MEM_FLAG_L1_NONALLOCATING = (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT), /* Allocating in L2									*/
    MEM_FLAG_ZERO = 1 << 4, /* initialise buffer to all zeros						*/
    MEM_FLAG_NO_INIT = 1 << 5, /* don't initialise (default is initialise to all ones	*/
    MEM_FLAG_HINT_PERMALOCK = 1 << 6, /* Likely to be locked for long periods of time.		*/
} V3D_MEMALLOC_FLAGS;

typedef u32 gpu_handle;
typedef u32 vc4_address;

typedef struct render_t
{
    /* This is the current load position */
    vc4_address loadpos; // Physical load address as ARM address

    /* These are all the same thing just handle and two different address GPU and ARM */
    gpu_handle rendererHandle; // Renderer memory handle
    vc4_address rendererDataVC4; // Renderer data VC4 locked address

    u16 renderWth; // Render width
    u16 renderHt; // Render height

    vc4_address shaderStart; // VC4 address shader code starts
    vc4_address fragShaderRecStart; // VC4 start address for fragment shader record

    u32 binWth; // Bin width
    u32 binHt; // Bin height

    vc4_address renderControlVC4; // VC4 render control start address
    vc4_address renderControlEndVC4; // VC4 render control end address

    vc4_address vertexVC4; // VC4 address to vertex data
    u32 num_verts; // number of vertices

    vc4_address indexVertexVC4; // VC4 address to start of index vertex data
    u32 IndexVertexCt; // Index vertex count
    u32 MaxIndexVertex; // Maximum Index vertex referenced

    /* TILE DATA MEMORY ... HAS TO BE 4K ALIGN */
    gpu_handle tileHandle; // Tile memory handle
    u32 tileMemSize; // Tiel memory size;
    vc4_address tileStateDataVC4; // Tile data VC4 locked address
    vc4_address tileDataBufferVC4; // Tile data buffer VC4 locked address

    /* BINNING DATA MEMORY ... HAS TO BE 4K ALIGN */
    gpu_handle binningHandle; // Binning memory handle
    vc4_address binningDataVC4; // Binning data VC4 locked address
    vc4_address binningCfgEnd; // VC4 binning config end address

} RENDER_STRUCT;

bool InitV3D(void);

gpu_handle V3D_mem_alloc(u32 size, u32 align, V3D_MEMALLOC_FLAGS flags);

bool V3D_mem_free(gpu_handle handle);

u32 V3D_mem_lock(gpu_handle handle);

bool V3D_mem_unlock(gpu_handle handle);

bool V3D_execute_code(u32 code, u32 r0, u32 r1, u32 r2, u32 r3, u32 r4, u32 r5);
bool V3D_execute_qpu(i32 num_qpus, u32 control, u32 noflush, u32 timeout);

bool V3D_InitializeScene(RENDER_STRUCT* scene, u32 renderWth, u32 renderHt);
bool V3D_AddVertexesToScene(RENDER_STRUCT* scene);
bool V3D_AddShadderToScene(RENDER_STRUCT* scene, u32* frag_shader, u32 frag_shader_emits);
bool V3D_SetupRenderControl(RENDER_STRUCT* scene, vc4_address renderBufferAddr);
bool V3D_SetupBinningConfig(RENDER_STRUCT* scene);

void V3D_RenderScene(RENDER_STRUCT* scene);