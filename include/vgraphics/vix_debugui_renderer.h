#ifndef VIX_DEBUGUI_RENDERER_H
#define VIX_DEBUGUI_RENDERER_H

#include <vix_platform.h>
#include <vix_imgui.h>
#include <vix_camera2d.h>
#include <cstdint>

namespace Vixen {

    class VIX_API IDebugUIRenderer
    {
    public:
        virtual ~IDebugUIRenderer() { };

        virtual bool VInitialize() = 0;
        virtual void VRenderDrawLists(ImDrawData* data) = 0;
        virtual void VInitShaders() = 0;
        virtual void VInitFontTexture() = 0;
        virtual void VSetCamera(ICamera2D* camera) = 0;
        virtual void VResizeBuffers(uint32_t width, uint32_t height) = 0;
    };

}

#endif