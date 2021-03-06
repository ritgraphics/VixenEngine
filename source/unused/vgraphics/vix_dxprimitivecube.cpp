/*
    The MIT License(MIT)

    Copyright(c) 2015 Matt Guerrette

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <vix_dxprimitivecube.h>

namespace Vixen {

    DXPrimitiveCube::DXPrimitiveCube()
    {
        m_vertexBuffer = nullptr;
        m_indexBuffer = nullptr;
    }

    DXPrimitiveCube::~DXPrimitiveCube()
    {
        ReleaseCOM(m_vertexBuffer);
        ReleaseCOM(m_indexBuffer);
    }

    void DXPrimitiveCube::Initialize(ID3D11Device* device)
    {
        DXVertexPosColor verts[8] =
        {
            {DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Blue) },
            {DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Red) },
            {DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Yellow) },
            {DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Green) },
            {DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Pink) },
            {DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Purple) },
            {DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Orange) },
            {DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Teal) }
        };

        USHORT indices[36] =
        {
            3,1,0,
            2,1,3,

            0,5,4,
            1,5,0,

            3,4,7,
            0,4,3,

            1,6,5,
            2,6,1,

            2,7,6,
            3,7,2,

            6,4,5,
            7,4,6,
        };

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(DXVertexPosColor) * 8;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA InitData;
        ZeroMemory(&InitData, sizeof(InitData));
        InitData.pSysMem = verts;
        device->CreateBuffer(&bd, &InitData, &m_vertexBuffer);

        D3D11_BUFFER_DESC ibd;
        ZeroMemory(&ibd, sizeof(ibd));
        ibd.Usage = D3D11_USAGE_DEFAULT;
        ibd.ByteWidth = sizeof(USHORT) * 36;
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA IndexInitData;
        ZeroMemory(&IndexInitData, sizeof(IndexInitData));
        IndexInitData.pSysMem = indices;
        device->CreateBuffer(&ibd, &IndexInitData, &m_indexBuffer);



      
    }


    void DXPrimitiveCube::Render(ID3D11DeviceContext* context)
    {
        UINT stride = sizeof(DXVertexPosColor);
        UINT offset = 0;
        if(m_vertexBuffer)
            context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
        if(m_indexBuffer)
            context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);

        // Set primitive topology
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        /*if(m_vShader)
            context->VSSetShader(m_vShader, nullptr, 0);
        if(m_constantBuffer)
            context->VSSetConstantBuffers(0, 1, &m_constantBuffer);
        if(m_pShader)
            context->PSSetShader(m_pShader, nullptr, 0);*/
        context->DrawIndexed(36, 0, 0);
        
    }

    void DXPrimitiveCube::SetVertexShader(ID3D11VertexShader* vs)
    {
        m_vShader = vs;
    }

    void DXPrimitiveCube::SetPixelShader(ID3D11PixelShader* ps)
    {
        m_pShader = ps;
    }

    void DXPrimitiveCube::SetConstantBuffer(ID3D11Buffer* cb)
    {
        m_constantBuffer = cb;
    }

}