#include "Math/VolumeSphere.h"
#include "Math/Bounder.h"
#include <vector>
#include <glm/gtx/norm.hpp>

namespace Primitives
{
	std::shared_ptr<BoundingVolume> VolumeSphere::makeVolume(const Model& model) const
	{
		glm::vec3 R;
		try {
			R = jd::math::findEigenVectors(model)[0];
		}
		catch (...) {
			return nullptr;
		}
		glm::vec3 Pmin{ 0.0f }, Pmax{ 0.0f };
		std::vector<const glm::vec3*> ptr_pos;

		{
			float min = FLT_MAX, max = -FLT_MAX;
			for (auto&& mesh : model.meshes)
			{
				for (auto&& vertex : mesh.vertices)
				{
					ptr_pos.push_back(std::addressof(vertex.Position));
					float d = glm::dot(vertex.Position, R);
					if (d < min)
					{
						min = d;
						Pmin = vertex.Position;
					}
					if (d > max)
					{
						max = d;
						Pmax = vertex.Position;
					}
				}
			}
		}

		glm::vec3 Q = (Pmin + Pmax) / 2.0f;
		float r = glm::l2Norm(Pmin - Q);

		// Expand sphere if it is necessary 
		for (const auto& point : ptr_pos)
		{
			glm::vec3 prob = *point - Q;
			if (float n = glm::l2Norm(prob); n * n > r * r)
			{
				glm::vec3 G = Q - r * (prob) / n;
				Q = (G + *point) / 2.0f;
				r = glm::l2Norm(*point - Q);
			}
		}

		return std::make_shared<Sphere>(Q, r);
	}
}