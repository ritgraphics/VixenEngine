#include <vix_dxdefferedbuffers.h>

namespace Vixen
{
    DXDefferedBuffers::~DXDefferedBuffers() = default;

    void DXDefferedBuffers::ReleaseBuffers()
    {
        for (int i = 0; i < BUFFER_COUNT; i++)
        {
            if (m_RenderTargetTextures[i])
                m_RenderTargetTextures[i] = nullptr;
            if (m_RenderTargetViews[i])
                m_RenderTargetViews[i] = nullptr;
            if (m_ShaderResourceViews[i])
                m_ShaderResourceViews[i] = nullptr;
        }

        if (m_DepthStencilView)
            m_DepthStencilView = nullptr;
        if (m_DepthStencilBuffer)
            m_DepthStencilBuffer = nullptr;
    }

    ID3D11ShaderResourceView* DXDefferedBuffers::GetShaderResourceView(int index)
    {
        if (index < 0 || index >= BUFFER_COUNT)
            return NULL;

        return m_ShaderResourceViews[index].get();
    }

    bool DXDefferedBuffers::Initialize(ID3D11Device* device, uint32_t width, uint32_t height)
    {
        HRESULT hr = S_OK;

        // Setup Render Target Texture Desc

        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        D3D11_TEXTURE2D_DESC dtexDesc;
        ZeroMemory(&dtexDesc, sizeof(D3D11_TEXTURE2D_DESC));
        dtexDesc.Width = width;
        dtexDesc.Height = height;
        dtexDesc.MipLevels = 1;
        dtexDesc.ArraySize = 1;
        dtexDesc.Format = DXGI_FORMAT_R32_FLOAT;
        dtexDesc.SampleDesc.Count = 1;
        dtexDesc.SampleDesc.Quality = 0;
        dtexDesc.Usage = D3D11_USAGE_DEFAULT;
        dtexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        dtexDesc.CPUAccessFlags = 0;
        dtexDesc.MiscFlags = 0;

        // Create render target textures
        int i = 0;
        for (i = 0; i < BUFFER_COUNT; i++)
        {
            if (i == 2)
                hr = device->CreateTexture2D(&dtexDesc, NULL, m_RenderTargetTextures[i].put());
            else
                hr = device->CreateTexture2D(&texDesc, NULL, m_RenderTargetTextures[i].put());
            if (FAILED(hr))
                return false;
        }

        // Setup Render Target View Desc
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        rtvDesc.Format = texDesc.Format;
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        // Create render target views
        for (i = 0; i < BUFFER_COUNT; i++)
        {
            if (i == 2)
                rtvDesc.Format = dtexDesc.Format;
            else
                rtvDesc.Format = texDesc.Format;

            hr =
                device->CreateRenderTargetView(m_RenderTargetTextures[i].get(), &rtvDesc, m_RenderTargetViews[i].put());
            if (FAILED(hr))
                return false;
        }

        // Setup Shader Resource View Desc
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        // Create shader resource views
        for (i = 0; i < BUFFER_COUNT; i++)
        {
            if (i == 2)
                srvDesc.Format = dtexDesc.Format;
            else
                srvDesc.Format = texDesc.Format;

            hr = device->CreateShaderResourceView(m_RenderTargetTextures[i].get(), &srvDesc,
                                                  m_ShaderResourceViews[i].put());
            if (FAILED(hr))
                return false;
        }

        // CREATE DEPTH/STENCIL VIEW
        D3D11_TEXTURE2D_DESC dsd;
        dsd.Width = width;
        dsd.Height = height;
        dsd.MipLevels = 1;
        dsd.ArraySize = 1;
        dsd.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; // D24 S8
        dsd.SampleDesc.Count = 1;
        dsd.SampleDesc.Quality = 0;
        dsd.Usage = D3D11_USAGE_DEFAULT;
        dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        dsd.CPUAccessFlags = 0;
        dsd.MiscFlags = 0;

        hr = device->CreateTexture2D(&dsd, 0, m_DepthStencilBuffer.put());
        if (FAILED(hr))
            return false;

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));
        descDSV.Format = dsd.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;

        hr = device->CreateDepthStencilView(m_DepthStencilBuffer.get(), &descDSV, m_DepthStencilView.put());
        if (FAILED(hr))
            return false;

        return true;
    }

    void DXDefferedBuffers::BindRenderTargets(ID3D11DeviceContext* context)
    {
        // Bind render target view array and depth stencil buffer
        ID3D11RenderTargetView* views[] = { m_RenderTargetViews[0].get(),
        m_RenderTargetViews[1].get(), m_RenderTargetViews[2].get(), m_RenderTargetViews[3].get() };
        context->OMSetRenderTargets(3, views, m_DepthStencilView.get());
    }

    void DXDefferedBuffers::BindRenderTarget(uint32_t index, ID3D11DeviceContext* context)
    {

        ID3D11RenderTargetView* view = m_RenderTargetViews[3].get();
        ID3D11RenderTargetView* views[4] = {view, 0, 0, 0};
        context->OMSetRenderTargets(4, views, m_DepthStencilView.get());
    }

    void DXDefferedBuffers::UnbindRenderTargets(ID3D11DeviceContext* context)
    {
        context->OMSetRenderTargets(BUFFER_COUNT, NULL, NULL);
    }

    void DXDefferedBuffers::ClearRenderTargets(ID3D11DeviceContext* context, DirectX::XMVECTORF32 clearColor)
    {
        for (int i = 0; i < BUFFER_COUNT; i++)
            context->ClearRenderTargetView(m_RenderTargetViews[i].get(), clearColor);

        context->ClearDepthStencilView(m_DepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void DXDefferedBuffers::ClearDepthStencil(ID3D11DeviceContext* context)
    {
        context->ClearDepthStencilView(m_DepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
} // namespace Vixen