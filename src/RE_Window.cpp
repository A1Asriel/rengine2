#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "RE_Window.h"
#include "Cube.h"
#include <glm/gtc/type_ptr.hpp>
#include <memory>

RE_Window::RE_Window(std::string title, int width, int height) {
    this->title = title;
    this->width = width;
    this->height = height;
}

RE_Window::~RE_Window() {
    SDL_GL_DeleteContext(sdl_glcontext);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

SDL_Window* RE_Window::getSDL_Window() {
    return sdl_window;
}

SDL_GLContext RE_Window::getSDL_GLContext() {
    return sdl_glcontext;
}

int RE_Window::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    sdl_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!sdl_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    sdl_glcontext = SDL_GL_CreateContext(sdl_window);
    if (!sdl_glcontext) {
        std::cerr << "OpenGL sdl_context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return -1;
    }

    std::cout << "GPU vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);


    // FIXME: Кубы
    for (int i = 0; i < 10; i++) {
        std::unique_ptr<Cube> cube = std::make_unique<Cube>();
        cube->update((float)i);
        vecCube.push_back(std::move(cube));
    }



    return 0;
}

void RE_Window::Draw(double deltaTime) {
    glClearColor(0.53f, 0.39f, 0.72f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)width / (float)height,
        0.1f,
        100.0f
    );

    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for (std::unique_ptr<Cube>& cube : this->vecCube) {
        shader->setMat4("model", cube->getModelMatrix());
        cube->draw();
        cube->update(deltaTime);
    }

    SDL_GL_SwapWindow(sdl_window);
}