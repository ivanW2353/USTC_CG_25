﻿
#include "nodes/core/def/node_def.hpp"
#include "nvrhi/nvrhi.h"
#include "nvrhi/utils.h"
#include "render_node_base.h"
#include "shaders/shaders/utils/taa_cb.h"
#include "utils/math.h"
NODE_DEF_OPEN_SCOPE
struct CameraState {
    pxr::GfMatrix4f camera_status;
};

NODE_DECLARATION_FUNCTION(taa)
{
    b.add_input<nvrhi::TextureHandle>("Previous Frame");
    b.add_input<nvrhi::TextureHandle>("Current Frame");
    b.add_input<nvrhi::TextureHandle>("Motion Vector");

    b.add_output<nvrhi::TextureHandle>("Output Frame");
}

NODE_EXECUTION_FUNCTION(taa)
{
    auto previous = params.get_input<TextureHandle>("Previous Frame");
    auto current = params.get_input<TextureHandle>("Current Frame");
    if (!previous) {
        previous = current;
    }
    else {
        assert(previous != current);
    }

    auto cam = get_free_camera(params);

    auto motion_vector = params.get_input<TextureHandle>("Motion Vector");
    auto texture_info = current->getDesc();

    texture_info.isUAV = true;
    auto output = resource_allocator.create(texture_info);

    std::string error_string;
    ShaderReflectionInfo reflection;
    auto compute_shader = shader_factory.compile_shader(
        "main",
        nvrhi::ShaderType::Compute,
        "shaders/TAA.slang",
        reflection,
        error_string);
    MARK_DESTROY_NVRHI_RESOURCE(compute_shader);
    nvrhi::BindingLayoutDescVector binding_layout_desc_vec =
        reflection.get_binding_layout_descs();

    if (!error_string.empty()) {
        resource_allocator.destroy(output);
        log::warning(error_string.c_str());
        return false;
    }

    auto binding_layout = resource_allocator.create(binding_layout_desc_vec[0]);
    MARK_DESTROY_NVRHI_RESOURCE(binding_layout);

    ComputePipelineDesc pipeline_desc;
    pipeline_desc.CS = compute_shader;
    pipeline_desc.bindingLayouts = { binding_layout };
    auto compute_pipeline = resource_allocator.create(pipeline_desc);
    MARK_DESTROY_NVRHI_RESOURCE(compute_pipeline);

    auto command_list = resource_allocator.create(CommandListDesc{});
    MARK_DESTROY_NVRHI_RESOURCE(command_list);

    auto samplerDesc =
        nvrhi::SamplerDesc().setAllFilters(false).setAllAddressModes(
            nvrhi::SamplerAddressMode::Clamp);
    auto point_sampler = resource_allocator.create(samplerDesc);
    MARK_DESTROY_NVRHI_RESOURCE(point_sampler);

    auto cb_desc = BufferDesc()
                       .setIsConstantBuffer(true)
                       .setByteSize(sizeof(FrameConstants))
                       .setCpuAccess(nvrhi::CpuAccessMode::Write);

    auto frame_constants = resource_allocator.create(cb_desc);
    MARK_DESTROY_NVRHI_RESOURCE(frame_constants);

    BindingSetDesc binding_set_desc;
    binding_set_desc.bindings = {
        nvrhi::BindingSetItem::Texture_SRV(0, current),
        nvrhi::BindingSetItem::Texture_SRV(1, previous),
        nvrhi::BindingSetItem::Texture_SRV(2, motion_vector),
        nvrhi::BindingSetItem::Texture_UAV(0, output),
        nvrhi::BindingSetItem::Sampler(0, point_sampler),
        nvrhi::BindingSetItem::ConstantBuffer(0, frame_constants)
    };
    auto binding_set =
        resource_allocator.create(binding_set_desc, binding_layout.Get());
    MARK_DESTROY_NVRHI_RESOURCE(binding_set);

    FrameConstants cpu_frame_constants;
    cpu_frame_constants.Resolution[0] = texture_info.width;
    cpu_frame_constants.Resolution[1] = texture_info.height;

    command_list->open();
    command_list->writeBuffer(
        frame_constants, &cpu_frame_constants, sizeof(FrameConstants));
    nvrhi::ComputeState compute_state;
    compute_state.pipeline = compute_pipeline;
    compute_state.addBindingSet(binding_set);
    command_list->setComputeState(compute_state);
    command_list->dispatch(
        div_ceil(texture_info.width, 8), div_ceil(texture_info.height, 8));
    command_list->close();

    resource_allocator.device->executeCommandList(command_list);

    params.set_output("Output Frame", output);
    return true;
}

NODE_DECLARATION_UI(taa);
NODE_DEF_CLOSE_SCOPE
