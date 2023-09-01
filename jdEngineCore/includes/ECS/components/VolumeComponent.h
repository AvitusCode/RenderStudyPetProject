#pragma once
#include <memory>

struct BoundingVolume;

struct VolumeComponent
{
	std::shared_ptr<BoundingVolume> p_volume;
};