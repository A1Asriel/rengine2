#ifndef CAMERA_H
#define CAMERA_H

#include <SDL_stdinc.h>

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position, front, up;
    float pitch, yaw, fov;
    int w, h;
    Camera(int width, int height);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void processKeyboard(float deltaTime, const Uint8* keystate);
    void processMouse(int dx, int dy);
};

#endif
