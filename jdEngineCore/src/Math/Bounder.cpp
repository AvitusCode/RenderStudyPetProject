#include "Math/Bounder.h"
#include "Utils/logger.h"
#include <algorithm>
#include <vector>
#include <array>

namespace jd::math
{
	glm::vec3 meanVectors(const Model& model)
	{
		glm::vec3 result{ 0.0f };
		size_t cVert = 0;

		for (auto&& mesh : model.meshes)
		{
			for (auto&& vertex : mesh.vertices)
			{
				result.x += vertex.Position.x;
				result.y += vertex.Position.y;
				result.z += vertex.Position.z;
				++cVert;
			}
		}

		result /= static_cast<float>(cVert);
		return result;
	}

	std::array<float, 6> calcCIJ(const Model& model)
	{
		std::array<float, 6> result; result.fill(0.0f);
		size_t cVert = 0;
		const glm::vec3 m = meanVectors(model);

		for (auto&& mesh : model.meshes)
		{
			for (auto&& vertex : mesh.vertices)
			{
				result[0] += (vertex.Position.x - m.x) * (vertex.Position.x - m.x);
				result[1] += (vertex.Position.y - m.y) * (vertex.Position.y - m.y);
				result[2] += (vertex.Position.z - m.z) * (vertex.Position.z - m.z);
				result[3] += (vertex.Position.x - m.x) * (vertex.Position.y - m.y);
				result[4] += (vertex.Position.x - m.x) * (vertex.Position.z - m.z);
				result[5] += (vertex.Position.y - m.y) * (vertex.Position.z - m.z);
				++cVert;
			}
		}

		for (auto& c : result) {
			c /= static_cast<float>(cVert);
		}

		return result;
	}

	std::array<glm::vec3, 3> findEigenVectors(const Model& model)
	{
		// Finding mean vectors and correlations betwean axises
		const auto C = calcCIJ(model);

		// Forming a covariance matrix
		using matrix_t = std::vector<std::vector<double>>;
		using vec_t = std::vector<double>;

		matrix_t matC(3, vec_t(3, 0.0f));
		matrix_t evec(3, vec_t(3, 0.0f));
		vec_t eval(3);
		matC[0][0] = C[0]; matC[1][1] = C[1]; matC[2][2] = C[2];
		matC[0][1] = matC[1][0] = C[3];
		matC[0][2] = matC[2][0] = C[4];
		matC[1][2] = matC[2][1] = C[5];

		try {
			Jacobi<double, vec_t&, matrix_t&, const matrix_t&> eigen_calc(3);
			size_t status = eigen_calc.makeEval(matC, eval, evec);
			if (status == 0) {
				LOG(ERROR) << "eigenVectors iterations fail!";
			}
		}
		catch (...) {
			LOG(ERROR) << "eigenVectors estimation has been broken!";
			throw;
		}

		std::array<glm::vec3, 3> result;
		for (size_t i = 0; i < result.size(); i++)
		{
			result[i].x = evec[i][0];
			result[i].y = evec[i][1];
			result[i].z = evec[i][2];
		}

		return result;
	}
}