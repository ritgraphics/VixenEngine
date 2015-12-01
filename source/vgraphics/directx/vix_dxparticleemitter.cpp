#include "vix_dxparticleemitter.h"

namespace Vixen {
	DXParticleEmitter::DXParticleEmitter(float ageToSpawn, float maxLifetime, Vector3 constAccel, ParticleSettings settings)
		: IParticleEmitter(ageToSpawn, maxLifetime, constAccel, settings)
	{
	}

	bool DXParticleEmitter::Init(ID3D11Device* device, ID3D11DeviceContext* context, UString spawnVS, UString spawnGS, UString particleVS, UString particleGS, UString particlePS)
	{
		if (m_initialized)
			return false;

		this->m_device = device;
		this->m_context = context;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof(ParticleSettings) * 1;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA initialVertexData;
		initialVertexData.pSysMem = &m_settings;
		m_device->CreateBuffer(&vbd, &initialVertexData, &m_vBuffer);

		/*unsigned int randomTextureWidth = 1024;

		// Random data for the 1D texture
		srand((unsigned int)time(0));
		std::vector<float> data(randomTextureWidth * 4);
		for (unsigned int i = 0; i < randomTextureWidth * 4; i++)
			data[i] = rand() / (float)RAND_MAX * 2.0f - 1.0f;

		// Set up texture
		D3D11_TEXTURE1D_DESC textureDesc;
		textureDesc.ArraySize = 1;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.MipLevels = 1;
		textureDesc.MiscFlags = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.Width = 100;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = (void*)&data[0];
		initData.SysMemPitch = randomTextureWidth * sizeof(float) * 4;
		initData.SysMemSlicePitch = 0;
		m_device->CreateTexture1D(&textureDesc, &initData, &m_randomTexture);

		// Set up SRV for texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		srvDesc.Texture1D.MipLevels = 1;
		srvDesc.Texture1D.MostDetailedMip = 0;
		m_device->CreateShaderResourceView(m_randomTexture, &srvDesc, &m_randomSRV);
		*/

		m_spawnVS = (DXVertexShader*)ResourceManager::OpenShader(spawnVS, ShaderType::VERTEX_SHADER);
		m_spawnGS = (DXGeometryShader*)ResourceManager::OpenShader(spawnGS, ShaderType::GEOMETRY_SHADER);
		m_particleVS = (DXVertexShader*)ResourceManager::OpenShader(particleVS, ShaderType::VERTEX_SHADER);
		m_particleGS = (DXGeometryShader*)ResourceManager::OpenShader(particleGS, ShaderType::GEOMETRY_SHADER);
		m_particlePS = (DXPixelShader*)ResourceManager::OpenShader(particlePS, ShaderType::PIXEL_SHADER);

		m_spawnGS->CreateCompatibleStreamOutBuffer(&m_soBufferRead, 1000000);
		m_spawnGS->CreateCompatibleStreamOutBuffer(&m_soBufferWrite, 1000000);

		//load particle texture

		m_initialized = true;
		return true;
	}

	DXParticleEmitter::~DXParticleEmitter()
	{
		this->m_vBuffer->Release();
		this->m_spawnVS->DecrementRefCount();
		this->m_spawnGS->DecrementRefCount();
		this->m_particleVS->DecrementRefCount();
		this->m_particleGS->DecrementRefCount();
		this->m_particlePS->DecrementRefCount();
		this->m_randomSRV->Release();
		this->m_randomTexture->Release();
		this->m_soBufferRead->Release();
		this->m_soBufferWrite->Release();
		this->m_particleBlendState->Release();
		this->m_particleDepthState->Release();
	}
	
	void DXParticleEmitter::VRenderSpawn(float dt, float totalTime) 
	{
		if (!m_initialized)
			return;

		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		UINT stride = sizeof(ParticleSettings);
		UINT offset = 0;

		m_spawnGS->VSetFloat("dt", dt);
		m_spawnGS->VSetFloat("ageToSpawn", m_ageToSpawn);
		m_spawnGS->VSetFloat("maxLifetime", m_maxLifetime);
		m_spawnGS->VSetFloat("totalTime", totalTime);
		//m_spawnGS->VSetSamplerState("randomSampler", m_sampler);
		//m_spawnGS->VSetShaderResourceView("randomTexture", m_randomSRV);

		//m_spawnVS->SetShader();
		//m_spawnGS->SetShader();
		m_context->PSSetShader(0, 0, 0);

		ID3D11Buffer* unset = 0;
		m_context->IASetVertexBuffers(0, 1, &unset, &stride, &offset);

		// First frame?
		if (m_isFirstFrame)
		{
			// Draw using the seed vertex
			m_context->IASetVertexBuffers(0, 1, &m_vBuffer, &stride, &offset);
			m_context->SOSetTargets(1, &m_soBufferWrite, &offset);
			m_context->Draw(1, 0);
			m_isFirstFrame = false;
		}
		else
		{
			// Draw using the buffers
			m_context->IASetVertexBuffers(0, 1, &m_soBufferRead, &stride, &offset);
			m_context->SOSetTargets(1, &m_soBufferWrite, &offset);
			m_context->DrawAuto();
		}

		// Unbind SO targets and shader
		DXGeometryShader::UnbindStreamOutStage(m_context);
		m_context->GSSetShader(0, 0, 0);

		// Swap after draw
		ID3D11Buffer* temp = m_soBufferRead;
		m_soBufferRead = m_soBufferWrite;
		m_soBufferWrite = temp;
	}

	void DXParticleEmitter::VRender(float dt, float totalTime, ICamera3D* camera)
	{
		m_particleGS->SetMatrix4x4("world", XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)); // Identity
		m_particleGS->SetMatrix4x4("view", ((DXCamera3D*)camera)->View());
		m_particleGS->SetMatrix4x4("projection", ((DXCamera3D*)camera)->Projection());
		
		m_particleVS->SetFloat3("acceleration", m_constAccel);
		m_particleVS->VSetFloat("maxLifetime", m_maxLifetime);
		
		m_particlePS->SetSamplerState("trilinear", sampler);
		m_particlePS->SetShaderResourceView("particleTexture", m_particleTexture);
		
		m_particleVS->SetShader(true);
		m_particlePS->SetShader(true);
		m_particleGS->SetShader(true);

		// Set up states
		float factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_context->OMSetBlendState(m_particleBlendState, factor, 0xffffffff);
		m_context->OMSetDepthStencilState(m_particleDepthState, 0);

		// Set buffers
		UINT particleStride = sizeof(ParticleSettings);
		UINT particleOffset = 0;
		m_context->IASetVertexBuffers(0, 1, &m_soBufferRead, &particleStride, &particleOffset);

		// Draw auto - draws based on current stream out buffer
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		m_context->DrawAuto();

		// Unset Geometry Shader for next frame and reset states
		m_context->GSSetShader(0, 0, 0);
		m_context->OMSetBlendState(0, factor, 0xffffffff);
		m_context->OMSetDepthStencilState(0, 0);
	}
}