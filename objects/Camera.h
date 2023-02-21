#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Camera interface
class Camera
{
public:
    // const parametrs
    inline static const float YAW = -90.0f;
    inline static const float PITCH = 0.0f;
    inline static const float SPEED = 2.5f;
    inline static const float SENSITIVITY = 0.1f;
    inline static const float ZOOM = 45.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true); 
    void ProcessMouseScroll(float yoffset);

    const glm::vec3& getPosition() const;
    const glm::vec3& getFront() const;
    const glm::vec3& getUp() const;
    const glm::vec3& getRight() const;
    const glm::vec3& getWorldUp() const;
    float getYaw() const;
    float getPitch() const;
    float getMSpeed() const;
    float getMSentivity() const;
    float getZoom() const;

    glm::vec3& getPosition();
    glm::vec3& getFront();
    glm::vec3& getUp();
    glm::vec3& getRight();
    glm::vec3& getWorldUp();
    void setYaw(float num);
    void setPitch(float num);
    void setMSpeed(float num);
    void setMSentivity(float num);
    void setZoom(float num);

private:
    // Camera attribs
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Camera settings
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    void updateCameraVectors();
};