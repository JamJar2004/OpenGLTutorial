#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

std::shared_ptr<Texture> Texture::Load(const std::string& fileName, GLint samplingMode, GLint wrappingMode)
{
	stbi_set_flip_vertically_on_load(true);

	int width;
	int height;
	int channels;
	uint8_t* pixelData = stbi_load(("./res/textures/" + fileName).c_str(), &width, &height, &channels, 0);

	GLenum internalFormat;
	GLenum format;

	switch(channels)
	{
	case 3:
		internalFormat = GL_RGB8;
		format = GL_RGB;
		break;
	case 4:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		break;
	}

	return std::make_shared<Texture>(width, height, pixelData, internalFormat, format, samplingMode, wrappingMode);
}

Texture::Texture(uint32_t width, uint32_t height, const uint8_t* pixels,
	GLenum internalFormat, GLenum format, 
	GLint samplingMode, GLint wrappingMode) :
	Width(width), Height(height)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingMode);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind(uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}