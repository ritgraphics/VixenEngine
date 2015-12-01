#include "vix_particleemitter.h"

namespace Vixen {
	IParticleEmitter::IParticleEmitter(float ageToSpawn, float maxLifetime, Vector3 constAccel, ParticleSettings settings)
	{
		this->m_ageToSpawn = ageToSpawn;
		this->m_maxLifetime = maxLifetime;
		this->m_constAccel = constAccel;
		this->m_settings = settings;
	}


	IParticleEmitter::~IParticleEmitter()
	{
	}
}