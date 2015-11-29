#include <vix_dxdebugui_renderer.h>
#include <vix_resourcemanager.h>
#include <vix_dbgrenderer_singleton.h>

namespace Vixen {

    ////////////////////////////////////////////////////////////////////////////////////////
    // DXImGuiBuffer
    ////////////////////////////////////////////////////////////////////////////////////////

    DXImGuiBuffer::DXImGuiBuffer(size_t count, ID3D11Device* device, ID3D11DeviceContext* context)
    {
        m_count = count;
        m_device = device;
        m_context = context;
        m_buffer = nullptr;

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(ImDrawVert) * m_count;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        m_device->CreateBuffer(&bd, nullptr, &m_buffer);

    }

    DXImGuiBuffer::~DXImGuiBuffer()
    {
        ReleaseCOM(m_buffer);
    }

    void DXImGuiBuffer::VSetData(const void* data)
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(ImDrawVert) * m_count;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = data;
        m_device->CreateBuffer(&bd, &InitData, &m_buffer);
    }

    void DXImGuiBuffer::VUpdateSubData(size_t offset, size_t stride, size_t count, const void* data)
    {

        HRESULT hr = S_OK;

        D3D11_MAP type = (offset <= 0) ? D3D11_MAP_WRITE_DISCARD : D3D11_MAP_WRITE_NO_OVERWRITE;
        D3D11_MAPPED_SUBRESOURCE map;
        hr = m_context->Map(m_buffer, 0, type, 0, &map);
        if (SUCCEEDED(hr))
        {
            memcpy(map.pData, data, stride * count);
        }
        m_context->Unmap(m_buffer, 0);

    }

    void DXImGuiBuffer::VBind()
    {
        unsigned stride = sizeof(ImDrawVert);
        unsigned offset = 0;
        m_context->IASetVertexBuffers(0, 1, &m_buffer, &stride, &offset);
    }

    void DXImGuiBuffer::VUnbind()
    {
        m_context->IASetVertexBuffers(0, 1, nullptr, NULL, NULL);
    }

    ID3D11Buffer* DXImGuiBuffer::Buffer()
    {
        return m_buffer;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    // DXDebugUIRenderer
    ////////////////////////////////////////////////////////////////////////////////////////

    DXDebugUIRenderer::DXDebugUIRenderer(ID3D11Device* device, ID3D11DeviceContext* context, HWND hwnd)
    {
        m_nativeHandle = hwnd;
        m_device = device;
        m_context = context;

        m_vBuffer = nullptr;
        m_iBuffer = nullptr;
    }

    DXDebugUIRenderer::~DXDebugUIRenderer()
    {
        delete m_vBuffer;
        delete m_iBuffer;

        ReleaseCOM(m_blendState);
        ReleaseCOM(m_rasterState);
        ReleaseCOM(m_font);
        ReleaseCOM(m_samplerLinear);

        ResourceManager::DecrementAssetRef(m_vShader);
        ResourceManager::DecrementAssetRef(m_pShader);
    }

    bool DXDebugUIRenderer::VInitialize()
    {
        ImGuiIO& io = ImGui::GetIO();

        RECT r;
        GetClientRect(m_nativeHandle, &r);

        io.DisplaySize.x = r.right - r.left;
        io.DisplaySize.y = r.bottom - r.top;
        

        //Alpha transparency blend state
        D3D11_BLEND_DESC blendDesc;
        ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        m_device->CreateBlendState(&blendDesc, &m_blendState);

        D3D11_RASTERIZER_DESC rastDesc;
        ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
        rastDesc.FillMode = D3D11_FILL_SOLID;
        rastDesc.CullMode = D3D11_CULL_NONE;
        rastDesc.ScissorEnable = true;
        rastDesc.DepthClipEnable = true;
        m_device->CreateRasterizerState(&rastDesc, &m_rasterState);

        VInitFontTexture();

        return true;
    }

    void DXDebugUIRenderer::VRenderDrawLists(ImDrawData* data)
    {
        // Create and grow vertex/index buffers if needed
        if (!m_vBuffer || m_vBufferSize < data->TotalVtxCount)
        {
            if (m_vBuffer) { 
                delete m_vBuffer; 
                m_vBuffer = nullptr;
            }

            m_vBufferSize = data->TotalVtxCount + 5000;
            
            m_vBuffer = new DXImGuiBuffer(m_vBufferSize, m_device, m_context);
        }

        if (!m_iBuffer || m_iBufferSize < data->TotalIdxCount)
        {
            if (m_iBuffer) {
                delete m_iBuffer;
                m_iBuffer = nullptr;
            }

            m_iBufferSize = data->TotalIdxCount + 10000;
            
            m_iBuffer = new DXIndexBuffer(m_iBufferSize, m_device, m_context);
        }

        //Map ImGui command list data to buffers
        D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
        if (m_context->Map(m_vBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
            return;
        if (m_context->Map(m_iBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
            return;
        ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
        ImDrawIdx*  idx_dst = (ImDrawIdx*)idx_resource.pData;
        for (int n = 0; n < data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = data->CmdLists[n];
            
            memcpy(vtx_dst, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
            memcpy(idx_dst, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx));

            vtx_dst += cmd_list->VtxBuffer.size();
            idx_dst += cmd_list->IdxBuffer.size();
        }

        m_context->Unmap(m_vBuffer->Buffer(), 0);
        m_context->Unmap(m_iBuffer->Buffer(), 0);

        //Bind vertex and index buffers, as well as shaders

        D3D11_VIEWPORT vp;
        memset(&vp, 0, sizeof(D3D11_VIEWPORT));
        vp.Width = ImGui::GetIO().DisplaySize.x;
        vp.Height = ImGui::GetIO().DisplaySize.y;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_context->RSSetViewports(1, &vp);

        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_vShader->SetMatrix4x4("projection", m_camera2D->Projection());
        m_pShader->VSetShaderResourceView("texture0", m_font);
        m_pShader->VSetSamplerState("samLinear", m_samplerLinear);
        m_vBuffer->VBind();
        m_iBuffer->VBind();
        m_vShader->Activate();
        m_pShader->Activate();

        const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_context->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);
        m_context->RSSetState(m_rasterState);

        int voffset = 0;
        int ioffset = 0;
        for (int n = 0; n < data->CmdListsCount; n++)
        {
            const ImDrawList* cmd_list = data->CmdLists[n];
            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback)
                {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
                else
                {
                    const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
                    m_pShader->VSetShaderResourceView("texture0", (ID3D11ShaderResourceView*)pcmd->TextureId);
                    m_context->RSSetScissorRects(1, &r);
                    m_context->DrawIndexed(pcmd->ElemCount, ioffset, voffset);
                }
                ioffset += pcmd->ElemCount;
            }
            voffset += cmd_list->VtxBuffer.size();
        }

        m_context->OMSetBlendState(nullptr, nullptr, 0xffffffff);
        m_context->RSSetState(nullptr);
        m_vShader->Deactivate();
        m_pShader->Deactivate();
    }

    void DXDebugUIRenderer::VInitShaders()
    {
        m_vShader = (DXVertexShader*)ResourceManager::OpenShader(VTEXT("DebugUI_VS.hlsl"), ShaderType::VERTEX_SHADER);
        m_vShader->IncrementRefCount();
        m_pShader = (DXPixelShader*)ResourceManager::OpenShader(VTEXT("DebugUI_PS.hlsl"), ShaderType::PIXEL_SHADER);
        m_pShader->IncrementRefCount();
    }

    void DXDebugUIRenderer::VInitFontTexture()
    {
        ImGuiIO& io = ImGui::GetIO();

        BYTE* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        //Create texture from memory
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(texDesc));
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;

        ID3D11Texture2D *pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = pixels;
        subResource.SysMemPitch = texDesc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        m_device->CreateTexture2D(&texDesc, &subResource, &pTexture);

        // Create texture view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
        srvDesc.Texture2D.MostDetailedMip = 0;
        m_device->CreateShaderResourceView(pTexture, &srvDesc, &m_font);

        ReleaseCOM(pTexture);

        //Store font identifier
        io.Fonts->TexID = (void*)m_font;

        //Create texture sampler
        D3D11_SAMPLER_DESC samplerDesc;
        ZeroMemory(&samplerDesc, sizeof(samplerDesc));
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.f;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDesc.MinLOD = 0.f;
        samplerDesc.MaxLOD = 0.f;
        m_device->CreateSamplerState(&samplerDesc, &m_samplerLinear);


        // NOTE:
        // Only do this if you don't want to add fonts later at runtime.
        io.Fonts->ClearInputData();
        io.Fonts->ClearTexData();
    }

    void DXDebugUIRenderer::VSetCamera(ICamera2D* camera)
    {
        m_camera2D = (DXCamera2D*)camera;
    }

    void DXDebugUIRenderer::VResizeBuffers(uint32_t width, uint32_t height)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.DisplaySize.x = width;
        io.DisplaySize.y = height;
    }

}