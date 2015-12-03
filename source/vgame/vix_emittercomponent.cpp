#include <vix_emittercomponent.h>
#include <vix_resourcemanager.h>
#include <vix_gameobject.h>

namespace Vixen {

	EmitterComponent::EmitterComponent()
		: Component(Component::Type::EMITTER)
	{
		m_emitter = nullptr;
	}

	void EmitterComponent::VOnInit()
	{

	}

	void EmitterComponent::VOnEnable()
	{

	}

	void EmitterComponent::VOnDisable()
	{

	}

	void EmitterComponent::VOnDestroy()
	{

	}

	void EmitterComponent::VUpdate()
	{

	}

	void EmitterComponent::VRender(ICamera3D* camera)
	{
		if (m_emitter) {
			m_emitter->VRenderSpawn(Time::DeltaTime(), Time::TotalTime());
			m_emitter->VRender(Time::DeltaTime(), Time::TotalTime(), camera);
		}
	}

	void EmitterComponent::VBindParent(GameObject* parent)
	{
		m_parent = parent;
	}

	Emitter* EmitterComponent::GetEmitter()
	{
		return m_emitter;
	}

	void EmitterComponent::SetEmitter(Emitter* emitter)
	{
		m_emitter = emitter;
	}
}