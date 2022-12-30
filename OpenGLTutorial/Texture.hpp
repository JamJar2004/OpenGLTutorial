#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <GL/glew.h>

class Texture
{
private:
	GLuint m_ID;
public:
	static std::shared_ptr<Texture> Load(const std::string& fileName, GLint samplingMode = GL_LINEAR, GLint wrappingMode = GL_REPEAT);

	Texture(uint32_t width, uint32_t height, const uint8_t* pixels, GLenum internalFormat, GLenum format, GLint samplingMode = GL_LINEAR, GLint wrappingMode = GL_REPEAT);

	~Texture();

	const uint32_t Width;
	const uint32_t Height;

	void Bind(uint32_t slot);
};