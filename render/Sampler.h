#pragma once
#include <glad/glad.h>


enum class MagFilter
{
	MAG_FILTER_NEAREST, // Nearest filter for magnification
	MAG_FILTER_BILINEAR, // Bilinear filter for magnification
};

enum class MinFilter
{
	MIN_FILTER_NEAREST, // Nearest filter for minification
	MIN_FILTER_BILINEAR, // Bilinear filter for minification
	MIN_FILTER_NEAREST_MIPMAP, // Nearest filter for minification, but on closest mipmap
	MIN_FILTER_BILINEAR_MIPMAP, // Bilinear filter for minification, but on closest mipmap
	MIN_FILTER_TRILINEAR, // Bilinear filter for minification on two closest mipmaps, then averaged
};

/**
 * Wraps OpenGL sampler into a convenient class.
 */
class Sampler
{
public:
	Sampler() = default;
	~Sampler();

	void create(GLuint dim = 2, bool repeat = true);
	void bind(int textureUnit = 0) const;
	void unbind(int textureUnit = 0) const;
	void deleteSampler();
	void setMagnificationFilter(MagFilter magFilter) const;
	void setMinificationFilter(MinFilter minFilter) const;
	void setRepeat(bool repeat) const;

private:
	GLuint m_samplerID = 0; // OpenGL-assigned sampler ID
	bool m_isCreated = false; // Flag telling, if sampler has been created
	GLuint m_dimension = 2; // Sampler dimension

	/**
	 * Checks, if the sampler has been created and if not, logs it into console.
	 * @return True, if sampler has been created or false otherwise.
	 */
	bool createdCheck() const;
};