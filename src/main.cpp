#include <filesystem>
#include <iostream>

#include "RE_Window.h"
#include "SceneLoader.h"
#include "Shader.h"
#include "Utils.h"

int main(int argc, char* argv[]) {
    RE_Window* re_window = new RE_Window("REngine", 800, 600);
    if (re_window->Init() != 0) {
        FATAL("Couldn't initialize window");
        return -1;
    }

    Scene* scene = new Scene();
    if (!SceneLoader::load("scene.csv", scene)) {
        if (!SceneLoader::load("../scene.csv", scene)) {
            FATAL("Couldn't load the scene");
            return -1;
        }
    }
    re_window->scene = scene;

    Shader shader("", "");
    if (!std::filesystem::exists("shaders/vertex.glsl") || !std::filesystem::exists("shaders/fragment.glsl")) {
        DEBUG("shaders/ not found");
        if (!std::filesystem::exists("../shaders/vertex.glsl") || !std::filesystem::exists("../shaders/fragment.glsl")) {
            DEBUG("../shaders/ not found");
            FATAL("Shaders not found!");
            delete re_window;
            return -1;
        }
        shader = Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    } else {
        shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    }

    re_window->shader = &shader;

    bool quit = false;
    SDL_Event e;

    Uint32 previousTime = SDL_GetTicks();
    double deltaTime = 0.0f;
    int frames = 0;

    while (!quit) {
        // Проверка ввода
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
        }
        // Вычислить дельту по времени
        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - previousTime) / 1000.0f;

        // Вывести FPS
        if (currentTime / 1000 - previousTime / 1000 >= 1) {
            INFO("FPS: " << std::to_string(frames));
            frames = 0;
        }
        frames++;
        previousTime = currentTime;
        re_window->Draw(deltaTime);
    }

    delete re_window;
    DEBUG("Goodnight");

    return 0;
}