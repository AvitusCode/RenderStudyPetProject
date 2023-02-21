#pragma once
#include <vector>
#include <string>

#include <glad/glad.h>

// Simple cubemap class

class Cubemap
{
public:
	Cubemap();
	Cubemap& operator=(const Cubemap& cm);
	Cubemap& operator=(Cubemap&& cm) noexcept;

	/*A constructor that builds a cubemap from a path that contains 6 PNG files named: 
	right.png 
	left.png 
	top.png 
	bottom.png 
	front.png 
	back.png*/ 
	Cubemap(std::string facesPath);
	// A constructor that builds a cubemap directly from any 6 given images path
	Cubemap(const std::vector<std::string>& faces);

	void setupObject();

	unsigned int getId() const;
	const std::vector<std::string>& getFaces() const;

public:
	unsigned int texID;
	std::vector<std::string> faces;
};
