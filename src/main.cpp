#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Cube.h"
#include "Shader.h"
#include "RE_Window.h"

int main(int argc, char* argv[]) {
    RE_Window* re_window = new RE_Window("REngine", 800, 600);
    if (re_window->Init() != 0) {
        return -1;
    }

    Shader shader("", "");
    if (!std::filesystem::exists("shaders/vertex.glsl") || !std::filesystem::exists("shaders/fragment.glsl")) {
        std::cerr << "ERROR: shaders/ not found" << std::endl;
        if (!std::filesystem::exists("../shaders/vertex.glsl") || !std::filesystem::exists("../shaders/fragment.glsl")) {
            std::cerr << "ERROR: ../shaders/ not found" << std::endl;
            std::cerr << "Shaders not found!" << std::endl;
            delete re_window;
            return -1;
        }
        std::cout << "DEBUG: ../shaders/ found" << std::endl;
        shader = Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    } else {
        std::cout << "DEBUG: shaders/ found" << std::endl;
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
            std::cout << "INFO: FPS: " << std::to_string(frames) << std::endl;
            frames = 0;
        }
        frames++;
        previousTime = currentTime;
        re_window->Draw(deltaTime);
    }

    delete re_window;
    std::cout << "Goodnight" << std::endl;

    return 0;
}