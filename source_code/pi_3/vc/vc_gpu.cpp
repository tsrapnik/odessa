#include "vc_gpu.h"
#include "enum_flags.h"
#include "memory.h"
#include "vc_mailbox_property_tags.h"
//todo: remove.
#include "screen.h"
#include "string.h"
#include "uart.h"
extern uart* a_uart;

vc_gpu::v3d_registers* const vc_gpu::v3d = reinterpret_cast<v3d_registers* const>(0x3fc00000);

constexpr vc_gpu::vc_instruction vc_gpu::fragment_shader_0[];

vc_gpu::vc_gpu(vc_pointer framebuffer, u16 framebuffer_width, u16 framebuffer_height) :
    framebuffer(framebuffer),
    framebuffer_width(framebuffer_width),
    framebuffer_height(framebuffer_height),
    binning_height(static_cast<u8>((framebuffer_height + 63) / 64)),
    binning_width(static_cast<u8>((framebuffer_width + 63) / 64)),
    //all handles are initialized in function body, assigning null is just to avoid compiler errors.
    a_binning_control_list_handle(vc_handle::make_null_handler()),
    a_buffer_tile_data_handle(vc_handle::make_null_handler()),
    a_buffer_tile_state_handle(vc_handle::make_null_handler()),
    a_nv_shader_state_record_handle(vc_handle::make_null_handler()),
    fragment_shader_handle(vc_handle::make_null_handler()),
    buffer_fragment_uniform_handle(vc_handle::make_null_handler()),
    vertices_handle(vc_handle::make_null_handler()),
    triangles_handle(vc_handle::make_null_handler()),
    a_render_control_list_handle(vc_handle::make_null_handler())
{
    vc_mailbox_property_tags::set_clock_rate(vc_mailbox_property_tags::clock_id::v3d, 250000000);
    vc_mailbox_property_tags::enable_qpu(true);

    const vc_mailbox_property_tags::allocate_memory_flag allocate_flags =
        vc_mailbox_property_tags::allocate_memory_flag::coherent | vc_mailbox_property_tags::allocate_memory_flag::zero;

    //initially allocate an empty vertices and triangles object, so only background is shown.
    vertices_size = 0;
    vertices_handle = vc_mailbox_property_tags::allocate_memory(vertices_size * sizeof(vertex), 0x1000, allocate_flags);
    vertices = reinterpret_cast<volatile vertex*>(
        vc_mailbox_property_tags::lock_memory(vertices_handle)
            .to_arm_pointer());

    triangles_size = 0;
    triangles_handle = vc_mailbox_property_tags::allocate_memory(triangles_size * sizeof(triangle), 0x1000, allocate_flags);
    triangles = reinterpret_cast<volatile triangle*>(
        vc_mailbox_property_tags::lock_memory(triangles_handle)
            .to_arm_pointer());

    //initialize fragment shader.
    //for now only a standard shader fragment_shader_0 is used.
    fragment_shader_size = fragment_shader_0_size;
    fragment_shader_handle = vc_mailbox_property_tags::allocate_memory(fragment_shader_size * sizeof(vc_instruction), 0x1000, allocate_flags);
    fragment_shader = reinterpret_cast<volatile vc_instruction*>(
        vc_mailbox_property_tags::lock_memory(fragment_shader_handle)
            .to_arm_pointer());

    for(u32 i = 0; i < fragment_shader_size; i++)
    {
        fragment_shader[i] = fragment_shader_0[i];
    }

    //initialize buffer fragment uniform.
    //no uniforms used for now.
    buffer_fragment_uniform_size = 0;
    buffer_fragment_uniform_handle = vc_mailbox_property_tags::allocate_memory(buffer_fragment_uniform_size * sizeof(u8), 0x1000, allocate_flags);
    buffer_fragment_uniform = reinterpret_cast<volatile u8*>(
        vc_mailbox_property_tags::lock_memory(buffer_fragment_uniform_handle)
            .to_arm_pointer());

    //initialize nv shader state record.
    a_nv_shader_state_record_handle = vc_mailbox_property_tags::allocate_memory(sizeof(nv_shader_state_record), 0x1000, allocate_flags);
    a_nv_shader_state_record = reinterpret_cast<volatile nv_shader_state_record*>(
        vc_mailbox_property_tags::lock_memory(a_nv_shader_state_record_handle)
            .to_arm_pointer());

    a_nv_shader_state_record->a_flag_bits =
        vc_gpu::nv_shader_state_record::flag_bits::fragment_shader_is_single_threaded;
    a_nv_shader_state_record->shade_vertex_data_stride = 6 * 4;
    a_nv_shader_state_record->fragment_shader_number_of_uniforms = 0xcc;
    a_nv_shader_state_record->fragment_shader_number_of_varyings = 3;
    a_nv_shader_state_record->fragment_shader_code_address = vc_pointer::arm_to_vc_pointer(fragment_shader);
    a_nv_shader_state_record->fragment_shader_uniform_address = vc_pointer::arm_to_vc_pointer(buffer_fragment_uniform);
    //below should be updated each time set_triangles() reallocates vertices.
    a_nv_shader_state_record->shaded_vertex_data_address = vc_pointer::arm_to_vc_pointer(vertices);

    //initialize buffer tile state.
    a_buffer_tile_state_handle = vc_mailbox_property_tags::allocate_memory(sizeof(buffer_tile_state), 0x1000, allocate_flags);
    a_buffer_tile_state = reinterpret_cast<volatile buffer_tile_state*>(
        vc_mailbox_property_tags::lock_memory(a_buffer_tile_state_handle)
            .to_arm_pointer());

    //initialize buffer tile date.
    a_buffer_tile_data_handle = vc_mailbox_property_tags::allocate_memory(sizeof(buffer_tile_data), 0x1000, allocate_flags);
    a_buffer_tile_data = reinterpret_cast<volatile buffer_tile_data*>(
        vc_mailbox_property_tags::lock_memory(a_buffer_tile_data_handle)
            .to_arm_pointer());

    //initialize binning control list.
    a_binning_control_list_handle = vc_mailbox_property_tags::allocate_memory(sizeof(binning_control_list), 0x1000, allocate_flags);
    a_binning_control_list = reinterpret_cast<volatile binning_control_list*>(
        vc_mailbox_property_tags::lock_memory(a_binning_control_list_handle)
            .to_arm_pointer());
    a_binning_control_list->a_tile_binning_mode_configuration.a_id = tile_binning_mode_configuration::id::id;
    a_binning_control_list->a_tile_binning_mode_configuration.tile_allocation_memory_address = vc_pointer::arm_to_vc_pointer(a_buffer_tile_data);
    a_binning_control_list->a_tile_binning_mode_configuration.tile_allocation_memory_size = sizeof(buffer_tile_state);
    a_binning_control_list->a_tile_binning_mode_configuration.tile_state_data_array_base_address = vc_pointer::arm_to_vc_pointer(a_buffer_tile_state);
    a_binning_control_list->a_tile_binning_mode_configuration.width = binning_width;
    a_binning_control_list->a_tile_binning_mode_configuration.height = binning_height;
    a_binning_control_list->a_tile_binning_mode_configuration.a_option =
        tile_binning_mode_configuration::option::auto_initialise_tile_state_data_array;

    a_binning_control_list->a_start_tile_binning.a_id = start_tile_binning::id::id;

    a_binning_control_list->a_primitive_list_format.a_id = primitive_list_format::id::id;
    a_binning_control_list->a_primitive_list_format.a_option =
        primitive_list_format::option::primitive_type_triangles |
        primitive_list_format::option::data_type_32_bit_x_y;

    a_binning_control_list->a_clip_window.a_id = clip_window::id::id;
    a_binning_control_list->a_clip_window.left_pixel_coordinate = 0;
    a_binning_control_list->a_clip_window.bottom_pixel_coordinate = 0;
    a_binning_control_list->a_clip_window.width_in_pixels = framebuffer_width;
    a_binning_control_list->a_clip_window.height_in_pixels = framebuffer_height;

    a_binning_control_list->a_configuration_bits.a_id = configuration_bits::id::id;
    a_binning_control_list->a_configuration_bits.a_option_0 =
        configuration_bits::option_0::enable_forward_facing_primitive |
        configuration_bits::option_0::enable_reverse_facing_primitive |
        configuration_bits::option_0::rasterizer_oversample_mode_none;
    a_binning_control_list->a_configuration_bits.a_option_1 =
        configuration_bits::option_1::coverage_update_mode_nonzero |
        configuration_bits::option_1::depth_test_function_never;
    a_binning_control_list->a_configuration_bits.a_option_2 =
        configuration_bits::option_2::early_z_updates_enable;

    a_binning_control_list->a_viewport_offset.a_id = viewport_offset::id::id;
    a_binning_control_list->a_viewport_offset.viewport_centre_x_coordinate = 0;
    a_binning_control_list->a_viewport_offset.viewport_centre_y_coordinate = 0;

    a_binning_control_list->a_nv_shader_state.a_id = nv_shader_state::id::id;
    a_binning_control_list->a_nv_shader_state.memory_address_of_shader_record = vc_pointer::arm_to_vc_pointer(a_nv_shader_state_record);

    a_binning_control_list->a_indexed_primitive_list.a_id = indexed_primitive_list::id::id;
    a_binning_control_list->a_indexed_primitive_list.a_option =
        indexed_primitive_list::option::primitive_mode_triangles |
        indexed_primitive_list::option::index_type_16_bit;
    //below three are updated each time set_triangles() reallocates triangles or vertices.
    a_binning_control_list->a_indexed_primitive_list.length = triangles_size * 3;
    a_binning_control_list->a_indexed_primitive_list.address_of_indices_list = vc_pointer::arm_to_vc_pointer(triangles);
    a_binning_control_list->a_indexed_primitive_list.maximum_index = vertices_size - 1;

    a_binning_control_list->a_flush_all_state.a_id = flush_all_state::id::id;

    a_binning_control_list->a_nop.a_id = nop::id::id;

    a_binning_control_list->a_halt.a_id = halt::id::id;

    //initialize render control.
    a_render_control_list_handle = vc_mailbox_property_tags::allocate_memory(sizeof(render_control_list) + binning_height * binning_width * sizeof(binned_list), 0x1000, allocate_flags);
    a_render_control_list = reinterpret_cast<volatile render_control_list*>(
        vc_mailbox_property_tags::lock_memory(a_render_control_list_handle)
            .to_arm_pointer());

    a_render_control_list->a_clear_colors.a_id = vc_gpu::clear_colors::id::id;
    //update below each time the background color changes.
    a_render_control_list->a_clear_colors.clear_color = 0;
    a_render_control_list->a_clear_colors.clear_stencil = 0;
    a_render_control_list->a_clear_colors.clear_vg_mask = 0;
    a_render_control_list->a_clear_colors.clear_zs_0 = 0;
    a_render_control_list->a_clear_colors.clear_zs_1 = 0;
    a_render_control_list->a_clear_colors.clear_zs_2 = 0;

    a_render_control_list->a_tile_rendering_mode_configuration.a_id = vc_gpu::tile_rendering_mode_configuration::id::id;
    a_render_control_list->a_tile_rendering_mode_configuration.memory_address = framebuffer;
    a_render_control_list->a_tile_rendering_mode_configuration.width = framebuffer_width;
    a_render_control_list->a_tile_rendering_mode_configuration.height = framebuffer_height;
    a_render_control_list->a_tile_rendering_mode_configuration.a_option =
        vc_gpu::tile_rendering_mode_configuration::option::non_hdr_frame_buffer_color_format_rgba8888;

    a_render_control_list->a_tile_coordinates.a_id = vc_gpu::tile_coordinates::id::id;
    a_render_control_list->a_tile_coordinates.tile_row_number = 0;
    a_render_control_list->a_tile_coordinates.tile_collumn_number = 0;

    a_render_control_list->a_store_tile_buffer_general.a_id = vc_gpu::store_tile_buffer_general::id::id;
    a_render_control_list->a_store_tile_buffer_general.a_option_0 =
        vc_gpu::store_tile_buffer_general::option_0::pixel_color_format_rgba8888;
    a_render_control_list->a_store_tile_buffer_general.a_option_1 =
        static_cast<vc_gpu::store_tile_buffer_general::option_1>(0);

    for(int x = 0; x < binning_width; x++)
    {
        for(int y = 0; y < binning_height; y++)
        {
            a_render_control_list->a_binned_list[x * binning_height + y].a_tile_coordinates.a_id = vc_gpu::tile_coordinates::id::id;
            a_render_control_list->a_binned_list[x * binning_height + y].a_tile_coordinates.tile_collumn_number = x;
            a_render_control_list->a_binned_list[x * binning_height + y].a_tile_coordinates.tile_row_number = y;

            a_render_control_list->a_binned_list[x * binning_height + y].a_branch_to_sub_list.a_id = vc_gpu::branch_to_sub_list::id::id;
            a_render_control_list->a_binned_list[x * binning_height + y].a_branch_to_sub_list.absolute_branch_address = vc_pointer::arm_to_vc_pointer(a_buffer_tile_data) + (y * binning_width + x) * 32;

            if(x == (binning_width - 1) && (y == binning_height - 1))
                a_render_control_list->a_binned_list[x * binning_height + y].a_store_multi_sample_resolved_tile_color_buffer_and_signal_end_of_frame.a_id = vc_gpu::store_multi_sample_resolved_tile_color_buffer_and_signal_end_of_frame::id::id;
            else
                a_render_control_list->a_binned_list[x * binning_height + y].a_store_multi_sample_resolved_tile_color_buffer.a_id = vc_gpu::store_multi_sample_resolved_tile_color_buffer::id::id;
        }
    }
}

void vc_gpu::resize_vertices(u32 vertices_size)
{
    const vc_mailbox_property_tags::allocate_memory_flag allocate_flags =
        vc_mailbox_property_tags::allocate_memory_flag::coherent | vc_mailbox_property_tags::allocate_memory_flag::zero;

    if(!vertices_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(vertices_handle);
        vc_mailbox_property_tags::release_memory(vertices_handle);
    }
    this->vertices_size = vertices_size;
    vertices_handle = vc_mailbox_property_tags::allocate_memory(vertices_size * sizeof(vertex), 0x1000, allocate_flags);
    this->vertices = reinterpret_cast<volatile vertex*>(
        vc_mailbox_property_tags::lock_memory(vertices_handle)
            .to_arm_pointer());

    a_nv_shader_state_record->shaded_vertex_data_address = vc_pointer::arm_to_vc_pointer(this->vertices);
    a_binning_control_list->a_indexed_primitive_list.maximum_index = vertices_size - 1;
}

void vc_gpu::resize_triangles(u32 triangles_size)
{
    const vc_mailbox_property_tags::allocate_memory_flag allocate_flags =
        vc_mailbox_property_tags::allocate_memory_flag::coherent | vc_mailbox_property_tags::allocate_memory_flag::zero;

    if(!triangles_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(triangles_handle);
        vc_mailbox_property_tags::release_memory(triangles_handle);
    }
    this->triangles_size = triangles_size;
    triangles_handle = vc_mailbox_property_tags::allocate_memory(triangles_size * sizeof(triangle), 0x1000, allocate_flags);
    this->triangles = reinterpret_cast<volatile triangle*>(
        vc_mailbox_property_tags::lock_memory(triangles_handle)
            .to_arm_pointer());

    a_binning_control_list->a_indexed_primitive_list.length = triangles_size * 3;
    a_binning_control_list->a_indexed_primitive_list.address_of_indices_list = vc_pointer::arm_to_vc_pointer(this->triangles);
}

void vc_gpu::set_triangles(vertex* vertices, u32 vertices_size, triangle* triangles, u32 triangles_size, color background_color)
{
    if(vertices_size != this->vertices_size)
        resize_vertices(vertices_size);

    if(triangles_size != this->triangles_size)
        resize_triangles(triangles_size);

    a_render_control_list->a_clear_colors.clear_color =
        (static_cast<u64>(background_color.to_argb_u32()) << 32) + static_cast<u64>(background_color.to_argb_u32());

    for(u32 index = 0; index < vertices_size; index++)
    {
        this->vertices[index].xs = vertices[index].xs;
        this->vertices[index].ys = vertices[index].ys;
        this->vertices[index].zs = vertices[index].zs;
        this->vertices[index].wc = vertices[index].wc;
        this->vertices[index].r = vertices[index].r;
        this->vertices[index].g = vertices[index].g;
        this->vertices[index].b = vertices[index].b;
    }

    for(u32 index = 0; index < triangles_size; index++)
    {
        this->triangles[index].index_0 = triangles[index].index_0;
        this->triangles[index].index_1 = triangles[index].index_1;
        this->triangles[index].index_2 = triangles[index].index_2;
    }
}

void vc_gpu::set_triangles(list<vertex>& vertices, list<triangle>& triangles, color background_color)
{
    a_uart->write("vertices: ", 10);
    a_uart->write(string::to_string(vertices.get_size()), 18);
    a_uart->write("\r\n", 2);
    a_uart->write("triangles: ", 11);
    a_uart->write(string::to_string(triangles.get_size()), 18);
    a_uart->write("\r\n", 2);
    if(vertices.get_size() != this->vertices_size)
        resize_vertices(vertices.get_size());

    if(triangles.get_size() != this->triangles_size)
        resize_triangles(triangles.get_size());

    a_render_control_list->a_clear_colors.clear_color =
        (static_cast<u64>(background_color.to_argb_u32()) << 32) + static_cast<u64>(background_color.to_argb_u32());

    u32 index = 0;
    list_iterator<vertex> vertices_iterator(vertices);
    //todo: remove.
    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);
    // a_uart->write("\r\n", 2);
    for(vertices_iterator.to_first(); !vertices_iterator.at_end(); vertices_iterator++)
    {
        vertex& current_vertex = vertices_iterator.get_data_reference();
        this->vertices[index].xs = current_vertex.xs;
        this->vertices[index].ys = current_vertex.ys;
        this->vertices[index].zs = current_vertex.zs;
        this->vertices[index].wc = current_vertex.wc;
        this->vertices[index].r = current_vertex.r;
        this->vertices[index].g = current_vertex.g;
        this->vertices[index].b = current_vertex.b;

        //todo: remove.
        // a_uart->write(string::to_string(index), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.xs), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].xs), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.ys), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].ys), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.zs), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].zs), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.wc), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].wc), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.r), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].r), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.g), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].g), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_vertex.b), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->vertices[index].b), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);

        index++;
    }

    index = 0;
    list_iterator<triangle> triangles_iterator(triangles);
    for(triangles_iterator.to_first(); !triangles_iterator.at_end(); triangles_iterator++)
    {
        triangle& current_triangle = triangles_iterator.get_data_reference();
        this->triangles[index].index_0 = current_triangle.index_0;
        this->triangles[index].index_1 = current_triangle.index_1;
        this->triangles[index].index_2 = current_triangle.index_2;

        //todo: remove.
        // a_uart->write(string::to_string(index), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_triangle.index_0), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->triangles[index].index_0), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_triangle.index_1), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->triangles[index].index_1), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(current_triangle.index_2), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write(string::to_string(this->triangles[index].index_2), 18);
        // a_uart->write("\r\n", 2);
        // a_uart->write("\r\n", 2);

        index++;
    }
}

void vc_gpu::render()
{
    //clear caches.
    v3d->l2cactl = 4;
    v3d->slcactl = 0x0f0f0f0f;

    //stop the thread.
    v3d->ct0cs = 0x20;
    //wait for it to stop.
    while(v3d->ct0cs & 0x20)
        ;

    //run binning control list.
    v3d->bfc = 1;
    v3d->ct0ca = vc_pointer::arm_to_vc_pointer(a_binning_control_list).get_raw_value();
    v3d->ct0ea = vc_pointer::arm_to_vc_pointer(a_binning_control_list).get_raw_value() + sizeof(binning_control_list);

    //wait for binning to finish.
    while(v3d->bfc == 0)
        ;

    //stop the thread.
    v3d->ct1cs = 0x20;
    // wait for it to stop.
    while(v3d->ct1cs & 0x20)
        ;

    //run render control list.
    v3d->rfc = 1;
    v3d->ct1ca = vc_pointer::arm_to_vc_pointer(a_render_control_list).get_raw_value();
    v3d->ct1ea = vc_pointer::arm_to_vc_pointer(a_render_control_list).get_raw_value() + sizeof(render_control_list) + binning_height * binning_width * sizeof(binned_list);

    //wait for rendering to finish.
    while(v3d->rfc == 0)
        ;
}

vc_gpu::~vc_gpu()
{
    if(!a_binning_control_list_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(a_binning_control_list_handle);
        vc_mailbox_property_tags::release_memory(a_binning_control_list_handle);
    }
    if(!a_buffer_tile_data_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(a_buffer_tile_data_handle);
        vc_mailbox_property_tags::release_memory(a_buffer_tile_data_handle);
    }
    if(!a_buffer_tile_state_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(a_buffer_tile_state_handle);
        vc_mailbox_property_tags::release_memory(a_buffer_tile_state_handle);
    }
    if(!a_nv_shader_state_record_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(a_nv_shader_state_record_handle);
        vc_mailbox_property_tags::release_memory(a_nv_shader_state_record_handle);
    }
    if(!fragment_shader_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(fragment_shader_handle);
        vc_mailbox_property_tags::release_memory(fragment_shader_handle);
    }
    if(!buffer_fragment_uniform_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(buffer_fragment_uniform_handle);
        vc_mailbox_property_tags::release_memory(buffer_fragment_uniform_handle);
    }
    if(!vertices_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(vertices_handle);
        vc_mailbox_property_tags::release_memory(vertices_handle);
    }
    if(!triangles_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(triangles_handle);
        vc_mailbox_property_tags::release_memory(triangles_handle);
    }
    if(!a_render_control_list_handle.is_null())
    {
        vc_mailbox_property_tags::unlock_memory(a_render_control_list_handle);
        vc_mailbox_property_tags::release_memory(a_render_control_list_handle);
    }
}