#pragma once

#include "type_definitions.h"
#include "vc_pointer.h"
#include "color.h"
#include "list.h"

class vc_gpu
{
  private:
    //todo: make enums from the u32's.
    struct v3d_registers
    {
        volatile u32 ident0;           //0x00
        volatile u32 ident1;           //0x04
        volatile u32 ident2;           //0x08
        u32 reserved_0x0c;             //0x0c
        volatile u32 scratch;          //0x10
        u32 reserved_0x14_0x1c[3];     //0x14-0x1c
        volatile u32 l2cactl;          //0x20
        volatile u32 slcactl;          //0x24
        u32 reserved_0x28_0x2c[2];     //0x28-0x2c
        volatile u32 intctl;           //0x30
        volatile u32 intena;           //0x34
        volatile u32 intdis;           //0x38
        u32 reserved_0x3c_0xfc[49];    //0x3c-0xfc
        volatile u32 ct0cs;            //0x100
        volatile u32 ct1cs;            //0x104
        volatile u32 ct0ea;            //0x108
        volatile u32 ct1ea;            //0x10c
        volatile u32 ct0ca;            //0x110
        volatile u32 ct1ca;            //0x114
        volatile u32 ct00ra0;          //0x118
        volatile u32 ct01ra0;          //0x11c
        volatile u32 ct0lc;            //0x120
        volatile u32 ct1lc;            //0x124
        volatile u32 ct0pc;            //0x128
        volatile u32 ct1pc;            //0x12c
        volatile u32 pcs;              //0x130
        volatile u32 bfc;              //0x134
        volatile u32 rfc;              //0x138
        u32 reserved_0x13c_0x2fc[113]; //0x13c-0x2fc
        volatile u32 bpca;             //0x300
        volatile u32 bpcs;             //0x304
        volatile u32 bpoa;             //0x308
        volatile u32 bpos;             //0x30c
        volatile u32 bxcf;             //0x310
        u32 reserved_0x314_0x40c[63];  //0x314-0x40c
        volatile u32 sqrsv0;           //0x410
        volatile u32 sqrsv1;           //0x414
        volatile u32 sqcntl;           //0x418
        u32 reserved_0x41c_42c[5];     //0x41c-0x42c
        volatile u32 srqpc;            //0x430
        volatile u32 srqua;            //0x434
        volatile u32 srqul;            //0x438
        volatile u32 srqcs;            //0x43c
        u32 reserved_0x440_0x4fc[48];  //0x440-0x4fc
        volatile u32 vpacntl;          //0x500
        volatile u32 vpmbase;          //0x504
        u32 reserved_0x508_0x66c[90];  //0x508-0x66c
        volatile u32 pctrc;            //0x670
        volatile u32 pctre;            //0x674
        u32 reserved_0x678_0x67c[2];   //0x678-0x67c
        volatile u32 pctr0;            //0x680
        volatile u32 pctrs0;           //0x684
        volatile u32 pctr1;            //0x688
        volatile u32 pctrs1;           //0x68c
        volatile u32 pctr2;            //0x690
        volatile u32 pctrs2;           //0x694
        volatile u32 pctr3;            //0x698
        volatile u32 pctrs3;           //0x69c
        volatile u32 pctr4;            //0x6a0
        volatile u32 pctrs4;           //0x6a4
        volatile u32 pctr5;            //0x6a8
        volatile u32 pctrs5;           //0x6ac
        volatile u32 pctr6;            //0x6b0
        volatile u32 pctrs6;           //0x6b4
        volatile u32 pctr7;            //0x6b8
        volatile u32 pctrs7;           //0x6bc
        volatile u32 pctr8;            //0x6c0
        volatile u32 pctrs8;           //0x6c4
        volatile u32 pctr9;            //0x6c8
        volatile u32 pctrs9;           //0x6cc
        volatile u32 pctr10;           //0x6d0
        volatile u32 pctrs10;          //0x6d4
        volatile u32 pctr11;           //0x6d8
        volatile u32 pctrs11;          //0x6dc
        volatile u32 pctr12;           //0x6e0
        volatile u32 pctrs12;          //0x6e4
        volatile u32 pctr13;           //0x6e8
        volatile u32 pctrs13;          //0x6ec
        volatile u32 pctr14;           //0x6f0
        volatile u32 pctrs14;          //0x6f4
        volatile u32 pctr15;           //0x6f8
        volatile u32 pctrs15;          //0x6fc
        u32 reserved_0x700_0xefc[512]; //0x700-0xefc
        volatile u32 dbge;             //0xf00
        volatile u32 fdbgo;            //0xf04
        volatile u32 fdbgb;            //0xf08
        volatile u32 fdbgr;            //0xf0c
        volatile u32 fdbgs;            //0xf10
        u32 reserved_0xf14_0xf1c[3];   //0xf14-0xf1c
        volatile u32 errstat;          //0xf20
    } __attribute__((packed));

    static v3d_registers *const v3d;

    //id of each control data record is forced to be set at a fixed value, by using an enum with only one enumerator.

    struct halt
    {
        enum class id : u8
        {
            id = 0,
        } a_id;
    } __attribute__((packed));

    struct nop
    {
        enum class id : u8
        {
            id = 1,
        } a_id;
    } __attribute__((packed));

    struct flush_all_state
    {
        enum class id : u8
        {
            id = 5,
        } a_id;
    } __attribute__((packed));

    struct start_tile_binning
    {
        enum class id : u8
        {
            id = 6,
        } a_id;
    } __attribute__((packed));

    struct branch_to_sub_list
    {
        enum class id : u8
        {
            id = 17,
        } a_id;
        vc_pointer absolute_branch_address;
    } __attribute__((packed));

    struct store_multi_sample_resolved_tile_color_buffer
    {
        enum class id : u8
        {
            id = 24,
        } a_id;
    } __attribute__((packed));

    struct store_multi_sample_resolved_tile_color_buffer_and_signal_end_of_frame
    {
        enum class id : u8
        {
            id = 25,
        } a_id;
    } __attribute__((packed));

    struct store_tile_buffer_general
    {
        enum class id : u8
        {
            id = 28,
        } a_id;
        enum class option_0 : u16
        {
            buffer_to_store_mask = 0x7 << 0,
            buffer_to_store_none = 0x0 << 0,
            buffer_to_store_color = 0x1 << 0,
            buffer_to_store_z_stensil = 0x2 << 0,
            buffer_to_store_z_only = 0x3 << 0,
            buffer_to_store_vg_mask = 0x4 << 0,
            buffer_to_store_full_dump = 0x5 << 0,
            format_mask = 0x3 << 4,
            format_raster_format = 0x0 << 4,
            format_t_format = 0x1 << 4,
            format_lt_format = 0x2 << 4,
            mode_mask = 0x3 << 6,
            mode_sample_0 = 0x0 << 6,
            mode_decimate_x4 = 0x1 << 6,
            mode_decimate_x16 = 0x2 << 6,
            pixel_color_format_mask = 0x3 << 8,
            pixel_color_format_rgba8888 = 0x0 << 8,
            pixel_color_format_bgr565_dithered = 0x1 << 8,
            pixel_color_format_bgr565 = 0x2 << 8,
            disable_double_buffer_swap_in_double_buffer_mode = 0x1 << 12,
            disable_color_buffer_clear_on_store_dump = 0x1 << 13,
            disable_z_stencil_buffer_clear_on_store_dump = 0x1 << 14,
            disable_vg_mask_buffer_clear_on_store_dump = 0x1 << 15,

        } a_option_0;
        enum class option_1 : u32
        {
            disable_color_buffer_dump = 0x1 << 0,
            disable_z_stencil_buffer_dump = 0x1 << 1,
            disable_vg_buffer_mask_buffer_dump = 0x1 << 2,
            last_tile_of_frame = 0x1 << 3,
            memory_base_address_of_frame_tile_dump_buffer_mask = 0xfffffff0,
            memory_base_address_of_frame_tile_dump_buffer_offset = 4,
        } a_option_1;
    } __attribute__((packed));

    struct indexed_primitive_list
    {
        enum class id : u8
        {
            id = 32,
        } a_id;
        enum class option : u8
        {
            primitive_mode_mask = 0xf << 0,
            primitive_mode_points = 0x0 << 0,
            primitive_mode_lines = 0x1 << 0,
            primitive_mode_line_loop = 0x2 << 0,
            primitive_mode_line_strip = 0x3 << 0,
            primitive_mode_triangles = 0x4 << 0,
            primitive_mode_triangle_strip = 0x5 << 0,
            primitive_mode_triangle_fan = 0x6 << 0,
            index_type_mask = 0xf << 4,
            index_type_8_bit = 0x0 << 4,
            index_type_16_bit = 0x1 << 4,
        } a_option;
        u32 length; //number of indices.
        vc_pointer address_of_indices_list;
        u32 maximum_index;
    } __attribute__((packed));

    struct primitive_list_format
    {
        enum class id : u8
        {
            id = 56,
        } a_id;
        enum class option : u8
        {
            primitive_type_mask = 0xf << 0,
            primitive_type_points = 0x0 << 0,
            primitive_type_lines = 0x1 << 0,
            primitive_type_triangles = 0x2 << 0,
            primitive_type_rht = 0x3 << 0,
            data_type_mask = 0xf << 4,
            data_type_16_bit_index = 0x1 << 4,
            data_type_32_bit_x_y = 0x3 << 4,
        } a_option;
    } __attribute__((packed));

    struct nv_shader_state
    {
        enum class id : u8
        {
            id = 65,
        } a_id;
        vc_pointer memory_address_of_shader_record;
    } __attribute__((packed));

    struct nv_shader_state_record
    {
        enum class flag_bits : u8
        {
            fragment_shader_is_single_threaded = 0x1 << 0,
            point_size_included_in_shaded_vertex_data = 0x1 << 1,
            enable_clipping = 0x1 << 2,
            clip_coordinates_header_included_in_shaded_vertex_data = 0x1 << 3,
        } a_flag_bits;
        u8 shade_vertex_data_stride;
        u8 fragment_shader_number_of_uniforms; //not used currently.
        u8 fragment_shader_number_of_varyings;
        vc_pointer fragment_shader_code_address;
        vc_pointer fragment_shader_uniform_address;
        vc_pointer shaded_vertex_data_address; //128 bit aligned, if including clip coordinate header.
    } __attribute__((packed));

    struct configuration_bits
    {
        enum class id : u8
        {
            id = 96,
        } a_id;
        enum class option_0 : u8
        {
            enable_forward_facing_primitive = 0x1 << 0,
            enable_reverse_facing_primitive = 0x1 << 1,
            clockwise_primitives = 0x1 << 2,
            enable_depth_offset = 0x1 << 3,
            antialiased_points_and_lines = 0x1 << 4, //not actually supported.
            coverage_read_type_16_bit_mask = 0x1 << 5,
            rasterizer_oversample_mode_mask = 0x3 << 6,
            rasterizer_oversample_mode_none = 0x0 << 6,
            rasterizer_oversample_mode_4x = 0x1 << 6,
            rasterizer_oversample_mode_16x = 0x2 << 6,
            rasterizer_oversample_mode_reversed = 0x3 << 6,
        } a_option_0;
        enum class option_1 : u8
        {
            coverage_pipe_select = 0x1 << 0,
            coverage_update_mode_mask = 0x3 << 1,
            coverage_update_mode_nonzero = 0x0 << 1,
            coverage_update_mode_odd = 0x1 << 1,
            coverage_update_mode_or = 0x2 << 1,
            coverage_update_mode_zero = 0x3 << 1,
            coverage_read_mode_leave_on_read = 0x1 << 3,
            depth_test_function_mask = 0x7 << 4,
            depth_test_function_never = 0x0 << 4,
            depth_test_function_lt = 0x1 << 4,
            depth_test_function_eq = 0x2 << 4,
            depth_test_function_le = 0x3 << 4,
            depth_test_function_gt = 0x4 << 4,
            depth_test_function_ne = 0x5 << 4,
            depth_test_function_ge = 0x6 << 4,
            depth_test_function_always = 0x7 << 4,
            z_updates_enable = 0x1 << 7,
        } a_option_1;
        enum class option_2 : u8
        {
            early_z_enable = 0x1 << 0,
            early_z_updates_enable = 0x1 << 1,
        } a_option_2;
    } __attribute__((packed));

    struct clip_window
    {
        enum class id : u8
        {
            id = 102,
        } a_id;
        u16 left_pixel_coordinate;
        u16 bottom_pixel_coordinate;
        u16 width_in_pixels;
        u16 height_in_pixels;
    } __attribute__((packed));

    struct viewport_offset
    {
        enum class id : u8
        {
            id = 103,
        } a_id;
        u16 viewport_centre_x_coordinate;
        u16 viewport_centre_y_coordinate;
    } __attribute__((packed));

    struct tile_binning_mode_configuration
    {
        enum class id : u8
        {
            id = 112,
        } a_id;
        vc_pointer tile_allocation_memory_address;
        u32 tile_allocation_memory_size;
        vc_pointer tile_state_data_array_base_address;
        u8 width;  //in tiles.
        u8 height; //in tiles.
        enum class option : u8
        {
            multisample_mode = 0x1 << 0,
            tile_buffer_64_bit_color_depth = 0x1 << 1,
            auto_initialise_tile_state_data_array = 0x1 << 2,
            tile_allocation_initial_block_size_mask = 0x3 << 3,
            tile_allocation_initial_block_size_32 = 0x0 << 3,
            tile_allocation_initial_block_size_64 = 0x1 << 3,
            tile_allocation_initial_block_size_128 = 0x2 << 3,
            tile_allocation_initial_block_size_256 = 0x3 << 3,
            tile_allocation_block_size_mask = 0x3 << 5,
            tile_allocation_block_size_32 = 0x0 << 5,
            tile_allocation_block_size_64 = 0x1 << 5,
            tile_allocation_block_size_128 = 0x2 << 5,
            tile_allocation_block_size_256 = 0x3 << 5,
            double_buffer_in_non_ms_mode = 0x1 << 7,
        } a_option;
    } __attribute__((packed));

    struct tile_rendering_mode_configuration
    {
        enum class id : u8
        {
            id = 113,
        } a_id;
        vc_pointer memory_address;
        u16 width;
        u16 height;
        enum class option : u16
        {
            multisampling_mode_4x = 0x1 << 0,
            tile_buffer_64_bit_color_depth = 0x1 << 1,
            non_hdr_frame_buffer_color_format_mask = 0x3 << 2,
            non_hdr_frame_buffer_color_format_bgr565_dithered = 0x0 << 2,
            non_hdr_frame_buffer_color_format_rgba8888 = 0x1 << 2,
            non_hdr_frame_buffer_color_format_bgr565 = 0x2 << 2,
            decimate_mode_mask = 0x3 << 4,
            decimate_mode_1x = 0x0 << 4,
            decimate_mode_4x = 0x1 << 4,
            decimate_mode_16x = 0x2 << 4,
            memory_format_mask = 0x3 << 6,
            memory_format_linear = 0x0 << 6,
            memory_format_t_format = 0x1 << 6,
            memory_format_lt_format = 0x2 << 6,
            enable_vg_mask_buffer = 0x1 << 8,
            select_coverage_mode = 0x1 << 9,
            early_z_update_direction_gt_ge = 0x1 << 10,
            early_z_early_cov_disable = 0x1 << 11,
            double_buffer_in_non_ms_mode = 0x1 << 12,
        } a_option;
    } __attribute__((packed));

    struct clear_colors
    {
        enum class id : u8
        {
            id = 114,
        } a_id;
        u64 clear_color; //2 x rgba8888 (put next to each other) or rgba16161616.
        u8 clear_zs_0;   //clear zs is actually u24, but is split in 3 u8 as u24 is not supported.
        u8 clear_zs_1;
        u8 clear_zs_2;
        u8 clear_vg_mask;
        u8 clear_stencil;
    } __attribute__((packed));

    struct tile_coordinates
    {
        enum class id : u8
        {
            id = 115,
        } a_id;
        u8 tile_collumn_number;
        u8 tile_row_number;
    } __attribute__((packed));

    struct binning_control_list
    {
        tile_binning_mode_configuration a_tile_binning_mode_configuration;
        start_tile_binning a_start_tile_binning;
        primitive_list_format a_primitive_list_format;
        clip_window a_clip_window;
        configuration_bits a_configuration_bits;
        viewport_offset a_viewport_offset;
        nv_shader_state a_nv_shader_state;
        indexed_primitive_list a_indexed_primitive_list;
        flush_all_state a_flush_all_state;
        nop a_nop;
        halt a_halt;
    } __attribute__((packed));

    struct binned_list
    {
        tile_coordinates a_tile_coordinates;
        branch_to_sub_list a_branch_to_sub_list;
        union {
            store_multi_sample_resolved_tile_color_buffer a_store_multi_sample_resolved_tile_color_buffer;
            store_multi_sample_resolved_tile_color_buffer_and_signal_end_of_frame a_store_multi_sample_resolved_tile_color_buffer_and_signal_end_of_frame;
        };
    } __attribute__((packed));

    struct render_control_list
    {
        clear_colors a_clear_colors;
        tile_rendering_mode_configuration a_tile_rendering_mode_configuration;
        tile_coordinates a_tile_coordinates;
        store_tile_buffer_general a_store_tile_buffer_general;
        binned_list a_binned_list[];
    } __attribute__((packed));

    struct buffer_tile_data
    {
        //don't know or care what is actually inside, the struct is only here to define the size.
      private:
        // u8 bytes[32];
        u8 bytes[0x4000]; //todo: set correct size;
    } __attribute__((packed));

    struct buffer_tile_state
    {
        //don't know or care what is actually inside, the struct is only here to define the size.
      private:
        // u8 bytes[0x5e00];
        u8 bytes[0x4000]; //todo: set correct size;
    } __attribute__((packed));

    struct vc_instruction
    {
        u32 instruction;

        // void operator=(const vc_instruction& source) = default;
        void operator=(const vc_instruction &source) volatile
        {
            instruction = source.instruction;
        }
    } __attribute__((packed));

    static constexpr u32 fragment_shader_0_size = 18;
    static constexpr vc_instruction fragment_shader_0[fragment_shader_0_size] = {
        0x958e0dbf,
        0xd1724823,
        0x818e7176,
        0x40024821,
        0x818e7376,
        0x10024862,
        0x819e7540,
        0x114248a3,
        0x809e7009,
        0x115049e3,
        0x809e7012,
        0x116049e3,
        0x159e76c0,
        0x30020ba7,
        0x009e7000,
        0x100009e7,
        0x009e7000,
        0x500009e7,
    };

  public:
    //shaded_vertex_no_clip_header_no_point_size_3_varyings
    struct vertex
    {
        u16 xs; //in 12.4 fixed point format.
        u16 ys; //in 12.4 fixed point format.
        f32 zs;
        f32 wc;
        f32 b; //varyings.
        f32 g; //varyings.
        f32 r; //varyings.
    } __attribute__((packed));

    //triangle_16_bit
    struct triangle
    {
        u16 index_0;
        u16 index_1;
        u16 index_2;
    } __attribute__((packed));

  private:
    const vc_pointer framebuffer;
    const u16 framebuffer_width;
    const u16 framebuffer_height;
    const u8 binning_height;
    const u8 binning_width;

    //handles are needed to free the memory afterwards.
    vc_handle a_binning_control_list_handle;
    vc_handle a_buffer_tile_data_handle;
    vc_handle a_buffer_tile_state_handle;
    vc_handle a_nv_shader_state_record_handle;
    vc_handle fragment_shader_handle;
    vc_handle buffer_fragment_uniform_handle;
    vc_handle vertices_handle;
    vc_handle triangles_handle;
    vc_handle a_render_control_list_handle;

    volatile binning_control_list *a_binning_control_list;
    volatile buffer_tile_data *a_buffer_tile_data;
    volatile buffer_tile_state *a_buffer_tile_state;
    volatile nv_shader_state_record *a_nv_shader_state_record;
    volatile vc_instruction *fragment_shader;
    u32 fragment_shader_size;
    volatile u8 *buffer_fragment_uniform;
    u32 buffer_fragment_uniform_size;
    volatile vertex *vertices;
    u32 vertices_size;
    volatile triangle *triangles;
    u32 triangles_size;
    volatile render_control_list *a_render_control_list;

  public:
    vc_gpu(vc_pointer framebuffer, u16 framebuffer_width, u16 framebuffer_height);
    ~vc_gpu();

    void set_triangles(vertex *vertices, u32 vertices_size, triangle *triangles, u32 triangles_size, color background_color);
    void set_triangles(const list<triangle>& triangles, const list<vertex>& vertices, color background_color);
    void render();
};