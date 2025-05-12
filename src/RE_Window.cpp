#include "RE_Window.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>
#include <filesystem>

#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Logging.h"
#include "Texture.h"

void applyTexture(Mesh* mesh, std::string texturePath);

RE_Window::RE_Window(std::string title, int width, int height)
    : title(title), width(width), height(height), camera(width, height) {}

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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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

    if (SDL_GL_SetSwapInterval(1) != 0) {
        ERROR("Couldn't set up Vsync: " << SDL_GetError());
    }

    return 0;
}

void RE_Window::Draw(double deltaTime) {
    glClearColor(0.53f, 0.39f, 0.72f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setMat4("view", camera.getViewMatrix());
    shader->setMat4("projection", camera.getProjectionMatrix());

    for (const SceneNode node : scene->nodes) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, node.position);
        model = glm::rotate(model, glm::radians(node.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, node.scale);
        shader->setMat4("model", model);
        switch (node.mesh) {
        case MeshType::Cube: {
            CubeMesh* mesh = new CubeMesh();
            applyTexture(mesh, node.texturePath);
            mesh->init();
            mesh->draw(*shader);
            delete mesh;
            break;
        }
        case MeshType::Sphere: {
            SphereMesh* mesh = new SphereMesh();
            applyTexture(mesh, node.texturePath);
            mesh->init();
            mesh->draw(*shader);
            delete mesh;
            break;
        }
        default: {
            ERROR("Mesh type not yet implemented");
            break;
        }
        }
    }

    // Удалить нереализованные объекты из сцены
    scene->nodes.erase(std::remove_if(scene->nodes.begin(), scene->nodes.end(), [&](SceneNode node) {
        return node.mesh == MeshType::NotImplemented;
    }), scene->nodes.end());

    SDL_GL_SwapWindow(sdl_window);
}

void applyTexture(Mesh* mesh, std::string texturePath) {
    if (texturePath.empty()) return;

    Texture* tex = new Texture();
    if (Texture::textures.count(texturePath) > 0) {
        mesh->texture = Texture::textures[texturePath];
    } else if (tex->loadBMP(texturePath)) {
        Texture::textures[texturePath] = tex;
        mesh->texture = tex;
    } else if (tex->loadBMP("./textures/" + texturePath)) {
        Texture::textures[texturePath] = tex;
        mesh->texture = tex;
    } else if (tex->loadBMP("../textures/" + texturePath)) {
        Texture::textures[texturePath] = tex;
        mesh->texture = tex;
    } else {
        ERROR("Failed to load texture: " + texturePath);
    }
}