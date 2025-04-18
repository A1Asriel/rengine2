#include <iostream>
#include <filesystem>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Cube.h"
#include "MathUtils.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(
        "Spinning Cube",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::cout << "GPU vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    Shader shader("", "");
    if (!std::filesystem::exists("shaders/vertex.glsl") || !std::filesystem::exists("shaders/fragment.glsl")) {
        if (!std::filesystem::exists("../shaders/vertex.glsl") || !std::filesystem::exists("../shaders/fragment.glsl")) {
            std::cerr << "Shaders not found!" << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }
        shader = Shader("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    } else {
        shader = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    }

    Cube cube;

    bool quit = false;
    SDL_Event e;

    Uint32 previousTime = SDL_GetTicks();
    float deltaTime = 0.0f;
    float fps = 0.0f;
    float avgFps = 0.0f;
    int frames = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        deltaTime = (currentTime - previousTime) / 1000.0f;

        if (std::floor(currentTime/1000)-std::floor(previousTime/1000) >= 1) {
            std::cout << "FPS: " << std::to_string(avgFps) << std::endl;
            std::cout << "Frames rendered: " << std::to_string(frames) << std::endl;
            avgFps = fps;
            frames = 1;
        }
        else {
            avgFps = MathUtils::continuousAverage(avgFps, fps, frames);
            frames += 1;
        }

        previousTime = currentTime;
        fps = 1 / deltaTime;

        glClearColor(0.53f, 0.39f, 0.72f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.update(deltaTime);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
            0.1f,
            100.0f
        );

        shader.use();

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat4("model", cube.getModelMatrix());

        cube.draw();

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}