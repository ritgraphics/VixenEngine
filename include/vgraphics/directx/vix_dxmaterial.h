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

#ifndef VIX_DXMATERIAL_H
#define VIX_DXMATERIAL_H

#include <vix_platform.h>
#include <vix_material.h>
#include <vix_dxtexture.h>
#include <vix_dxshader.h>
#include <vix_shadervariable.h>
#include <vix_dxvertexshader.h>
#include <vix_dxgeometryshader.h>
#include <vix_dxpixelshader.h>
#include <vix_tinyxml.h>

namespace Vixen {

	

    class VIX_API DXMaterial : public Material
    {
    public:
        DXMaterial();

        ~DXMaterial();

        void VBind();
        void VUnbind();

		DXVertexShader* GetVertexShader();
		

		bool VInitFromFile(File* file);

		UString VFilePath();

    private:
        std::map<ShaderRole,  DXShader*>		m_shaders;
		UString									m_path;

	private:
    };

}

#endif