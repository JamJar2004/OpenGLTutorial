#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

std::shared_ptr<Texture> Texture::LoadCubeMap(const std::string& left,
											  const std::string& right,
											  const std::string& bottom,
											  const std::string& top,
											  const std::string& back,
											  const std::string& front)
{
	Image* faces[] =
	{
		new Image("./res/textures/" + right),
		new Image("./res/textures/" + left),
		new Image("./res/textures/" + top),
		new Image("./res/textures/" + bottom),
		new Image("./res/textures/" + back),
		new Image("./res/textures/" + front)
	};

	uint32_t width  = faces[0]->GetWidth();
	uint32_t height = faces[0]->GetHeight();

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	for(int i = 0; i < 6; i++)
	{
		Image* image = faces[i];
		uint8_t* pixels = image->GetPixels();
		if(image->GetChannels() == 4)
		{
			size_t offset = 0;
			pixels = new uint8_t[size_t(image->GetWidth()) * image->GetHeight() * 3];
			for(size_t i = 0; i < size_t(image->GetWidth()) * image->GetHeight() * image->GetChannels(); i += image->GetChannels())
			{
				pixels[offset    ] = image->GetPixels()[i];
				pixels[offset + 1] = image->GetPixels()[i + 1];
				pixels[offset + 2] = image->GetPixels()[i + 2];

				offset += 3;
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		delete image;
		if(image->GetChannels() == 4)
			delete pixels;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return std::make_shared<Texture>(id, width, height);
}

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

Texture::Texture(GLuint ID, uint32_t width, uint32_t height) :
	m_ID(ID), Width(width), Height(height)
{
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

Image::Image(const std::string& fileName)
{
	m_pixels = stbi_load(fileName.c_str(), &m_width, &m_height, &m_channels, 0);
	stbi__vertical_flip(m_pixels, m_width, m_height, m_channels);
}

Image::~Image()
{
	stbi_image_free(m_pixels);
}
