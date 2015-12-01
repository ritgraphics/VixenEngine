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

#ifndef VIX_DXGEOMETRYSHADER_H
#define VIX_DXGEOMETRYSHADER_H

#include <vix_platform.h>
#include <vix_dxshader.h>

namespace Vixen {

	class VIX_API DXGeometryShader : public DXShader
	{
	public:
		DXGeometryShader(ID3D11Device* device,
			ID3D11DeviceContext* context,
			bool useStreamOut = 0, 
			bool allowStreamOutRasterization = 0);

		~DXGeometryShader();


		ID3D11GeometryShader* GetShader();
		ID3D11InputLayout*  GetLayout();

		bool VSetShaderResourceView(std::string name, ID3D11ShaderResourceView* rv);
		bool VSetSamplerState(std::string name, ID3D11SamplerState* ss);

		bool CreateCompatibleStreamOutBuffer(ID3D11Buffer** buffer, int vertexCount);

		static void UnbindStreamOutStage(ID3D11DeviceContext* deviceContext);

	protected:
		bool VInitShader(File* file) override;
		void VBind() override;
		void VUnbind() override;

	private:
		ID3D11GeometryShader*		m_shader;
		ID3D11InputLayout*			m_inputLayout;

		// Stream out related
		bool						m_useStreamOut;
		bool						m_allowStreamOutRasterization;
		unsigned int				m_streamOutVertexSize;
	};

}

#endif