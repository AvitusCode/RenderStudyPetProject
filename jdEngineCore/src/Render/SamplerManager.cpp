#include "Render/SamplerManager.h"
#include <stdexcept>

SamplerManager::SamplerManager(SamplerManager&& other) noexcept
{
    *this = std::move(other);
}

SamplerManager& SamplerManager::operator=(SamplerManager&& other) noexcept
{
    if (this != &other)
    {
        m_samplers.clear();
        m_samplers = std::move(other.m_samplers);
    }

    return *this;
}

Sampler* SamplerManager::createSampler(const std::string& key, const MagFilter magFilter, const MinFilter minFilter, GLuint dim, bool rep, GLuint bindUnit)
{
    if (containsSampler(key)) 
    {
        std::string msg = "Sampler alredy exists: " + key;
        throw std::runtime_error(msg.c_str());
    }

    std::unique_ptr<Sampler> sampler = std::make_unique<Sampler>();
    sampler->create(dim, rep);
    sampler->bind(bindUnit);
    sampler->setMagnificationFilter(magFilter);
    sampler->setMinificationFilter(minFilter);
    m_samplers[key] = std::move(sampler);

    return getSampler(key);
}

const Sampler* SamplerManager::getSampler(const std::string& key) const
{
    auto iter = m_samplers.find(key);

    if (iter == m_samplers.end())
    {
        std::string msg = "Attemp to read non-existing sampler with key: " + key;
        throw std::runtime_error(msg.c_str());
    }

    return iter->second.get();
}

Sampler* SamplerManager::getSampler(const std::string& key) {
    return const_cast<Sampler*>(static_cast<const SamplerManager*>(this)->getSampler(key));
}

void SamplerManager::clearSamplerCache() {
    for (auto& [key, sampler] : m_samplers) {
        sampler->deleteSampler();
    }
    m_samplers.clear();
}

bool SamplerManager::containsSampler(const std::string& key) const {
    return m_samplers.count(key) > 0;
}