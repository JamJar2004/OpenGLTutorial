#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <GL/glew.h>

class Image
{
private:
	int m_width;
	int m_height;
	int m_channels;

	uint8_t* m_pixels;
public:
	Image(const std::string& fileName);

	~Image();

	int GetWidth()  const { return m_width; }
	int GetHeight() const { return m_height; }

	int GetChannels() const { return m_channels; }

	uint8_t* GetPixels() const { return m_pixels; }
};

class Texture
{
private:
	GLuint m_ID;
public:
	static std::shared_ptr<Texture> LoadCubeMap(const std::string& left,
										        const std::string& right,
										        const std::string& bottom,
										        const std::string& top,
										        const std::string& back,
										        const std::string& front);

	static std::shared_ptr<Texture> Load(const std::string& fileName, GLint samplingMode = GL_LINEAR, GLint wrappingMode = GL_REPEAT);

	Texture(uint32_t width, uint32_t height, const uint8_t* pixels, GLenum internalFormat, GLenum format, GLint samplingMode = GL_LINEAR, GLint wrappingMode = GL_REPEAT);
	Texture(GLuint ID, uint32_t width, uint32_t height);

	~Texture();

	const uint32_t Width;
	const uint32_t Height;

	void Bind(uint32_t slot);
};