#pragma once

// A class for a collection of geometric textures
class Gbuffer
{
public:
	Gbuffer() = default;
	void init(int width, int height);
	void bind();
	// Transfer a depth component from one framebuffer to another
	void blitFromTo(unsigned int from, unsigned int to, int width, int height);

	unsigned int getBuffer() const;
	~Gbuffer();

private:
	unsigned int gBuffer;

	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedoSpec;

	unsigned int rboDepth;

	bool status = true;
};