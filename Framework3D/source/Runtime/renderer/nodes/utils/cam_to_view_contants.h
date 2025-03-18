#pragma once

#include "utils/view_cb.h"

USTC_CG_NAMESPACE_OPEN_SCOPE

inline PlanarViewConstants camera_to_view_constants(Hd_USTC_CG_Camera* camera)
{
    PlanarViewConstants constants;

    // Extract matrices and other data from Hd_USTC_CG_Camera
    constants.matWorldToView = GfMatrix4f(camera->viewMatrix);
    constants.matViewToClip = GfMatrix4f(camera->projMatrix);
    constants.matWorldToClip =
        constants.matWorldToView * constants.matViewToClip;
    constants.matClipToView = constants.matViewToClip.GetInverse();

    constants.matViewToWorld = constants.matWorldToView.GetInverse();
    constants.matClipToWorld = constants.matWorldToClip.GetInverse();

    // Extract viewport data
    constants.viewportOrigin =
        GfVec2f(0.0f, 0.0f);  // Assuming origin at (0, 0)
    constants.viewportSize = GfVec2f(camera->dataWindow.GetSize());

    // Calculate inverse of viewport size for efficient computations
    constants.viewportSizeInv = GfVec2f(
        1.0f / constants.viewportSize[0], 1.0f / constants.viewportSize[1]);

    // Other parameters
    constants.pixelOffset =
        GfVec2f(0.0f, 0.0f);  // Assuming no offset initially

    // For camera direction or position, we may need to decide based on the
    // camera implementation
    auto position = camera->inverseViewMatrix.ExtractTranslation();

    constants.cameraDirectionOrPosition =
        GfVec4f(position[0], position[1], position[2], 1.0f);

    constants.resolution = camera->dataWindow.GetSize();
    return constants;
}

USTC_CG_NAMESPACE_CLOSE_SCOPE