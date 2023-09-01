#pragma once
#include "IVolumeConstructor.h"
#include "Render/Primitives.h"

namespace Primitives
{
	class VolumeAABB : public IVolumeConstructor
	{
	public:
		std::shared_ptr<BoundingVolume> makeVolume(const Model& model) const override;
	};
}