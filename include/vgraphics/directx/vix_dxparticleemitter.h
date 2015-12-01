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

#ifndef VIX_DXPARTICLEEMITTER_H
#define VIX_DXPARTICLEEMITTER_H

#include <vix_directx.h>
#include <vix_dxspritebatcher.h>
#include <vix_dxcamera3d.h>
#include <vix_particleemitter.h>


namespace Vixen {

	class VIX_API DXParticleEmitter : IParticleEmitter {
	public:
		DXParticleEmitter(float ageToSpawn, float maxLifetime, Vector3 constAccel, ParticleSettings settings);
		~DXParticleEmitter();
		bool Init(ID3D11Device* device, 
			ID3D11DeviceContext* context, 
			UString spawnVS, 
			UString spawnGS, 
			UString particleVS, 
			UString particleGS, 
			UString particlePS);
		void VRenderSpawn(float dt, float totalTime);
		void VRender(float dt, float totalTime, ICamera3D* camera);


	private:
		ID3D11Device*                       m_device;
		ID3D11DeviceContext*                m_context;
		ID3D11Buffer*						m_vBuffer;
		ID3D11Buffer*						m_soBufferRead;
		ID3D11Buffer*						m_soBufferWrite;
		DXVertexShader*						m_particleVS;
		DXPixelShader*						m_particlePS;
		DXGeometryShader*					m_particleGS;
		DXVertexShader*						m_spawnVS;
		DXGeometryShader*					m_spawnGS;
		ID3D11Texture1D*					m_randomTexture;
		ID3D11ShaderResourceView*			m_randomSRV;
		ID3D11ShaderResourceView*			m_particleTexture;
		ID3D11BlendState*					m_particleBlendState;
		ID3D11DepthStencilState*			m_particleDepthState;

		bool								m_initialized;
		bool								m_isFirstFrame;
	};
}

#endif