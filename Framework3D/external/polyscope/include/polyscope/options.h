// Copyright 2017-2023, Nicholas Sharp and the Polyscope contributors. https://polyscope.run

#pragma once

#include <functional>
#include <string>
#include <tuple>

#include "imgui.h"

#include "polyscope/api.h"
#include "polyscope/scaled_value.h"
#include "polyscope/types.h"


namespace polyscope {
namespace options { // A general name to use when referring to the program in window headings.
extern POLYSCOPE_API std::string programName;

// How much should polyscope print to std::out?
// 0 --> none
// 1 --> some
// > 1 --> a lot
extern POLYSCOPE_API int verbosity;

// A string to prefex all messages printed to stdout
extern POLYSCOPE_API std::string printPrefix;

// Should errors throw exceptions, or just display? (default false)
extern POLYSCOPE_API bool errorsThrowExceptions;

// Don't let the main loop run at more than this speed. (-1 disables) (default: 60)
extern POLYSCOPE_API int maxFPS;

// If enable or disable swap synchronization (limits render ray to display refresh rate). (default: true)
// NOTE: some platforms may ignore the setting.
extern POLYSCOPE_API bool enableVSync;

// Read preferences (window size, etc) from startup file, write to same file on exit (default: true)
extern POLYSCOPE_API bool usePrefsFile;

// Should we redraw every frame, even if not requested? (default: false)
extern POLYSCOPE_API bool alwaysRedraw;

// Should we center/scale every structure after it is loaded up (default: false)
extern POLYSCOPE_API bool autocenterStructures;
extern POLYSCOPE_API bool autoscaleStructures;

// If true, Polyscope will automatically compute state::boundingBox and state::lengthScale parameters according to the
// registered structures, and update them whenever structures are added chagned. If false, the bounding box and length
// scale are left unchanged. If set to false before the first structure is registered, the user is required to set the
// bounding box and length scale manually. (default: true)
extern POLYSCOPE_API bool automaticallyComputeSceneExtents;

// If true, the user callback will be invoked for nested calls to polyscope::show(), otherwise not (default: false)
extern POLYSCOPE_API bool invokeUserCallbackForNestedShow;

// If true, focus the Polyscope window when shown (default: false)
extern POLYSCOPE_API bool giveFocusOnShow;

// If true, hide the polyscope window when a show() command finishes (default: true)
extern POLYSCOPE_API bool hideWindowAfterShow;

// === Scene options

// Behavior of the ground plane
extern POLYSCOPE_API GroundPlaneMode groundPlaneMode;
extern POLYSCOPE_API GroundPlaneHeightMode groundPlaneHeightMode;
extern POLYSCOPE_API bool
    groundPlaneEnabled; // deprecated, but kept and respected for compatability. use groundPlaneMode.
extern POLYSCOPE_API ScaledValue<float> groundPlaneHeightFactor;
extern POLYSCOPE_API float groundPlaneHeight;
extern POLYSCOPE_API int shadowBlurIters;
extern POLYSCOPE_API float shadowDarkness;

extern POLYSCOPE_API bool screenshotTransparency; // controls whether screenshots taken by clicking the GUI button have
                                                  // a transparent background
extern POLYSCOPE_API std::string screenshotExtension; // sets the extension used for automatically-numbered screenshots
                                                      // (e.g. by clicking the GUI button)

// === Rendering parameters

// SSAA scaling in pixel multiples
extern POLYSCOPE_API int ssaaFactor;

// Transparency settings for the renderer
extern POLYSCOPE_API TransparencyMode transparencyMode;
extern POLYSCOPE_API int transparencyRenderPasses;

// === Advanced ImGui configuration

// If false, Polyscope will not create any ImGui UIs at all, but will still set up ImGui and invoke its render steps
// each frame. The allows advanced users to create their own UIs totally from scratch and circumvent the standard
// Polyscope UIs. (default: true)
extern POLYSCOPE_API bool buildGui;

// Draw the user gui panel on the right side, if false will be drawn on the left
// (default: true)
extern POLYSCOPE_API bool userGuiIsOnRightSide;

// Build the built-in gui panels (polyscope & structure menus on the left, selection menu on the right)
// The difference between this vs. buildGui is whether or not the user gui panel is drawn, this setting does _not_
// effect the user gui panel. (default: true)
extern POLYSCOPE_API bool buildDefaultGuiPanels;

// If false, Polyscope will not render the scene into the draw buffer
// (this is useful if you are doing custom rendering and filling the draw buffer yourself)
extern POLYSCOPE_API bool renderScene;

// Should the user call back start out with an imgui window context open (default: true)
extern POLYSCOPE_API bool openImGuiWindowForUserCallback;

// A callback function which will be invoked when an ImGui context is created (which may happen several times as
// Polyscope runs). By default, this is set to invoke `configureImGuiStyle()` from Polyscope's imgui_config.cpp, but you
// may assign your own function to create custom styles. If this callback is null, the default ImGui style will be used.
extern POLYSCOPE_API std::function<void()> configureImGuiStyleCallback;

// A callback function which will be invoked exactly once during initialization to construct a font atlas for ImGui to
// use. The callback should return a tuple of three pointers: a newly created global shared font atlas, a regular font,
// and a mono font. By default, this is set to invoke prepareImGuiFonts() from Polyscope's imgui_config.cpp, but you may
// assign your own function to create custom styles. If this callback is null, default fonts will be used.
extern POLYSCOPE_API std::function<std::tuple<ImFontAtlas*, ImFont*, ImFont*>()> prepareImGuiFontsCallback;


// === Debug options

// Enables optional error checks in the rendering system
extern POLYSCOPE_API bool enableRenderErrorChecks;

// Render the pick buffer to screen rather than the regular scene
extern POLYSCOPE_API bool debugDrawPickBuffer;

} // namespace options
} // namespace polyscope
