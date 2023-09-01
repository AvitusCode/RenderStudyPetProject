#include "Utils/topology.h"
#include <unordered_map>
#include <cassert>

namespace jd::utils
{
	[[nodiscard]] std::vector<GLuint> makeAdjacencyIndexes(GLuint numTris, const std::vector<GLuint>& srcIndexes)
	{
		assert(3 * numTris == srcIndexes.size());

		const GLuint n = 3 * numTris;
		std::vector<GLuint> result(2 * n);
		std::unordered_map<Edge, GLuint, adjHash> adj;

		for (GLuint i = 0; i < numTris; ++i)
		{
			const GLuint j = 3 * i;
			adj[Edge{ srcIndexes[j], srcIndexes[j + 1] }] = srcIndexes[j + 2];
			adj[Edge{ srcIndexes[j + 1], srcIndexes[j + 2] }] = srcIndexes[j];
			adj[Edge{ srcIndexes[j + 2], srcIndexes[j] }] = srcIndexes[j + 1];
		}

		for (GLuint i = 0; i < numTris; ++i)
		{
			const GLuint j = 6 * i;
			const GLuint k = 3 * i;

			result[j] = srcIndexes[k];
			result[j + 2] = srcIndexes[k + 1];
			result[j + 4] = srcIndexes[k + 2];

			result[j + 1] = adj[Edge{ srcIndexes[k + 1], srcIndexes[k] }];
			result[j + 3] = adj[Edge{ srcIndexes[k + 2], srcIndexes[k + 1] }];
			result[j + 5] = adj[Edge{ srcIndexes[k], srcIndexes[k + 2] }];
		}

		return result;
	}
}