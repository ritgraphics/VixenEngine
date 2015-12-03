#ifndef VIX_EMITTERCOMPONENT_H
#define VIX_EMITTERCOMPONENT_H

#include <vix_platform.h>
#include <vix_component.h>
#include <vix_rendercomponent.h>
#include <vix_time.h>
#include <vix_particleemitter.h>

namespace Vixen {

	class GameObject;

	class VIX_API EmitterComponent : public IRenderComponent, public Component
	{
	public:
		EmitterComponent();

		void VOnInit();

		void VOnEnable();

		void VUpdate();

		void VOnDisable();

		void VOnDestroy();

		void VBindParent(GameObject* parent);

		void VRender(ICamera3D* camera);
		
		Emitter*			GetEmitter();

		void						SetEmitter(Emitter * emitter);

	private:
		Emitter*			m_emitter;
		GameObject*					m_parent;
	};

}

#endif