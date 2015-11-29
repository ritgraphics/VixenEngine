#include <vix_dbgrenderer_singleton.h>

#ifdef VIX_SYS_WINDOWS
#include <vix_dxrenderer.h>
#include <vix_dxdebugui_renderer.h>
#endif

namespace Vixen {

    bool DebugRenderer::Initialize(IRenderer* renderer)
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

#ifdef VIX_SYS_WINDOWS
        _renderer.m_debugUI = new DXDebugUIRenderer(((DXRenderer*)renderer)->Device(),
                                          ((DXRenderer*)renderer)->DeviceContext(),
                                          ((DXRenderer*)renderer)->NativeHandle());
        _renderer.m_debugUI->VInitialize();
#endif

        ImGuiIO& io = ImGui::GetIO();
        
        io.RenderDrawListsFn = &DebugRenderer::RenderDrawLists;


        return true;
    }

    void DebugRenderer::DeInitialize()
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        delete _renderer.m_debugUI;
    }

    void DebugRenderer::InitializeShaders()
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        _renderer.m_debugUI->VInitShaders();
    }

    void DebugRenderer::RenderDrawLists(ImDrawData* data)
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        _renderer.m_debugUI->VRenderDrawLists(data);
    }

    void DebugRenderer::FrameUpdate()
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        ImGui::NewFrame();
    }

    void DebugRenderer::ResizeBuffers(uint32_t width, uint32_t height)
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        _renderer.m_debugUI->VResizeBuffers(width, height);
    }

    void DebugRenderer::Render()
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
       
        ImGui::Render();
    }


    void DebugRenderer::SetCamera(ICamera2D* camera)
    {
        DebugRenderer& _renderer = DebugRenderer::instance();

        _renderer.m_debugUI->VSetCamera(camera);
    }
}