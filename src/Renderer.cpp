#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>

#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Logging.h"
#include "Texture.h"

void applyTexture(REngine::Mesh* mesh, std::string texturePath);

REngine::Renderer::Renderer(int width, int height)
    : width(width), height(height), camera(width, height) {}

int REngine::Renderer::Init(GLADloadproc procAddress) {
    if (!gladLoadGLLoader(procAddress)) {
        FATAL("Failed to initialize OpenGL");
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

void REngine::Renderer::Draw(unsigned long ticks) {
    glClearColor(0.63f, 0.63f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setMat4("view", camera.getViewMatrix());
    shader->setMat4("projection", camera.getProjectionMatrix());
    shader->setFloat("u_time", ticks / 1000.0f);
    shader->setVec3("u_light_color", scene.lightingColor);
    shader->setVec3("u_light_position", scene.lightingPosition);
    shader->setFloat("u_ambient_strength", scene.ambientStrength);

    for (const SceneNode node : scene.nodes) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, node.position);
        model = glm::rotate(model, glm::radians(node.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, node.scale);
        shader->setMat4("model", model);
        shader->setMat4("normalMatrix", glm::transpose(glm::inverse(model)));
        shader->setBool("distort", node.distort);
        shader->setVec3("cameraPos", camera.position);
        applyTexture(node.mesh, node.texturePath);
        node.mesh->draw(*shader);
    }
}

void applyTexture(REngine::Mesh* mesh, std::string texturePath) {
    if (texturePath.empty()) return;

    REngine::Texture* tex = new REngine::Texture();
    if (REngine::Texture::textures.count(texturePath) > 0) {
        mesh->texture = REngine::Texture::textures[texturePath];
    } else if (tex->loadBMP(texturePath)) {
        REngine::Texture::textures[texturePath] = tex;
        mesh->texture = tex;
    } else if (tex->loadBMP("./textures/" + texturePath)) {
        REngine::Texture::textures[texturePath] = tex;
        mesh->texture = tex;
    } else if (tex->loadBMP("../textures/" + texturePath)) {
        REngine::Texture::textures[texturePath] = tex;
        mesh->texture = tex;
    } else {
        ERROR("Failed to load texture: " + texturePath);
    }
}