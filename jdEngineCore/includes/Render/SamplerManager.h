#pragma once
#include <glad/glad.h>
#include <string>
#include <memory>
#include <map>
#include "Sampler.h"


class SamplerManager
{
public:
	SamplerManager() = default;
	SamplerManager(const SamplerManager&) = delete; 
	SamplerManager& operator=(const SamplerManager&) = delete;
	SamplerManager(SamplerManager&&) noexcept;
	SamplerManager& operator=(SamplerManager&&) noexcept;

	Sampler* createSampler(const std::string& key, const MagFilter magFilter, const MinFilter minFilter, GLuint dim = 2, bool rep = true, GLuint bindUnit = 0);
	const Sampler* getSampler(const std::string& key) const;
	Sampler* getSampler(const std::string& key);
	bool containsSampler(const std::string& key) const;
	void clearSamplerCache();

private:
	std::map<std::string, std::unique_ptr<Sampler>> m_samplers;
};
