/*
    The MIT License(MIT)

    Copyright(c) 2015 Matt Guerrette

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

#ifndef VIX_RESOURCELOADER_H
#define VIX_RESOURCELOADER_H

#include <vix_platform.h>
#include <vix_file.h>
#include <vix_texture.h>
#include <vix_shader.h>
#include <vix_model.h>
#include <vix_material.h>
#include <vix_font.h>
#include <vix_particleemitter.h>

namespace Vixen {

    class VIX_API IResourceLoader
    {
    public:
        virtual ~IResourceLoader() { }

        virtual Texture*	LoadTexture(File* file) = 0;
        virtual Shader*		LoadShader(File* file, ShaderType type) = 0;
        virtual Model*		LoadModel(File* file) = 0;
        virtual Font*		LoadFont(File* file) = 0;
		virtual Material*	LoadMaterial(File* file) = 0;
		virtual Emitter*	LoadEmitter(File* file) = 0;
    };

}

#endif
