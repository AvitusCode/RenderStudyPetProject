#include "Render/Sampler.h"
#include "Utils/logger.h"

Sampler::~Sampler() {
	deleteSampler();
}

void Sampler::create(GLuint dim, bool repeat)
{
	if (m_isCreated) {
		return;
	}

	if (dim < 2u || dim > 3u) {
		LOG(ERROR) << "Wrong sampler dimension";
		return;
	}

	m_dimension = dim;

	glGenSamplers(1, &m_samplerID);
	m_isCreated = true;
	setRepeat(repeat);

	LOG(INFO) << "Create sampler id=" << m_samplerID;
}

void Sampler::bind(int textureUnit) const
{
	if (!createdCheck()) {
		LOG(ERROR) << "non existing sampler!";
		return;
	}

	glBindSampler(textureUnit, m_samplerID);
}

void Sampler::unbind(int textureUnit) const {
	glBindSampler(textureUnit, 0);
}

void Sampler::deleteSampler()
{
	if (!createdCheck()) {
		return;
	}

	LOG(INFO) << "Deleting sampler with ID " << m_samplerID;

	glDeleteSamplers(1, &m_samplerID);
	m_isCreated = false;
}

void Sampler::setMagnificationFilter(MagFilter magFilter) const
{
	if (!createdCheck()) {
		LOG(ERROR) << "non existing sampler!";
		return;
	}

	const GLenum pname = GL_TEXTURE_MAG_FILTER;
	GLint param = GL_NEAREST;

	if (magFilter == MagFilter::MAG_FILTER_BILINEAR) {
		param = GL_LINEAR;
	}

	glSamplerParameteri(m_samplerID, pname, param);
}

void Sampler::setMinificationFilter(MinFilter minFilter) const
{
	if (!createdCheck()) {
		LOG(ERROR) << "non existing sampler!";
		return;
	}

	const GLenum pname = GL_TEXTURE_MIN_FILTER;
	GLint param = GL_NEAREST;

	switch (minFilter)
	{
	case MinFilter::MIN_FILTER_BILINEAR:
		param = GL_LINEAR;
		break;
	case MinFilter::MIN_FILTER_NEAREST_MIPMAP:
		param = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case MinFilter::MIN_FILTER_BILINEAR_MIPMAP:
		param = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case MinFilter::MIN_FILTER_TRILINEAR:
		param = GL_LINEAR_MIPMAP_LINEAR;
		break;
	}

	glSamplerParameteri(m_samplerID, pname, param);
}

void Sampler::setRepeat(bool repeat) const
{
	if (!createdCheck()) {
		LOG(ERROR) << "non existing sampler!";
		return;
	}

	GLint param = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

	glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_S, param);
	glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_T, param);

	if (m_dimension == 3) {
		glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_R, param);
	}
}

bool Sampler::createdCheck() const 
{
	if (!m_isCreated)
	{
		return false;
	}

	return true;
}