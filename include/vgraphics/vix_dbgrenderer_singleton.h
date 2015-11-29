#ifndef VIX_DBGRENDERER_SINGLETON_H
#define VIX_DBGRENDERER_SINGLETON_H

#include <vix_platform.h>
#include <vix_singleton.h>
#include <vix_renderer.h>
#include <vix_debugui_renderer.h>

namespace Vixen {

    class VIX_API DebugRenderer : public Singleton<DebugRenderer>
    {
    public:
        static bool Initialize(IRenderer* renderer);

        static void DeInitialize();

        static void InitializeShaders();

        static void ResizeBuffers(uint32_t width, uint32_t height);

        static void Render();

        static void RenderDrawLists(ImDrawData* data);

        static void FrameUpdate();

        static void SetCamera(ICamera2D* camera);

    private:
        IDebugUIRenderer* m_debugUI;
    };

}

#endif