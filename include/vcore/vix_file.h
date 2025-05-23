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

#ifndef VIX_FILE_H
#define VIX_FILE_H

#include <filesystem>

#include <vix_platform.h>

#include <SDL3/SDL.h>

namespace Vixen
{
    struct SDLIOStreamDeleter
    {
        void operator()(SDL_IOStream* stream) const
        {
            if (stream)
            {
                SDL_CloseIO(stream);
                stream = nullptr;
            }
        }
    };
    using SDLIOStreamPtr = std::unique_ptr<SDL_IOStream, SDLIOStreamDeleter>;

    class VIX_API File
    {
    public:
        explicit File(const char* path);

        std::string          FileName();
        std::string          FilePath();
        std::string          BaseName();
        std::string          Extension();
        std::vector<uint8_t> ReadAllBytes();

    private:
        std::filesystem::path m_filePath;
        SDLIOStreamPtr        m_stream;
    };

} // namespace Vixen

#endif
