#pragma once
#include "ECS/components/AssimpModel.h"
#include <memory>

namespace Primitives
{
    struct BoundingVolume;

    class IVolumeConstructor abstract
    {
    public:
        virtual std::shared_ptr<BoundingVolume> makeVolume(const Model& model) const abstract = 0;
        virtual ~IVolumeConstructor() = default;
    };
}