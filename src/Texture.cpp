#include "Texture.h"

// include stb here
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture(const std::filesystem::path& path)
		: m_filePath(path)
	{
		stbi_set_flip_vertically_on_load(true);

		m_localBuffer = stbi_load(m_filePath.string().c_str(), &m_width, &m_height, &m_bpp, STBI_rgb_alpha);

		GLCall(glGenTextures(1, &m_rendererId));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

		// setup default texture settings
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		
		// repeat for going 3D test
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_localBuffer)
			stbi_image_free(m_localBuffer);
	}


Texture::~Texture() 
{ 
	GLCall(glDeleteTextures(1, &m_rendererId)); 
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
	// also there are a term - "Bindless Texture"
}

void Texture::Unbind() const 
{ 
	GLCall(glBindTexture(GL_TEXTURE_2D, 0)); 
}
