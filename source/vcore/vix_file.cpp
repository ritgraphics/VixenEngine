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

#include <vix_file.h>

#include <filesystem>

#include <fmt/format.h>

namespace Vixen {

	File::File(const char* path)
	{
        //const std::filesystem::path basepath = SDL_GetBasePath();
		//m_filePath = basepath / path;

		m_filePath = std::filesystem::path(path);

		SDL_IOStream* stream = SDL_IOFromFile(m_filePath.string().c_str(), "rb");
		if (!stream)
		{
			throw std::runtime_error(fmt::format("Failed to open file: {}", FileName()));
		}

		m_stream.reset(stream);
	}

	std::vector<uint8_t> File::ReadAllBytes()
	{
		SDL_SeekIO(m_stream.get(), 0, SDL_IO_SEEK_END);
		const auto numBytes = SDL_TellIO(m_stream.get());
		SDL_SeekIO(m_stream.get(), 0, SDL_IO_SEEK_SET);

		std::vector<uint8_t> bytes(numBytes);

		size_t numBytesRead;
		void*  data = SDL_LoadFile_IO(m_stream.get(), &numBytesRead, false);
		if (data == nullptr)
		{
			throw std::runtime_error(fmt::format("Failed to read file: {}", FileName()));
		}

		memcpy(bytes.data(), data, numBytesRead);
		SDL_free(data);

		return bytes;
	}

	
	std::string File::BaseName()
	{
		return m_filePath.stem().string();
	}

	std::string File::FileName()
	{
		return m_filePath.filename().string();
	}

	std::string File::FilePath()
	{
		return m_filePath.string();
	}
	
	std::string File::Extension()
	{
		return m_filePath.extension().string();
	}
}
