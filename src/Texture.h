#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "Utility.hpp"
#include <GL/glew.h>
#include <filesystem>

constexpr int RGBA = 4;

class Texture
{
private:
	unsigned int m_rendererId = 0;

	std::filesystem::path m_filePath;

	unsigned char *m_localBuffer = nullptr;
	
	int m_width = 0, m_height = 0, m_bpp = 0;

public:
	Texture(const std::filesystem::path& path);

	~Texture();

	void Bind(unsigned int slot) const;

	void Unbind() const;

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
};

#endif