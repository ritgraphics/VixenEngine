#include <vix_uitext.h>
#include <vix_renderer_singleton.h>
#include <vix_resourcemanager.h>
#include <vix_gameobject.h>>

namespace Vixen {

    UIText::UIText() 
		: Component(Type::UI_TEXT)
    {
        m_text = "";
        m_font = NULL;
		m_parent = NULL;
    }

    UIText::UIText(std::string text) 
		: Component(Type::UI_TEXT)
    {
        m_text = text;
        m_font = NULL;
		m_parent = NULL;
    }


    UIText::UIText(std::string text, Font* font)
		: Component(Component::Type::UI_TEXT)
    {
        m_text = text;
        m_font = font;
        m_parent = NULL;
    }

    UIText::~UIText()
    {
        ResourceManager::DecrementAssetRef(m_font);
    }

    void UIText::SetText(std::string text)
    {
        m_text = text.c_str();
    }

    std::string UIText::GetText()
    {
        return m_text;
    }

    std::string UIText::GetTextW()
    {
        return m_text;
    }

    Font* UIText::GetFont()
    {
        return m_font;
    }

	void UIText::VOnInit()
	{

	}

	void UIText::VOnEnable()
	{

	}
	
	void UIText::VUpdate()
	{

	}

	void UIText::VOnDisable()
	{

	}

	void UIText::VOnDestroy()
	{

	}

	void UIText::VBindParent(GameObject* parent)
	{
		m_parent = parent;
        m_parent->MarkForLateRender();
	}

    void UIText::VRender(ICamera2D* camera, Viewport v)
    {
		OrthoRect rect;
		rect.left = 0.0f;
		rect.top = 0.0f;
		rect.right = v.width;
		rect.bottom = v.height;

		camera->VSetOrthoLHOffCenter(rect, 0.0f, 1.0f);

        if(m_font && m_parent)
            Renderer::Render2DText(m_font, 
                                   m_text, 
                                   Vector2(m_parent->GetTransform()->X(),
										   m_parent->GetTransform()->Y()));
    }
}
