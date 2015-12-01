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

#include <vix_dxgeometryshader.h>

namespace Vixen {

	DXGeometryShader::DXGeometryShader(ID3D11Device* device, ID3D11DeviceContext* context,
		bool useStreamOut,
		bool allowStreamOutRasterization)
		: DXShader(device, context, ShaderType::GEOMETRY_SHADER)
	{
		m_shader = nullptr;
		m_inputLayout = nullptr;
		m_useStreamOut = useStreamOut;
		m_allowStreamOutRasterization = allowStreamOutRasterization;
	}

	DXGeometryShader::~DXGeometryShader()
	{
		ReleaseCOM(m_shader);
		ReleaseCOM(m_inputLayout);
	}

	bool DXGeometryShader::VInitShader(File* file)
	{
		HRESULT hr = S_OK;

		BYTE* data = new BYTE[VIX_LARGE_BUFSIZE];
		file->Seek(0, FileSeek::End);
		size_t _size = file->Tell();
		file->Seek(0, FileSeek::Set);
		file->Read(data, _size); //read all of the file into memory

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3DBlob* errorBlob = nullptr;
		hr = D3DCompile2(data, _size, nullptr, nullptr,
			nullptr, "main", "gs_5_0", dwShaderFlags, NULL, NULL, NULL, NULL,
			&m_shaderBlob, &errorBlob);
		//hr = D3DReadFileToBlob(file->FilePath().c_str(), &m_shaderBlob);
		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
				errorBlob->Release();
			}
			return false;
		}
		if (errorBlob) errorBlob->Release();

		// Create the geometry shader
		hr = m_device->CreateGeometryShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, &m_shader);
		if (FAILED(hr))
		{
			ReleaseCOM(m_shaderBlob);
			return false;
		}

		//Reflect shader info
		ID3D11ShaderReflection* shaderReflection;
		D3DReflect(m_shaderBlob->GetBufferPointer(),
			m_shaderBlob->GetBufferSize(),
			__uuidof(ID3D11ShaderReflection),
			reinterpret_cast<void**>(&shaderReflection));

		// Get shader info
		D3D11_SHADER_DESC shaderDesc;
		shaderReflection->GetDesc(&shaderDesc);

		// Read input layout description from shader info
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (unsigned int i = 0; i< shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			shaderReflection->GetInputParameterDesc(i, &paramDesc);

			// Fill out input element desc
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// Determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			// Save element desc
			inputLayoutDesc.push_back(elementDesc);
		}

		// Try to create Input Layout
		hr = m_device->CreateInputLayout(
			&inputLayoutDesc[0],
			inputLayoutDesc.size(),
			m_shaderBlob->GetBufferPointer(),
			m_shaderBlob->GetBufferSize(),
			&m_inputLayout);

		if (FAILED(hr))
			return false;


		return true;
	}

	void DXGeometryShader::VBind()
	{
		m_context->IASetInputLayout(m_inputLayout);
		m_context->GSSetShader(m_shader, 0, 0);

		for (size_t i = 0; i < m_cbCount; i++)
		{
			m_context->GSSetConstantBuffers(m_cbArray[i].BindIndex,
				1, &m_cbArray[i].Buffer);
		}
	}

	void DXGeometryShader::VUnbind()
	{
		m_context->GSSetShader(0, 0, 0);
	}

	bool DXGeometryShader::VSetShaderResourceView(std::string name, ID3D11ShaderResourceView* rv)
	{
		//find variable
		size_t index = FindTextureBindIndex(name);
		if (index == -1)
			return false;

		//set resource view
		m_context->GSSetShaderResources(index, 1, &rv);

		return true;
	}

	bool DXGeometryShader::VSetSamplerState(std::string name, ID3D11SamplerState* ss)
	{
		//find variable
		size_t index = FindSampleBindIndex(name);
		if (index == -1)
			return false;

		//set sampler
		m_context->GSSetSamplers(index, 1, &ss);

		return true;
	}

	bool DXGeometryShader::CreateCompatibleStreamOutBuffer(ID3D11Buffer** buffer, int vertexCount)
	{
		// Was stream output actually used?
		if (!this->m_useStreamOut || m_streamOutVertexSize == 0)
			return false;

		// Set up the buffer description
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_STREAM_OUTPUT | D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = m_streamOutVertexSize * vertexCount;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;

		// Attempt to create the buffer and return the result
		HRESULT result = this->m_device->CreateBuffer(&desc, 0, buffer);
		return (result == S_OK);
	}

	void DXGeometryShader::UnbindStreamOutStage(ID3D11DeviceContext* deviceContext)
	{
		unsigned int offset = 0;
		ID3D11Buffer* unset[1] = { 0 };
		deviceContext->SOSetTargets(1, unset, &offset);
	}

	ID3D11GeometryShader* DXGeometryShader::GetShader()
	{
		return m_shader;
	}

	ID3D11InputLayout* DXGeometryShader::GetLayout()
	{
		return m_inputLayout;
	}

}