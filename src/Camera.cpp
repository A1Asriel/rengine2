#include "Camera.h"

#include <SDL2/SDL_scancode.h>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(int width, int height)
    : position{0,0,3}, front{0,0,-1}, up{0,1,0}, pitch{0}, yaw{-90}, fov{60.0f}, w(width), h(height) {}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov),
                            float(w)/float(h), 0.1f, 100.0f);
}

void Camera::processKeyboard(float deltaTime, const Uint8* keystate) {
    glm::vec3 viewPos(0.0f);

    if (keystate[SDL_SCANCODE_W]) viewPos.z += deltaTime * (keystate[SDL_SCANCODE_LSHIFT]+1);
    if (keystate[SDL_SCANCODE_A]) viewPos.x -= deltaTime * (keystate[SDL_SCANCODE_LSHIFT]+1);
    if (keystate[SDL_SCANCODE_S]) viewPos.z -= deltaTime * (keystate[SDL_SCANCODE_LSHIFT]+1);
    if (keystate[SDL_SCANCODE_D]) viewPos.x += deltaTime * (keystate[SDL_SCANCODE_LSHIFT]+1);
    if (keystate[SDL_SCANCODE_LCTRL] || keystate[SDL_SCANCODE_RCTRL]) viewPos.y -= deltaTime * (keystate[SDL_SCANCODE_LSHIFT]+1);
    if (keystate[SDL_SCANCODE_SPACE]) viewPos.y += deltaTime * (keystate[SDL_SCANCODE_LSHIFT]+1);

    glm::vec3 right = glm::normalize(glm::cross(front, up));
    glm::vec3 upDirection = glm::normalize(glm::cross(right, front));
    position += viewPos.z * front + viewPos.x * right + viewPos.y * upDirection;
}

void Camera::processMouse(int dx, int dy) {
    static const float limit = 89.0f;
    pitch = glm::clamp(pitch - dy / 5.0f, -limit, limit);
    yaw += dx / 5.0f;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
}
