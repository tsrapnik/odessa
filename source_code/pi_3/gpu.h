#pragma once
#include "mailbox_property_tags.h"
#include "type_definitions.h"

#include "screen.h" //todo: remove.
#include "string.h"

class gpu
{
    public:
    //in screen coordinates.
    struct vertex
    {
        u16 x; //in 12.4 fixed point format.
        u16 y; //in 12.4 fixed point format.
        f32 z;
        f32 w;
        f32 r;
        f32 g;
        f32 b;
    } __attribute__((packed));

    struct triangle
    {
        u8 index_0;
        u8 index_1;
        u8 index_2;
    } __attribute__((packed));

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
    u32 vertices; // VC4 address to vertex data
    u32 vertices_size; // number of vertices
    u32 triangles; // VC4 address to start of index vertex data
    u32 triangles_size; // Index vertex count
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
    screen* a_screen; //todo: remove.
    gpu();
    ~gpu();
    void V3D_InitializeScene(u16 renderWth, u16 renderHt);
    void V3D_AddVertexesToScene(vertex* vertices, triangle* triangles, u32 vertices_size, u32 triangles_size);
    void update_vertices(vertex* new_vertices, triangle* new_triangles);
    void V3D_AddShadderToScene();
    void V3D_SetupRenderControl(u32 renderBufferAddr);
    void V3D_SetupBinningConfig();
    void V3D_RenderScene();
    void testTriangle (u16 renderWth, u16 renderHt, u32 renderBufferAddr, vertex* new_vertices, triangle* new_triangles, u32 new_vertices_size, u32 new_triangles_size);
};