#pragma once
#include <glad/glad.h>
#include <vector>
#include <type_traits>

namespace jd::utils
{
	class Edge
	{
	public:
		Edge(GLuint f, GLuint t) : from(f), to(t) {}

		void setFrom(GLuint f) {
			from = f;
		}

		void setTo(GLuint t) {
			to = t;
		}

		GLuint getFrom() const {
			return from;
		}

		GLuint getTo() const {
			return to;
		}

	private:
		GLuint from;
		GLuint to;
	};

	inline bool operator==(const Edge& lhs, const Edge& rhs) {
		return lhs.getFrom() == rhs.getFrom() && lhs.getTo() == rhs.getTo();
	}

	inline bool operator<(const Edge& lhs, const Edge& rhs) {
		return lhs.getFrom() != rhs.getFrom() ? lhs.getFrom() < rhs.getFrom() : lhs.getTo() < rhs.getTo();
	}

	struct adjHash {
		size_t operator()(const Edge& edge) const noexcept
		{
			const size_t x = 2'946'901;
			const auto uiHash = std::hash<GLuint>{};
			return uiHash(edge.getFrom()) * x + uiHash(edge.getTo());
		}
	};

    // Making new adjacency topology
    // @param numTris - count of the triangles
    // @param srsIndexes - indexes of the source mesh
    // @return indexes of adjacency topology
	[[nodiscard]] std::vector<GLuint> makeAdjacencyIndexes(GLuint numTris, const std::vector<GLuint>& srcIndexes);
}