#include "vix_dxparticleemitter.h"
#include "vix_dxtexture.h"

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
		int type = settingsElement->IntAttribute("type");
		float age = settingsElement->FloatAttribute("age");
		float ageToSpawn = settingsElement->FloatAttribute("ageToSpawn");
		float maxLifetime = settingsElement->FloatAttribute("maxLifetime");
		Vector3 startPosition = VectorForXmlElement(settingsElement->FirstChildElement("startPosition"));
		Vector3 startVelocity = VectorForXmlElement(settingsElement->FirstChildElement("startVelocity"));
		Vector3 startMidEndSize = VectorForXmlElement(settingsElement->FirstChildElement("startMidEndSize"));
		Vector3 constantAcceleration = VectorForXmlElement(settingsElement->FirstChildElement("constantAcceleration"));

		XMLElement* colorsElement = settingsElement->FirstChildElement("colors");
		if (!colorsElement) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing colors"), file->BaseName().c_str());
			return false;
		}
		Color startColor = ColorForXmlElement(colorsElement->FirstChildElement("start-color"));
		Color midColor = ColorForXmlElement(colorsElement->FirstChildElement("mid-color"));
		Color endColor = ColorForXmlElement(colorsElement->FirstChildElement("end-color"));

		UString spawnVSPath = ShaderPathFromXmlElement(file, emtElement->FirstChildElement("vertex-shader"));
		UString spawnGSPath = ShaderPathFromXmlElement(file, emtElement->FirstChildElement("geometry-shader"));

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
		UString particleTexturePath = UStringFromCharArray(textureElement->Attribute("file"));
		UString particleVSPath = ShaderPathFromXmlElement(file, particleElement->FirstChildElement("vertex-shader"));
		UString particleGSPath = ShaderPathFromXmlElement(file, particleElement->FirstChildElement("geometry-shader"));
		UString particlePSPath = ShaderPathFromXmlElement(file, particleElement->FirstChildElement("pixel-shader"));
		
		//Set properties
		m_settings.type = type;
		m_settings.age = age;
		m_settings.startColor = startColor;
		m_settings.midColor = midColor;
		m_settings.endColor = endColor;
		m_settings.startPosition = startPosition;
		m_settings.startVelocity = startVelocity;
		m_settings.startMidEndSize = startMidEndSize;

		m_ageToSpawn = ageToSpawn;
		m_maxLifetime = maxLifetime;
		m_constAccel = constantAcceleration;

		if (!LoadShaders(spawnVSPath, spawnGSPath, particleVSPath, particleGSPath, particlePSPath)) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, invalid shaders"), file->BaseName().c_str());
			return false;
		}

		m_particleTexture = ResourceManager::OpenTexture(particleTexturePath);

		return Init();
	}

	Vector3 DXEmitter::VectorForXmlElement(tinyxml2::XMLElement* element)
	{
		if (!element)
			return Vector3::Zero;
		float _x = element->FloatAttribute("x");
		float _y = element->FloatAttribute("y");
		float _z = element->FloatAttribute("z");
		return Vector3(_x, _y, _z);
	}

	Color DXEmitter::ColorForXmlElement(tinyxml2::XMLElement* element)
	{
		if (!element)
			return Colors::Black;

		const char * _name = element->Attribute("name");
		if (_name) {
			return Colors::ColorFromUString(UStringFromCharArray(_name));
		}

		float _r = -1.0f;
		element->QueryFloatAttribute("r", &_r);
		float _g = -1.0f;
		element->QueryFloatAttribute("g", &_g);
		float _b = -1.0f;
		element->QueryFloatAttribute("b", &_b);
		float _a = 1.0f;
		element->QueryFloatAttribute("a", &_a);
		if (_r != -1.0f && _g != -1.0f && _b != -1.0f) {
			return Color(_r, _g, _b, _a);
		}

		return Colors::Black;
	}

	UString DXEmitter::ShaderPathFromXmlElement(File* file, tinyxml2::XMLElement* element)
	{
		if (!element) {
			DebugPrintF(VTEXT("Vixen Emitter File: %s, missing shader element"), file->BaseName().c_str());
			return false;
		}
		return UStringFromCharArray(element->Attribute("val"));
	}

	bool DXEmitter::LoadShaders(UString spawnVSPath, UString spawnGSPath, UString particleVSPath, UString particleGSPath, UString particlePSPath)
	{
		m_spawnVS = (DXVertexShader*)ResourceManager::OpenShader(spawnVSPath, ShaderType::VERTEX_SHADER);
		m_spawnGS = (DXGeometryShader*)ResourceManager::OpenShader(spawnGSPath, ShaderType::GEOMETRY_SHADER);
		m_particleVS = (DXVertexShader*)ResourceManager::OpenShader(particleVSPath, ShaderType::VERTEX_SHADER);
		m_particleGS = (DXGeometryShader*)ResourceManager::OpenShader(particleGSPath, ShaderType::GEOMETRY_SHADER);
		m_particlePS = (DXPixelShader*)ResourceManager::OpenShader(particlePSPath, ShaderType::PIXEL_SHADER);
		return m_spawnVS && m_spawnGS && m_particleVS && m_particleGS && m_particlePS;
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