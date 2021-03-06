/*
	The MIT License(MIT)

	Copyright(c) 2015 Vixen Team, Matt Guerrette

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

#ifndef VIX_DXVERTEXBUFFER_H
#define VIX_DXVERTEXBUFFER_H

#include <vix_platform.h>
#include <vix_directx.h>
#include <vix_vertexbuffer.h>

namespace Vixen {

    class VIX_API DXVPCBuffer: public IVertexBuffer
    {
    public:
        DXVPCBuffer(size_t count, ID3D11Device* device, ID3D11DeviceContext* context);

        ~DXVPCBuffer();

        void VSetData(const void* data);
        void VUpdateSubData(size_t offset, size_t stride, size_t count, const void* data);
        void VBind();
        void VUnbind();

   
    private:
        ID3D11Device*           m_device;
        ID3D11DeviceContext*    m_context;
        ID3D11Buffer*           m_buffer;
    };

    class VIX_API DXVPTBuffer : public IVertexBuffer
    {
    public:
        DXVPTBuffer(size_t count, ID3D11Device* device, ID3D11DeviceContext* context);

        ~DXVPTBuffer();

        void VSetData(const void* data);
        void VUpdateSubData(size_t offset, size_t stride, size_t count, const void* data);
        void VBind();
        void VUnbind();

    private:
        ID3D11Device*          m_device;
        ID3D11DeviceContext*   m_context;
        ID3D11Buffer*          m_buffer;
    };

    class VIX_API DXVPTNBuffer : public IVertexBuffer
    {
    public:
        DXVPTNBuffer(size_t count, ID3D11Device* device, ID3D11DeviceContext* context);

        ~DXVPTNBuffer();

        void VSetData(const void* data);
        void VUpdateSubData(size_t offset, size_t stride, size_t count, const void* data);
        void VBind();
        void VUnbind();

    private:
        ID3D11Device*          m_device;
        ID3D11DeviceContext*   m_context;
        ID3D11Buffer*          m_buffer;
    };
}

#endif
