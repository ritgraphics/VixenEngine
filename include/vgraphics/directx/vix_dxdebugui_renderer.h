#ifndef VIX_DXDEBUGUI_RENDERER_H
#define VIX_DXDEBUGUI_RENDERER_H

#include <vix_platform.h>
#include <vix_debugui_renderer.h>
#include <vix_directx.h>
#include <vix_dxtexture.h>
#include <vix_dxvertexbuffer.h>
#include <vix_dxindexbuffer.h>
#include <vix_dxvertexshader.h>
#include <vix_dxpixelshader.h>
#include <vix_dxcamera2d.h>

namespace Vixen {

    class VIX_API DXImGuiBuffer : public IVertexBuffer
    {
    public:
        DXImGuiBuffer(size_t count, ID3D11Device* device, ID3D11DeviceContext* context);

        ~DXImGuiBuffer();

        void VSetData(const void* data);
        void VUpdateSubData(size_t offset, size_t stride, size_t count, const void* data);
        void VBind();
        void VUnbind();

        ID3D11Buffer* Buffer();

    private:
        ID3D11Device*          m_device;
        ID3D11DeviceContext*   m_context;
        ID3D11Buffer*          m_buffer;
    };

    class VIX_API DXDebugUIRenderer : public IDebugUIRenderer
    {
    public:
        DXDebugUIRenderer(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd);

        ~DXDebugUIRenderer();

        bool VInitialize();
        void VRenderDrawLists(ImDrawData* data);
        void VInitFontTexture();
        void VInitShaders();
        void VSetCamera(ICamera2D* camera);
        void VResizeBuffers(uint32_t width, uint32_t height);

    private:
        HWND                        m_nativeHandle;
        ID3D11Device*               m_device;
        ID3D11DeviceContext*        m_context;
        ID3D11ShaderResourceView*   m_font;
        ID3D11BlendState*           m_blendState;
        ID3D11RasterizerState*      m_rasterState;
        ID3D11SamplerState*         m_samplerLinear;

        DXImGuiBuffer*              m_vBuffer;
        DXIndexBuffer*              m_iBuffer;
        DXVertexShader*             m_vShader;
        DXPixelShader*              m_pShader;
        DXCamera2D*                 m_camera2D;
        int                         m_vBufferSize;
        int                         m_iBufferSize;
    };

}

#endif