#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../physiX/Quaternion.h"

struct Camera
{
    inline static const float SPEED = 2.5f;
    inline static const float SENSITIVITY = 0.1f;
    inline static const float ZOOM = 45.0f;

    // Camera attribs
    jd::Quaternion orientation = jd::Quaternion::identity(0.0f, 0.0f, -1.0f, 0.0f);
    float RightAngle = 0.0f;
    float UpAngle = 0.0f;

    // Camera settings
    float MovementSpeed = SPEED;
    float MouseSensitivity = SENSITIVITY;
    float Zoom = ZOOM;

    glm::mat4 projectionTransform;

    static glm::mat4 GetProjectionTransform(float fov, float nearClip, float farClip, int windowWidth, int windowHeight) {
        return glm::perspective(fov, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), nearClip, farClip);
    }

    static glm::mat4 GetViewMatrix(const glm::vec3& position, const jd::Quaternion& orientation) {
        jd::Quaternion reverseOrient = orientation.conjugate();
        glm::mat4 rot = reverseOrient.mat4_cast();
        glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, -position);

        return rot * translation;
    }
};
