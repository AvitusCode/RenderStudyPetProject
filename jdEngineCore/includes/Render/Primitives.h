#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../ECS/configurations.h"
#include "../ECS/components/AssimpModel.h"
// PhysiX primitives

struct Transform2D;
struct Renderable;
struct Camera;
struct Transform;

namespace Primitives
{
    // A simple cube with unit size
    extern glm::vec3 cubeVertices[36];
    extern glm::vec3 cubeNormals[36];
    extern glm::vec2 cubeTexCoords[6];
    extern glm::vec3 cubeFaceColors[6];

    // A simple pyramid with unit size
    extern glm::vec3 pyramidVertices[12];
    extern glm::vec2 pyramidTexCoords[3];
    extern glm::vec3 pyramidFaceColors[3];

    // Ground, that has greenish color
    extern glm::vec3 plainGroundVertices[4];
    extern glm::vec2 plainGroundTexCoords[4];
    extern glm::vec3 plainGroundColors[4];

    // 2D quad of unit size, anchored in lower-left point (same as texture coordinates, render with triangle strip)
    extern glm::vec2 quad2D[4];


    // Start 2D physics shapes 
    struct Shape2D
    {
        Shape2D() = default;
        virtual ~Shape2D() = default;
        // First Configuration to make RenderInformation
        virtual void OnInit(Transform2D* body, Renderable* renderable) = 0;
        bool is_static = false;
    };


    struct Circle : Shape2D
    {
        Circle() = default;
        // @param s must be bigger than SEGMENTS
        Circle(size_t s, float r);
        void OnInit(Transform2D* body, Renderable* renderable) override;

        static inline constexpr size_t SEGMENTS = 20;
        size_t cSegments = SEGMENTS;
        float radius;
    };

    struct Polygon : Shape2D
    {
        void OnInit(Transform2D* body, Renderable* renderable) override;

        // Half width and half height
        void SetBox(float hw, float hh);
        // Building convex hull from the fiving vertices
        void SetBoxes(const std::vector<glm::vec2>& vertices, float hw, float hh);
        // The extreme point along a direction within a polygon
        glm::vec2 GetSupport(const glm::vec2& dir) const;

        inline static constexpr int MAX_VERTICES = 16;
        size_t m_cVertex = 0;
        float m_width = 0.0f;
        float m_height = 0.0f;
        glm::vec2 m_vertices[MAX_VERTICES] = { glm::vec2(0.0f, 0.0f) };
        glm::vec2 m_normals[MAX_VERTICES] = { glm::vec2(0.0f, 0.0f) };
        glm::vec2 m_textures[MAX_VERTICES] = { glm::vec2(0.0f, 0.0f) };
    };

    // Primitives version 2.0

    struct Plane
    {
        glm::vec3 normal{ 0.0f, 1.0f, 0.0f };
        float distance{ 0.0f };

        Plane() = default;
        Plane(const glm::vec3& p1, const glm::vec3& norm);
        float getDistanceToPlane(const glm::vec3& point) const;
    };

    // View Frustum
    struct Frustum
    {
        Plane topFace;
        Plane bottomFace;

        Plane rightFace;
        Plane leftFace;

        Plane farFace;
        Plane nearFace;
    };

    struct BoundingVolume abstract
    {
        virtual bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const abstract = 0;
        virtual bool isOnOrForwardPlane(const Plane& plane) const abstract = 0;
        virtual ~BoundingVolume() = default;

        bool isOnFrustum(const Frustum& camFrustum) const
        {
            return (isOnOrForwardPlane(camFrustum.leftFace) &&
                isOnOrForwardPlane(camFrustum.rightFace) &&
                isOnOrForwardPlane(camFrustum.topFace) &&
                isOnOrForwardPlane(camFrustum.bottomFace) &&
                isOnOrForwardPlane(camFrustum.nearFace) &&
                isOnOrForwardPlane(camFrustum.farFace));
        };
    };

	struct Sphere : public BoundingVolume
	{
		glm::vec3 center{ 0.f, 0.f, 0.f };
		float radius{ 0.f };

		Sphere(const glm::vec3& inCenter, float inRadius);
		bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const override final;
		bool isOnOrForwardPlane(const Plane& plane) const override final;
	};

	struct SquareAABB : public BoundingVolume
	{
		glm::vec3 center{ 0.f, 0.f, 0.f };
		float extent{ 0.f };

        SquareAABB(const glm::vec3& inCenter, float inExtent);
        bool isOnOrForwardPlane(const Plane& plane) const override final;
        bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const override final;
	};

	struct AABB : public BoundingVolume
	{
		glm::vec3 center{ 0.f, 0.f, 0.f };
		glm::vec3 extents{ 0.f, 0.f, 0.f };

        AABB(const glm::vec3& min, const glm::vec3& max);
        AABB(const glm::vec3& inCenter, float iI, float iJ, float iK);
        std::array<glm::vec3, 8> getVertice() const;
        bool isOnOrForwardPlane(const Plane& plane) const override final;
        bool isOnFrustum(const Frustum& camFrustum, const Transform& transform) const override final;
	};

    Frustum createFrustumFromCamera(Entity cam_id, float aspect, float fovY, float zNear, float zFar);
    AABB generateAABB(const Model& model);
    Sphere generateSphereBV(const Model& model);
}

