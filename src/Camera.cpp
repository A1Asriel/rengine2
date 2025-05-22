#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Logging.h"

REngine::Camera::Camera(int width, int height)
    : position{0,0,3}, front{0,0,-1}, up{0,1,0}, fov{60.0f}, w(width), h(height) {}

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

void REngine::Camera::rotateRelative(int drx, int dry, int drz) {
    // TODO: Крен
    static const float limit = 89.0f;
    float yaw = glm::degrees(atan2(front.z, front.x));
    float pitch = glm::degrees(asin(front.y));

    yaw += drx / 5.0f;
    pitch = glm::clamp(pitch - dry / 5.0f, -limit, limit);

    float yawRad = glm::radians(yaw);
    float pitchRad = glm::radians(pitch);

    front.x = cos(yawRad) * cos(pitchRad);
    front.y = sin(pitchRad);
    front.z = sin(yawRad) * cos(pitchRad);
    front = glm::normalize(front);
}
