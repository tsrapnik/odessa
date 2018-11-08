#pragma once
#include "mailbox_property_tags.h"
#include "type_definitions.h"

#include "screen.h" //todo: remove.
#include "string.h"

class gpu
{
    private:
    u32 loadpos;
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

    mailbox_property_tags& a_mailbox_property_tags;

    public:
    screen* a_screen;
    gpu();
    ~gpu();
    void V3D_InitializeScene(u16 renderWth, u16 renderHt);
    void V3D_AddVertexesToScene();
    void V3D_AddShadderToScene(u32* frag_shader, u32 frag_shader_emits);
    void V3D_SetupRenderControl(u32 renderBufferAddr);
    void V3D_SetupBinningConfig();
    void V3D_RenderScene();
};