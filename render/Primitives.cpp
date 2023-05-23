#include <glad/glad.h>
#include "Primitives.h"

// PhysiX primitives
#include "../ECS/components/RigidBody.h"
#include "../ECS/components/Thrust.h"
#include "../ECS/components/Mesh2D.h"
#include "../ECS/components/Transform.h"
#include "../ECS/components/Renderable.h"
#include "../ECS/components/Vertex.h"
#include "../ECS/components/Camera.h"
#include "../ECS/components/AssimpModel.h"
#include "../physiX/PhysX.h"

#include "../ECS/jdEngine.h"
#include <glm/gtc/constants.hpp>
#include <limits>
#include <cassert>

#include "../render/VertexBuffer.h"
namespace Primitives
{
    glm::vec3 cubeVertices[36] =
    {
        // Front face
        glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), 
        glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
        // Back face
        glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), 
        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
        // Left face
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, 0.5f), 
        glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
        // Right face
        glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f), 
        glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
        // Top face
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 
        glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-0.5f, 0.5f, -0.5f),
        // Bottom face
        glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f), 
        glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
    };

    glm::vec3 cubeNormals[36] =
    {
        glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0)
    };

    glm::vec2 cubeTexCoords[6] =
    {
        glm::vec2(0.0f, 1.0f), 
        glm::vec2(1.0f, 1.0f), 
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 0.0f), 
        glm::vec2(0.0f, 0.0f), 
        glm::vec2(0.0f, 1.0f)
    };

    glm::vec3 cubeFaceColors[6] =
    {
        glm::vec3(1.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3(1.0f, 1.0f, 0.0f), 
        glm::vec3(1.0f, 0.0f, 0.0f)
    };

    glm::vec3 pyramidVertices[12] =
    {
        // Front face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
        // Back face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
        // Left face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
        // Right face
        glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f, -0.5f)
    };

    glm::vec2 pyramidTexCoords[3] =
    {
        glm::vec2(0.5f, 1.0f), 
        glm::vec2(0.0f, 0.0f), 
        glm::vec2(1.0f, 0.0f)
    };

    glm::vec3 pyramidFaceColors[3] =
    {
        glm::vec3(1.0f, 1.0f, 0.0f), 
        glm::vec3(0.25f, 0.0f, 0.5f), 
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    // Render with GL_TRIANGLE_STRIP
    glm::vec3 plainGroundVertices[4] =
    {
        glm::vec3(-200.0f, 0.0f, -200.0f), // Left-back point
        glm::vec3(-200.0f, 0.0f, 200.0f), // Left-front point
        glm::vec3(200.0f, 0.0f, -200.0f), // Right-back point
        glm::vec3(200.0f, 0.0f, 200.0f) // Right-front point
    };

    glm::vec2 plainGroundTexCoords[4] =
    {
        glm::vec2(0.0f, 20.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(20.0f, 20.0f),
        glm::vec2(20.0f, 0.0f)
    };

    glm::vec3 plainGroundColors[4] =
    {
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.0f, 0.85f, 0.0f),
        glm::vec3(0.0f, 0.35f, 0.25f),
        glm::vec3(0.0f, 0.8f, 0.2f)
    };

    // Render with GL_TRIANGLE_STRIP
    glm::vec2 quad2D[4] =
    {
        glm::vec2(0, 1), // Top-left point
        glm::vec2(0, 0), // Bottom-left point
        glm::vec2(1, 1), // Top-right point
        glm::vec2(1, 0) // Bottom-right point
    };


    Circle::Circle(size_t s, float r) : cSegments(s), radius(r) {
        assert(s >= SEGMENTS && "to less segments in Circle");
    }

    void Circle::OnInit(Transform2D* body, Renderable* renderable)
    {
        renderable->rdata.emplace_back();
        auto& data = renderable->rdata.back();

        glGenVertexArrays(1, &data.VAO);
        glBindVertexArray(data.VAO);

        VertexBuffer vbo;
        std::vector<Vertex2D> vertices(cSegments + 2);
        vbo.makeVBO((cSegments + 2) * sizeof(Vertex2D));
        vbo.bindVBO(GL_ARRAY_BUFFER);

        // BEGIN main part to estimates data
        const float twicePi = 2.0f * glm::pi<float>() / static_cast<float>(cSegments);
        vertices[0].Position = glm::vec2(0.0f, 0.0f);
        vertices[0].TexCoords = glm::vec2(0.5f, 0.5f);
        for (size_t i = 0; i <= cSegments; i++)
        {
            const float x = glm::cos(static_cast<float>(i) * twicePi);
            const float y = glm::sin(static_cast<float>(i) * twicePi);

            vertices[i + 1].Position = glm::vec2{ x, y };
            vertices[i + 1].TexCoords = glm::vec2((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f);
        }
        // END

        vbo.addRawData(vertices.data(), vertices.size() * sizeof(Vertex2D));
        vbo.uploadDataToGPU(GL_STATIC_DRAW);

        std::vector<GLuint> indices;
        for (size_t i = 0; i < vertices.size(); i++) {
            indices.push_back(static_cast<GLuint>(i));
        }

        VertexBuffer ebo;
        ebo.makeVBO(indices.size() * sizeof(GLuint));
        ebo.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
        ebo.addRawData(indices.data(), indices.size() * sizeof(GLuint));
        ebo.uploadDataToGPU(GL_STATIC_DRAW);

        glVertexAttribPointer(Mesh2D::position_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
        glEnableVertexAttribArray(Mesh2D::position_attrib);
        glVertexAttribPointer(Mesh2D::texture_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<const void*>(offsetof(Vertex2D, TexCoords)));
        glEnableVertexAttribArray(Mesh2D::texture_attrib);

        glBindVertexArray(0);

        data.drawArray = GL_ELEMENT_ARRAY_BUFFER;
        data.drawModeType = GL_TRIANGLE_FAN;
        data.sizeOfDraw = static_cast<GLuint>(indices.size());
        body->scale = glm::vec2{ radius, radius };
    }


    void Polygon::OnInit(Transform2D* body, Renderable* renderable)
    {
        renderable->rdata.emplace_back();
        auto& data = renderable->rdata.back();

        glGenVertexArrays(1, &data.VAO);
        glBindVertexArray(data.VAO);

        VertexBuffer vbo;
        std::vector<Vertex2D> vertices(m_cVertex);
        vbo.makeVBO(m_cVertex * sizeof(Vertex2D));
        vbo.bindVBO(GL_ARRAY_BUFFER);

        for (size_t i = 0; i < m_cVertex; i++)
        {
            vertices[i].Position = m_vertices[i];
            vertices[i].TexCoords = m_textures[i];
        }

        vbo.addRawData(vertices.data(), vertices.size() * sizeof(Vertex2D));
        vbo.uploadDataToGPU(GL_STATIC_DRAW);

        std::vector<GLuint> indices;

        for (size_t i = 0; i < m_cVertex; i++) {
            indices.push_back(static_cast<GLuint>(i));
        }

        VertexBuffer ebo;
        ebo.makeVBO(indices.size() * sizeof(GLuint));
        ebo.bindVBO(GL_ELEMENT_ARRAY_BUFFER);
        ebo.addRawData(indices.data(), indices.size() * sizeof(GLuint));
        ebo.uploadDataToGPU(GL_STATIC_DRAW);

        glVertexAttribPointer(Mesh2D::position_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), nullptr);
        glEnableVertexAttribArray(Mesh2D::position_attrib);
        glVertexAttribPointer(Mesh2D::texture_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), reinterpret_cast<const void*>(offsetof(Vertex2D, TexCoords)));
        glEnableVertexAttribArray(Mesh2D::texture_attrib);

        glBindVertexArray(0);

        data.drawArray = GL_ELEMENT_ARRAY_BUFFER;
        data.drawModeType = GL_TRIANGLE_FAN;
        data.sizeOfDraw = static_cast<GLuint>(indices.size());
        body->scale = glm::vec2(m_width, m_height);
    }

    void Polygon::SetBox(float hw, float hh)
    {
        m_cVertex = 4;

        m_vertices[0] = glm::vec2(-1.0f, -1.0f);
        m_vertices[1] = glm::vec2(1.0f, -1.0f);
        m_vertices[2] = glm::vec2(1.0f, 1.0f);
        m_vertices[3] = glm::vec2(-1.0f, 1.0f);

        m_normals[0] = glm::vec2(0.0f, -1.0f);
        m_normals[1] = glm::vec2(1.0f, 0.0f);
        m_normals[2] = glm::vec2(0.0f, 1.0f);
        m_normals[3] = glm::vec2(-1.0f, 0.0f);

        m_textures[0] = glm::vec2(0.0f, 0.0f);
        m_textures[1] = glm::vec2(1.0f, 0.0f);
        m_textures[2] = glm::vec2(1.0f, 1.0f);
        m_textures[3] = glm::vec2(0.0f, 1.0f);

        m_width = hw;
        m_height = hh;
    }

    // Building convex hull from the fiving vertices
    void Polygon::SetBoxes(const std::vector<glm::vec2>& vertices, float hw, float hh)
    {
        // No hulls with less than 3 vertices (ensure actual polygon)
        assert(vertices.size() > 2 && vertices.size() <= MAX_VERTICES);
        const size_t count = glm::min(static_cast<int>(vertices.size()), MAX_VERTICES);

        // Find the right most point on the hull
        size_t rightMost = 0;
        float highestXCoord = vertices[0].x;
        for (size_t i = 1; i < count; i++)
        {
            float x = vertices[i].x;
            if (x > highestXCoord)
            {
                highestXCoord = x;
                rightMost = i;
            }

            // If matching x then take farthest negative y
            else if (x == highestXCoord) {
                if (vertices[i].y < vertices[rightMost].y) {
                    rightMost = i;
                }
            }
        }

        size_t hull[MAX_VERTICES] = { 0 };
        size_t outCount = 0;
        size_t indexHull = rightMost;

        while (true)
        {
            hull[outCount] = indexHull;

            // Search for next index that wraps around the hull
            // by computing cross products to find the most counter-clockwise
            // vertex in the set, given the previos hull index
            size_t nextHullIndex = 0;
            for (size_t i = 1; i < count; i++)
            {
                // Skip if same coordinate as we need three unique
                // points in the set to perform a cross product
                if (nextHullIndex == indexHull)
                {
                    nextHullIndex = i;
                    continue;
                }

                // Cross every set of three unique vertices
                // Record each counter clockwise third vertex and add
                // to the output hull
                glm::vec2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
                glm::vec2 e2 = vertices[i] - vertices[hull[outCount]];
                float c = jd::cross(e1, e2);
                if (c < 0.0f) {
                    nextHullIndex = i;
                }

                // Cross product is zero then e vectors are on same line
                // therefor want to record vertex farthest along that line
                if (c == 0.0f && jd::lenSqr(e2) > jd::lenSqr(e1)) {
                    nextHullIndex = i;
                }
            }

            ++outCount;
            indexHull = nextHullIndex;

            // Conclude algorithm upon wrap-around
            if (nextHullIndex == rightMost)
            {
                m_cVertex = outCount;
                break;
            }
        }

        // Copy vertices into shape's vertices
        for (size_t i = 0; i < m_cVertex; i++) {
            m_vertices[i] = vertices[hull[i]];
        }

        // Compute face normals
        for (size_t i = 0; i < m_cVertex; i++)
        {
            glm::vec2 face = m_vertices[(i + 1) % m_cVertex] - m_vertices[i];
            // Ensure no zero-length edges, because that's bad
            assert(jd::lenSqr(face) > 0.0001 * 0.0001);
            // Calculate normal with 2D cross product between vector and scalar
            m_normals[i] = glm::normalize(glm::vec2(face.y, -face.x));
        }

        // Compute texcoords
        for (size_t i = 0; i < m_cVertex; i++) {
            m_textures[i] = glm::vec2((m_vertices[i].x + 1.0f) * 0.5f, (m_vertices[i].y + 1.0f) * 0.5f);
        }

        m_width = hw;
        m_height = hh;
    }

    // The extreme point along a direction within a polygon
    glm::vec2 Polygon::GetSupport(const glm::vec2& dir) const
    {
        float bestProjection = -FLT_MAX;
        glm::vec2 bestVertex{ 0.0f, 0.0f };

        for (size_t i = 0; i < m_cVertex; i++)
        {
            glm::vec2 v = m_vertices[i];
            float projection = glm::dot(v, dir);

            if (projection > bestProjection)
            {
                bestVertex = v;
                bestProjection = projection;
            }
        }

        return bestVertex;
    }

    // Primitives version 2.0
    Plane::Plane(const glm::vec3& p1, const glm::vec3& norm) :
        normal(glm::normalize(norm)),
        distance(glm::dot(normal, p1)) {}

    float Plane::getDistanceToPlane(const glm::vec3& point) const {
        return glm::dot(point, normal) - distance;
    }

    Sphere::Sphere(const glm::vec3& inCenter, float inRadius)
        : BoundingVolume{}, center{ inCenter }, radius{ inRadius }
    {}

    bool Sphere::isOnOrForwardPlane(const Plane& plane) const {
        return plane.getDistanceToPlane(center) > -radius;
    }

    bool Sphere::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const
    {
        const glm::vec3 globalCenter{ transform.computeModelMatrix() * glm::vec4(center, 1.f) };
        const glm::vec3 globalScale = transform.getGlobalScale();
        const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

        //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
        Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

        //Check Firstly the result that have the most chance to failure to avoid to call all functions.
        return (globalSphere.isOnOrForwardPlane(camFrustum.leftFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.rightFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.farFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.nearFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.topFace) &&
            globalSphere.isOnOrForwardPlane(camFrustum.bottomFace));
    };


    SquareAABB::SquareAABB(const glm::vec3& inCenter, float inExtent)
        : BoundingVolume{}, center{ inCenter }, extent{ inExtent }
    {}

    bool SquareAABB::isOnOrForwardPlane(const Plane& plane) const
    {
        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
        const float r = extent * (std::abs(plane.normal.x) + std::abs(plane.normal.y) + std::abs(plane.normal.z));
        return -r <= plane.getDistanceToPlane(center);
    }

    bool SquareAABB::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const
    {
        //Get global scale thanks to our transform
        const glm::vec3 globalCenter{ transform.computeModelMatrix() * glm::vec4(center, 1.f) };

        // Scaled orientation
        const glm::vec3 right = transform.getRight() * extent;
        const glm::vec3 up = transform.getUp() * extent;
        const glm::vec3 forward = transform.getForward() * extent;

        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

        const SquareAABB globalAABB(globalCenter, std::max(std::max(newIi, newIj), newIk));

        return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.farFace));
    };

    AABB::AABB(const glm::vec3& min, const glm::vec3& max)
        : BoundingVolume{}, center{ (max + min) * 0.5f }, extents{ max.x - center.x, max.y - center.y, max.z - center.z }
    {}

    AABB::AABB(const glm::vec3& inCenter, float iI, float iJ, float iK)
        : BoundingVolume{}, center{ inCenter }, extents{ iI, iJ, iK }
    {}

    std::array<glm::vec3, 8> AABB::getVertice() const
    {
        std::array<glm::vec3, 8> vertice;
        vertice[0] = { center.x - extents.x, center.y - extents.y, center.z - extents.z };
        vertice[1] = { center.x + extents.x, center.y - extents.y, center.z - extents.z };
        vertice[2] = { center.x - extents.x, center.y + extents.y, center.z - extents.z };
        vertice[3] = { center.x + extents.x, center.y + extents.y, center.z - extents.z };
        vertice[4] = { center.x - extents.x, center.y - extents.y, center.z + extents.z };
        vertice[5] = { center.x + extents.x, center.y - extents.y, center.z + extents.z };
        vertice[6] = { center.x - extents.x, center.y + extents.y, center.z + extents.z };
        vertice[7] = { center.x + extents.x, center.y + extents.y, center.z + extents.z };
        return vertice;
    }

    //see https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html
    bool AABB::isOnOrForwardPlane(const Plane& plane) const
    {
        // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
        const float r = extents.x * std::abs(plane.normal.x) + extents.y * std::abs(plane.normal.y) + extents.z * std::abs(plane.normal.z);
        return -r <= plane.getDistanceToPlane(center);
    }

    bool AABB::isOnFrustum(const Frustum& camFrustum, const Transform& transform) const
    {
        //Get global scale thanks to our transform
        const glm::vec3 globalCenter{ transform.computeModelMatrix() * glm::vec4(center, 1.f) };

        // Scaled orientation
        const glm::vec3 right = transform.getRight() * extents.x;
        const glm::vec3 up = transform.getUp() * extents.y;
        const glm::vec3 forward = transform.getForward() * extents.z;

        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

        const AABB globalAABB(globalCenter, newIi, newIj, newIk);

        return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
            globalAABB.isOnOrForwardPlane(camFrustum.farFace));
    };

    Frustum createFrustumFromCamera(Entity cam_id, float aspect, float fovY, float zNear, float zFar)
    {
        auto& engine = jd::Engine::getEngine();
        auto& cam = engine.GetComponent<Camera>(cam_id);
        auto& transform = engine.GetComponent<Transform>(cam_id);

        const jd::Quaternion qF = cam.orientation * jd::Quaternion::identity(0.0f, 0.0f, -1.0f, 0.0f) * cam.orientation.conjugate();
        const glm::vec3 Front{ qF.x, qF.y, qF.z };
        const glm::vec3 Right = glm::normalize(glm::cross(Front, glm::vec3{ 0.0f, 1.0f, 0.0f }));
        const glm::vec3 Up = glm::normalize(glm::cross(Right, Front));

        Frustum frustum;
        const float halfVSide = zFar * tanf(fovY * 0.5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * Front;

        frustum.nearFace = { transform.position + zNear * Front, Front };
        frustum.farFace = { transform.position + frontMultFar, -Front };
        frustum.rightFace = { transform.position, glm::cross(frontMultFar - Right * halfHSide, Up) };
        frustum.leftFace = { transform.position, glm::cross(Up, frontMultFar + Right * halfHSide) };
        frustum.topFace = { transform.position, glm::cross(Right, frontMultFar - Up * halfVSide) };
        frustum.bottomFace = { transform.position, glm::cross(frontMultFar + Up * halfVSide, Right) };

        return frustum;
    }

    AABB generateAABB(const Model& model)
    {
        glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());

        for (auto&& mesh : model.meshes)
        {
            for (auto&& vertex : mesh.vertices)
            {
                minAABB.x = glm::min(minAABB.x, vertex.Position.x);
                minAABB.y = glm::min(minAABB.y, vertex.Position.y);
                minAABB.z = glm::min(minAABB.z, vertex.Position.z);

                maxAABB.x = glm::max(maxAABB.x, vertex.Position.x);
                maxAABB.y = glm::max(maxAABB.y, vertex.Position.y);
                maxAABB.z = glm::max(maxAABB.z, vertex.Position.z);
            }
        }

        return AABB(minAABB, maxAABB);
    }

    Sphere generateSphereBV(const Model& model)
    {
        glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());

        for (auto&& mesh : model.meshes)
        {
            for (auto&& vertex : mesh.vertices)
            {
                minAABB.x = glm::min(minAABB.x, vertex.Position.x);
                minAABB.y = glm::min(minAABB.y, vertex.Position.y);
                minAABB.z = glm::min(minAABB.z, vertex.Position.z);

                maxAABB.x = glm::max(maxAABB.x, vertex.Position.x);
                maxAABB.y = glm::max(maxAABB.y, vertex.Position.y);
                maxAABB.z = glm::max(maxAABB.z, vertex.Position.z);
            }
        }

        return Sphere((maxAABB + minAABB) * 0.5f, glm::length(minAABB - maxAABB));
    }
}



