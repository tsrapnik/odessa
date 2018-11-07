#pragma once
#include "type_definitions.h"
#include "mailbox_property_tags.h"

typedef enum
{
    MEM_FLAG_DISCARDABLE = 1 << 0, /* can be resized to 0 at any time. Use for cached data */
    MEM_FLAG_NORMAL = 0 << 2, /* normal allocating alias. Don't use from ARM			*/
    MEM_FLAG_DIRECT = 1 << 2, /* 0xC alias uncached									*/
    MEM_FLAG_COHERENT = 2 << 2, /* 0x8 alias. Non-allocating in L2 but coherent			*/
    MEM_FLAG_L1_NONALLOCATING = (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT), /* Allocating in L2									*/
    MEM_FLAG_ZERO = 1 << 4, /* initialise buffer to all zeros						*/
    MEM_FLAG_NO_INIT = 1 << 5, /* don't initialise (default is initialise to all ones	*/
    MEM_FLAG_Hbool_PERMALOCK = 1 << 6, /* Likely to be locked for long periods of time.		*/
} V3D_MEMALLOC_FLAGS;

typedef struct render_t
{
    /* This is the current load position */
    u32 loadpos; // Physical load address as ARM address

    /* These are all the same thing just handle and two different address GPU and ARM */
    u32 rendererHandle; // Renderer memory handle
    u32 rendererDataVC4; // Renderer data VC4 locked address

    u16 renderWth; // Render width
    u16 renderHt; // Render height

    u32 shaderStart; // VC4 address shader code starts
    u32 fragShaderRecStart; // VC4 start address for fragment shader record

    u32 binWth; // Bin width
    u32 binHt; // Bin height

    u32 renderControlVC4; // VC4 render control start address
    u32 renderControlEndVC4; // VC4 render control end address

    u32 vertexVC4; // VC4 address to vertex data
    u32 num_verts; // number of vertices

    u32 indexVertexVC4; // VC4 address to start of index vertex data
    u32 IndexVertexCt; // Index vertex count
    u32 MaxIndexVertex; // Maximum Index vertex referenced

    /* TILE DATA MEMORY ... HAS TO BE 4K ALIGN */
    u32 tileHandle; // Tile memory handle
    u32 tileMemSize; // Tiel memory size;
    u32 tileStateDataVC4; // Tile data VC4 locked address
    u32 tileDataBufferVC4; // Tile data buffer VC4 locked address

    /* BINNING DATA MEMORY ... HAS TO BE 4K ALIGN */
    u32 binningHandle; // Binning memory handle
    u32 binningDataVC4; // Binning data VC4 locked address
    u32 binningCfgEnd; // VC4 binning config end address

} RENDER_STRUCT;

class gpu
{
    private:
    mailbox_property_tags& a_mailbox_property_tags;

    public:
    gpu();
    ~gpu();
    bool V3D_InitializeScene(RENDER_STRUCT* scene, u32 renderWth, u32 renderHt);
    bool V3D_AddVertexesToScene(RENDER_STRUCT* scene);
    bool V3D_AddShadderToScene(RENDER_STRUCT* scene, u32* frag_shader, u32 frag_shader_emits);
    bool V3D_SetupRenderControl(RENDER_STRUCT* scene, u32 renderBufferAddr);
    bool V3D_SetupBinningConfig(RENDER_STRUCT* scene);
    bool V3D_RenderScene(RENDER_STRUCT* scene);
};