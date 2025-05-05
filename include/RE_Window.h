#ifndef RE_WINDOW_H
#define RE_WINDOW_H

#include <SDL2/SDL.h>

#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Scene.h"

class RE_Window {
private:
    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;
    int width, height;
    std::string title;
public:
    RE_Window(std::string title, int width, int height);
    ~RE_Window();
    SDL_Window* getSDL_Window();
    SDL_GLContext getSDL_GLContext();
    Shader* shader;
    Scene* scene;
    Camera camera;
    int Init();
    void Draw(double deltaTime);
};

#endif
