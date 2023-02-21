#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 makeLookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
    glm::vec3 z = glm::normalize(position - target);
    glm::vec3 x = glm::normalize(glm::cross(up, z));
    glm::vec3 y = glm::cross(z, x);

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -position.x;
    translation[3][1] = -position.y;
    translation[3][2] = -position.z;

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = x.x;
    rotation[1][0] = x.y;
    rotation[2][0] = x.z;
    rotation[0][1] = y.x;
    rotation[1][1] = y.y;
    rotation[2][1] = y.z;
    rotation[0][2] = z.x;
    rotation[1][2] = z.y;
    rotation[2][2] = z.z;

    return rotation * translation;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD) {
        Position += Front * velocity;
    }
    if (direction == Camera_Movement::BACKWARD) {
        Position -= Front * velocity;
    }
    if (direction == Camera_Movement::LEFT) {
        Position -= Right * velocity;
    }
    if (direction == Camera_Movement::RIGHT) {
        Position += Right * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f) {
        Zoom -= yoffset;
    }
    if (Zoom <= 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom >= 45.0f) {
        Zoom = 45.0f;
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

const glm::vec3& Camera::getPosition() const{
    return Position;
}
const glm::vec3& Camera::getFront() const {
    return Front;
}
const glm::vec3& Camera::getUp() const {
    return Up;
}
const glm::vec3& Camera::getRight() const {
    return Right;
}
const glm::vec3& Camera::getWorldUp() const {
    return WorldUp;
}
float Camera::getYaw() const {
    return Yaw;
}
float Camera::getPitch() const {
    return Pitch;
}
float Camera::getMSpeed() const {
    return MovementSpeed;
}
float Camera::getMSentivity() const {
    return MouseSensitivity;
}
float Camera::getZoom() const {
    return Zoom;
}

glm::vec3& Camera::getPosition() {
    return Position;
}
glm::vec3& Camera::getFront() {
    return Front;
}
glm::vec3& Camera::getUp() {
    return Up;
}
glm::vec3& Camera::getRight() {
    return Right;
}
glm::vec3& Camera::getWorldUp() {
    return WorldUp;
}
void Camera::setYaw(float num) {
    Yaw = num;
}
void Camera::setPitch(float num) {
    Pitch = num;
}
void Camera::setMSpeed(float num) {
    MovementSpeed = num;
}
void Camera::setMSentivity(float num) {
    MouseSensitivity = num;
}
void Camera::setZoom(float num) {
    Zoom = num;
}