#include "Gbuffer.h"
#include <glad/glad.h>


void makeTexture(GLenum& target, GLint internalFormat, GLsizei width, GLsizei height, GLint format, GLenum type, int i = 0)
{
	glGenTextures(1, &target);
	glBindTexture(GL_TEXTURE_2D, target);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, target, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Gbuffer::init(int width, int height)
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	makeTexture(gPosition, GL_RGBA16F, width, height, GL_RGBA, GL_FLOAT, 0);
	makeTexture(gNormal, GL_RGBA16F, width, height, GL_RGBA, GL_FLOAT, 1);
	makeTexture(gAlbedoSpec, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 2);

	unsigned int attachment[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachment);

	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		status = false;
	}
}

void Gbuffer::bind()
{
	if (!status) {
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
}

void Gbuffer::blitFromTo(unsigned int from, unsigned int to, int width, int height)
{
	if (!status) {
		return;
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, from);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, to);
}

unsigned int Gbuffer::getBuffer() const {
	return gBuffer;
}

Gbuffer::~Gbuffer()
{
	glDeleteTextures(1, &gPosition);
	glDeleteTextures(1, &gNormal);
	glDeleteTextures(1, &gAlbedoSpec);

	glDeleteRenderbuffers(1, &rboDepth);
	glDeleteFramebuffers(1, &gBuffer);
}
