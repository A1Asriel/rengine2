#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Logging.h"

REngine::Camera::Camera(int width, int height)
    : position{0,0,3}, front{0,0,-1}, up{0,1,0}, fov{60.0f}, w(width), h(height) {}

void REngine::Camera::setRotation(float rx, float ry, float rz) {
    front = glm::vec3(cos(glm::radians(ry)) * cos(glm::radians(rx)),
                      sin(glm::radians(rx)),
                      sin(glm::radians(ry)) * cos(glm::radians(rx)));
    front = glm::normalize(front);
}

std::tuple<float, float, float> REngine::Camera::getRotation() const {
    return std::make_tuple(glm::degrees(asin(front.y)), glm::degrees(atan2(front.z, front.x)), 0);
}

glm::mat4 REngine::Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 REngine::Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov),
                            float(w)/float(h), 0.1f, 100.0f);
}

void REngine::Camera::moveRelative(float dx, float dy, float dz) {
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    glm::vec3 upDirection = glm::normalize(glm::cross(right, front));
    position += dz * front + dx * right + dy * upDirection;
}

void REngine::Camera::rotateRelative(float drx, float dry, float drz) {
    // TODO: Крен
    static const float limit = 89.0f;
    auto [pitch, yaw, roll] = getRotation();

    yaw += drx;
    pitch = glm::clamp(pitch - dry, -limit, limit);

    setRotation(pitch, yaw, 0);
}
