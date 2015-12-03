#include "vix_dxparticleemitter.h"
#include "vix_dxtexture.h"
#include "vix_resourcemanager.h"

namespace Vixen {
	DXEmitter::DXEmitter(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		this->m_device = device;
		this->m_context = context;
		this->m_initialized = false;
	}
	
	bool DXEmitter::Init()
	{
		if (m_initialized)
			return false;
		
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

		unsigned int randomTextureWidth = 1024;

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

		// Create the sampler
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		m_device->CreateSamplerState(&samplerDesc, &m_sampler);

		m_spawnGS->CreateCompatibleStreamOutBuffer(&m_soBufferRead, 1000000);
		m_spawnGS->CreateCompatibleStreamOutBuffer(&m_soBufferWrite, 1000000);
		
		m_initialized = true;
		return true;
	}

	DXEmitter::~DXEmitter()
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
		this->m_particleTexture->DecrementRefCount();
		this->m_sampler->Release();
	}
	
	bool DXEmitter::VInitFromFile(File* file)
	{
		if (!file)
			return false;

		using namespace tinyxml2;

		auto VectorForElement = [](XMLElement* element) -> Vector3 {
			if (!element)
				return Vector3::Zero;
			float _x = element->FloatAttribute("x");
			float _y = element->FloatAttribute("y");
			float _z = element->FloatAttribute("z");
			return Vector3(_x, _y, _z);
		};

		auto ColorForElement = [](XMLElement* element) -> Color {
			if (!element)
				return Colors::Black;
			const char * _name = element->Attribute("name");
			float _r = -1.0f; 
			element->QueryFloatAttribute("r", &_r);
			float _g = -1.0f; 
			element->QueryFloatAttribute("g", &_g);
			float _b = -1.0f; 
			element->QueryFloatAttribute("b", &_b);
			float _a = 1.0f; 
			element->QueryFloatAttribute("a", &_a);
			if (_name) {
				return Color::FromUString(UStringFromCharArray(_name));
			}
			else if (_r != -1.0f && _g != -1.0f && _b != -1.0f) {
				return Color(_r, _g, _b, _a);
			}
			else {
				return Colors::Black;
			}
		};

		XMLDOC document;
		XMLError err = document.LoadFile(file->Handle());
		UString errString;
		if (XMLErrCheck(err, errString))
		{
			DebugPrintF(VTEXT("Vixen Emitter File: %s failed to load\n"), file->BaseName().c_str());
			return false;
		}

		XMLElement* emtElement = document.FirstChildElement("emitter");
		XMLElement* settingsElement = emtElement->FirstChildElement("settings");
		if (!settingsElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing settings"), file->BaseName().c_str());
			return false;
		}
		int _type = settingsElement->IntAttribute("type");
		float _age = settingsElement->FloatAttribute("age");
		float _ageToSpawn = settingsElement->FloatAttribute("ageToSpawn");
		float _maxLifetime = settingsElement->FloatAttribute("maxLifetime");
		Vector3 _startPosition = VectorForElement(settingsElement->FirstChildElement("startPosition"));
		Vector3 _startVelocity = VectorForElement(settingsElement->FirstChildElement("startVelocity"));
		Vector3 _startMidEndSize = VectorForElement(settingsElement->FirstChildElement("startMidEndSize"));
		Vector3 _constantAcceleration = VectorForElement(settingsElement->FirstChildElement("constantAcceleration"));

		XMLElement* colorsElement = settingsElement->FirstChildElement("colors");
		if (!colorsElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing colors"), file->BaseName().c_str());
			return false;
		}
		Color _startColor = ColorForElement(colorsElement->FirstChildElement("start-color"));
		Color _midColor = ColorForElement(colorsElement->FirstChildElement("mid-color"));
		Color _endColor = ColorForElement(colorsElement->FirstChildElement("end-color"));

		XMLElement* spawnVSElement = emtElement->FirstChildElement("vertex-shader");
		if (!spawnVSElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing spawn vertex shader"), file->BaseName().c_str());
			return false;
		}
		UString _spawnVSPath = UStringFromCharArray(spawnVSElement->Attribute("val"));

		XMLElement* spawnGSElement = emtElement->FirstChildElement("geometry-shader");
		if (!spawnGSElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing spawn geometry shader"), file->BaseName().c_str());
			return false;
		}
		UString _spawnGSPath = UStringFromCharArray(spawnGSElement->Attribute("val"));


		XMLElement* particleElement = emtElement->FirstChildElement("particle");
		if (!particleElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing particle"), file->BaseName().c_str());
			return false;
		}

		XMLElement* textureElement = particleElement->FirstChildElement("texture");
		if (!textureElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing particle texture"), file->BaseName().c_str());
			return false;
		}
		UString _particleTexturePath = UStringFromCharArray(textureElement->Attribute("file"));

		XMLElement* particleVSElement = particleElement->FirstChildElement("vertex-shader");
		if (!particleVSElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing particle vertex shader"), file->BaseName().c_str());
			return false;
		}
		UString _particleVSPath = UStringFromCharArray(particleVSElement->Attribute("val"));

		XMLElement* particleGSElement = particleElement->FirstChildElement("geometry-shader");
		if (!particleGSElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing particle geometry shader"), file->BaseName().c_str());
			return false;
		}
		UString _particleGSPath = UStringFromCharArray(particleGSElement->Attribute("val"));

		XMLElement* particlePSElement = particleElement->FirstChildElement("pixel-shader");
		if (!particlePSElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing particle pixel shader"), file->BaseName().c_str());
			return false;
		}
		UString _particlePSPath = UStringFromCharArray(particlePSElement->Attribute("val"));


		//Set properties
		m_settings.type = _type;
		m_settings.age = _age;
		m_settings.startColor = _startColor;
		m_settings.midColor = _midColor;
		m_settings.endColor = _endColor;
		m_settings.startPosition = _startPosition;
		m_settings.startVelocity = _startVelocity;
		m_settings.startMidEndSize = _startMidEndSize;

		m_ageToSpawn = _ageToSpawn;
		m_maxLifetime = _maxLifetime;
		m_constAccel = _constantAcceleration;

		m_spawnVS = (DXVertexShader*)ResourceManager::OpenShader(_spawnVSPath, ShaderType::VERTEX_SHADER);
		m_spawnGS = (DXGeometryShader*)ResourceManager::OpenShader(_spawnGSPath, ShaderType::GEOMETRY_SHADER);
		m_particleVS = (DXVertexShader*)ResourceManager::OpenShader(_particleVSPath, ShaderType::VERTEX_SHADER);
		m_particleGS = (DXGeometryShader*)ResourceManager::OpenShader(_particleGSPath, ShaderType::GEOMETRY_SHADER);
		m_particlePS = (DXPixelShader*)ResourceManager::OpenShader(_particlePSPath, ShaderType::PIXEL_SHADER);

		m_particleTexture = ResourceManager::OpenTexture(_particleTexturePath);

		return Init();
	}

	void DXEmitter::VRenderSpawn(float dt, float totalTime)
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
		m_spawnGS->VSetSamplerState("randomSampler", m_sampler);
		m_spawnGS->VSetShaderResourceView("randomTexture", m_randomSRV);

		m_spawnVS->Activate();
		m_spawnGS->Activate();
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

	void DXEmitter::VRender(ICamera3D* camera)
	{
		if (!m_initialized)
			return;

		m_particleGS->SetMatrix4x4("world", DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)); // Identity
		m_particleGS->SetMatrix4x4("view", ((DXCamera3D*)camera)->View());
		m_particleGS->SetMatrix4x4("projection", ((DXCamera3D*)camera)->Projection());
		
		m_particleVS->SetFloat3("acceleration", DirectX::XMFLOAT3(m_constAccel.X(), m_constAccel.Y(), m_constAccel.Z()));
		m_particleVS->VSetFloat("maxLifetime", m_maxLifetime);
		
		m_particlePS->VSetSamplerState("trilinear", m_sampler);
		m_particlePS->VSetShaderResourceView("particleTexture", ((DXTexture*)m_particleTexture)->ResourceView());
		
		m_particleVS->Activate(true);
		m_particlePS->Activate(true);
		m_particleGS->Activate(true);

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