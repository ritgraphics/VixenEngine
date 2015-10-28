#ifndef VIX_UITEXT_H
#define VIX_UITEXT_H

#include <vix_platform.h>
#include <vix_component.h>
#include <vix_rendercomponent2D.h>
#include <vix_gameobject.h>
#include <vix_stringutil.h>

namespace Vixen {

    class VIX_API UIText : public IRenderComponent2D, public IComponent
    {
    public:
        UIText();
        UIText(UString text);
        UIText(UString text, IFont* font);

        ~UIText();

		void VOnInit();
		void VOnEnable();
		void VUpdate();
		void VOnDisable();
		void VOnDestroy();
        void VRender(ICamera2D* camera);
		void VBindParent(GameObject* parent);

		void VSetType(IComponent::Type type);
		IComponent::Type VGetType();

        std::string GetText();
        void        SetText(std::string text);

    private:
        UString     m_text;
        IFont*      m_font;
		GameObject* m_parent;

		IComponent::Type m_type;
    };
}

#endif