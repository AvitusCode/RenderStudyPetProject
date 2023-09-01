#include "Math/VolumeAABB.h"
#include "Math/Bounder.h"

namespace Primitives
{
	std::shared_ptr<BoundingVolume> VolumeAABB::makeVolume(const Model& model) const
	{
		glm::vec3 R, S, T;
		try {
			auto evecs = jd::math::findEigenVectors(model);
			R = evecs[0];
			S = evecs[1];
			T = evecs[2];
		}
		catch (...) {
			return nullptr;
		}

		glm::vec3 min{ FLT_MAX };
		glm::vec3 max{ -FLT_MAX };
		for (auto&& mesh : model.meshes)
		{
			for (auto&& vertex : mesh.vertices)
			{
				float d1 = glm::dot(vertex.Position, R);
				min.x = glm::min(min.x, d1);
				max.x = glm::max(max.x, d1);
				float d2 = glm::dot(vertex.Position, S);
				min.y = glm::min(min.y, d2);
				max.y = glm::max(max.y, d2);
				float d3 = glm::dot(vertex.Position, T);
				min.z = glm::min(min.z, d3);
				max.z = glm::max(max.z, d3);
			}
		}

		return std::make_shared<AABB>(min, max);
	}
}