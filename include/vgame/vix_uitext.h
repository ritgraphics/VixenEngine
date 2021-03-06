#ifndef VIX_UITEXT_H
#define VIX_UITEXT_H

#include <vix_platform.h>
#include <vix_component.h>
#include <vix_rendercomponent2D.h>
#include <vix_stringutil.h>

namespace Vixen {

    class GameObject;

    class VIX_API UIText : public IRenderComponent2D, public Component
    {
    public:
        UIText();
        UIText(UString text);
        UIText(UString text, Font* font);

        ~UIText();

		void VOnInit();
		void VOnEnable();
		void VUpdate();
		void VOnDisable();
		void VOnDestroy();
        void VRender(ICamera2D* camera, Viewport v);
		void VBindParent(GameObject* parent);

        std::string GetText();
        UString     GetTextW();
        void        SetText(std::string text);

        Font*       GetFont();

    private:
        UString     m_text;
        Font*       m_font;
		GameObject* m_parent;
    };
}

#endif
