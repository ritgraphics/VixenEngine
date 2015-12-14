#include <vix_lightmanager.h>
#include <vix_renderer_singleton.h>
#include <vix_resourcemanager.h>

namespace Vixen {

	void LightManager::Initialize()
	{
		LightManager& _manager = LightManager::instance();

		_manager.m_pointLightModel = ResourceManager::OpenModel(VTEXT("pointlight.obj"));
		_manager.m_spotLightModel = ResourceManager::OpenModel(VTEXT("spotlight.obj"));
		_manager.m_lightMaterial = ResourceManager::OpenMaterial(VTEXT("Light.vmt"));
		_manager.m_lightMaterial->IncrementRefCount();
		_manager.m_pointLightModel->VSetMaterial(_manager.m_lightMaterial);
		_manager.m_pointLightModel->IncrementRefCount();
		_manager.m_spotLightModel->VSetMaterial(_manager.m_lightMaterial);
		_manager.m_spotLightModel->IncrementRefCount();
	}

	void LightManager::DeInitialize()
	{
		LightManager& _manager = LightManager::instance();
		
		ResourceManager::DecrementAssetRef(_manager.m_pointLightModel);
		ResourceManager::DecrementAssetRef(_manager.m_spotLightModel);
		ResourceManager::DecrementAssetRef(_manager.m_lightMaterial);
	}

	void LightManager::RegisterPointLight(PointLight* light, MATRIX* transform)
	{
		LightManager& _manager = LightManager::instance();

		_manager.m_pointLights.push_back(light);

		_manager.m_pointLightModel->VBatchRender(transform);
	}

	void LightManager::RegisterSpotLight(SpotLight* light, MATRIX* transform)
	{
		LightManager& _manager = LightManager::instance();

		_manager.m_spotLights.push_back(light);

		_manager.m_spotLightModel->VBatchRender(transform);
	}

	void LightManager::RenderLights(ICamera3D* camera)
	{
		LightManager& _manager = LightManager::instance();

		Renderer::RenderLights(camera, _manager.m_pointLightModel, _manager.m_pointLights);
		//Renderer::RenderLights(camera, _manager.m_spotLightModel, _manager.m_spotLights);
	}
	
	void LightManager::ClearLights()
	{
		LightManager& _manager = LightManager::instance();

		_manager.m_pointLights.clear();
		_manager.m_spotLights.clear();
	}
	Material* LightManager::GetMaterial()
	{
		return LightManager::instance().m_lightMaterial;
	}
}