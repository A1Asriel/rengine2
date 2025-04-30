#include "RE_Window.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "Cube.h"
#include "Utils.h"

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
        FATAL("SDL could not initialize! SDL_Error: " << SDL_GetError());
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
        FATAL("Window could not be created! SDL_Error: " << SDL_GetError());
        SDL_Quit();
        return -1;
    }

    sdl_glcontext = SDL_GL_CreateContext(sdl_window);
    if (!sdl_glcontext) {
        FATAL("OpenGL sdl_context could not be created! SDL_Error: " << SDL_GetError());
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        FATAL("Failed to initialize GLAD");
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return -1;
    }

    INFO("GPU vendor: " << glGetString(GL_VENDOR));
    INFO("Renderer: " << glGetString(GL_RENDERER));
    INFO("OpenGL version: " << glGetString(GL_VERSION));
    INFO("Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

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

    std::vector<SceneNode*> toBeDiscarded;
    for (const SceneNode node : scene->nodes) {
        if (node.mesh == MeshType::Cube) {
            Cube cube;
            glm::mat4 model = cube.getModelMatrix();
            model = glm::translate(model, node.position);
            model = glm::rotate(model, glm::radians(node.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(node.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(node.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, node.scale);
            shader->setMat4("model", model);
            cube.draw();
        } else {
            ERROR("Mesh type not yet implemented");
            toBeDiscarded.push_back(const_cast<SceneNode*>(&node));
        }
    }

    // Удалить нереализованные объекты из сцены
    for (SceneNode* nodePointer : toBeDiscarded) {
        int curPos = -1;
        for (const SceneNode node: scene->nodes) {
            curPos++;
            if (&node == nodePointer) break;
        }
        if (curPos > -1) scene->nodes.erase(scene->nodes.begin() + curPos);
    }
    toBeDiscarded.clear();

    SDL_GL_SwapWindow(sdl_window);
}