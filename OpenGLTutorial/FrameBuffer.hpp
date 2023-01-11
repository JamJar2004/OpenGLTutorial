#pragma once

#include <stdint.h>
#include <GL/glew.h>

#include "Texture.hpp"

class FrameBuffer
{
private:
	GLuint m_ID;

	GLuint m_renderBufferID;

	std::shared_ptr<Texture> m_colorAttachment;
	std::shared_ptr<Texture> m_depthAttachment;
public:
	FrameBuffer(uint32_t width, uint32_t height, bool colorAttachment, bool depthAttachment, GLenum samplingMode, GLenum wrappingMode);

	~FrameBuffer();

	const uint32_t Width;
	const uint32_t Height;

	std::shared_ptr<Texture> GetColorAttachment() const { return m_colorAttachment; }
	std::shared_ptr<Texture> GetDepthAttachment() const { return m_depthAttachment; }

	void Bind();
};

