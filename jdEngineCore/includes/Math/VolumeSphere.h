#pragma once
#include "IVolumeConstructor.h"
#include "Render/Primitives.h"

namespace Primitives
{
	class VolumeSphere : public IVolumeConstructor
	{
	public:
		std::shared_ptr<BoundingVolume> makeVolume(const Model& model) const override;
	};
}