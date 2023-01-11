#include "FrameBuffer.hpp"

#include <iostream>

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, bool colorAttachment, bool depthAttachment, GLenum samplingMode, GLenum wrappingMode) : Width(width), Height(height)
{
	glGenFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	if(colorAttachment)
	{
		m_colorAttachment = std::make_shared<Texture>(width, height, nullptr, GL_RGB8, GL_RGB, samplingMode, wrappingMode);
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment->m_ID, 0);
	}

	if(depthAttachment)
	{
		m_depthAttachment = std::make_shared<Texture>(width, height, nullptr, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, samplingMode, wrappingMode);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment->m_ID, 0);
	}
	else
	{
		glGenRenderbuffers(1, &m_renderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferID);

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	if(!m_depthAttachment)
		glDeleteRenderbuffers(1, &m_renderBufferID);

	glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	if(!m_depthAttachment)
		glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferID);

	glViewport(0, 0, Width, Height);
}
