#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>
#include <SDL.h>

#include "Mesh.h"
#include "Logging.h"
#include "Texture.h"
#include "Volume.h"

void applyTexture(REngine::SceneNode& node, glm::vec3 defColor = glm::vec3(1.0f));
void applySpecTexture(REngine::SceneNode& node, glm::vec3 defColor = glm::vec3(0.5f));

REngine::Renderer::Renderer(int width, int height)
    : width(width), height(height) {}

void REngine::Renderer::setScene(Scene* scene) {
    this->scene = scene;
}

void REngine::Renderer::setShader(const char* vertexPath, const char* fragmentPath) {
    shader = new Shader(vertexPath, fragmentPath);
}

REngine::Renderer* REngine::initRenderer(int width, int height) {
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        FATAL("Failed to initialize OpenGL");
        return nullptr;
    }

    INFO("GPU vendor: " << glGetString(GL_VENDOR));
    INFO("Renderer: " << glGetString(GL_RENDERER));
    INFO("OpenGL version: " << glGetString(GL_VERSION));
    INFO("Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION));

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    return new REngine::Renderer(width, height);
}

void REngine::Renderer::draw(unsigned long ticks) {
    glClearColor(scene->skyColor.x, scene->skyColor.y, scene->skyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setMat4("view", scene->camera.getViewMatrix());
    shader->setMat4("projection", scene->camera.getProjectionMatrix());
    shader->setFloat("u_time", ticks / 1000.0f);
    shader->setVec3("u_camera_position", scene->camera.position);
    shader->setVec3("dirLight.direction", scene->dirLight.direction);
    shader->setVec3("dirLight.ambient", scene->dirLight.ambient);
    shader->setVec3("dirLight.diffuse", scene->dirLight.diffuse);
    shader->setVec3("dirLight.specular", scene->dirLight.specular);
    shader->setInt("pointLightsCount", std::min((int)scene->pointLights.size(), POINT_LIGHTS_MAX));
    for (int i = 0; i < std::min((int)scene->pointLights.size(), POINT_LIGHTS_MAX); i++) {
        shader->setVec3("pointLights[" + std::to_string(i) + "].position", scene->pointLights[i].position);
        shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", scene->pointLights[i].ambient);
        shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", scene->pointLights[i].diffuse);
        shader->setVec3("pointLights[" + std::to_string(i) + "].specular", scene->pointLights[i].specular);
        shader->setFloat("pointLights[" + std::to_string(i) + "].constant", scene->pointLights[i].constant);
        shader->setFloat("pointLights[" + std::to_string(i) + "].linear", scene->pointLights[i].linear);
        shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", scene->pointLights[i].quadratic);
    }

    REngine::Frustum frustum(scene->camera, (float)width / (float)height);
    for (SceneNode& node : scene->nodes) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, node.position);
        model = glm::rotate(model, glm::radians(node.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        const glm::vec3 min = node.mesh->getMin() * node.scale;
        const glm::vec3 max = node.mesh->getMax() * node.scale;

        const std::vector<glm::vec3> corners = {
            glm::vec3(model * glm::vec4(min.x, min.y, min.z, 1.0f)),
            glm::vec3(model * glm::vec4(min.x, min.y, max.z, 1.0f)),
            glm::vec3(model * glm::vec4(min.x, max.y, min.z, 1.0f)),
            glm::vec3(model * glm::vec4(min.x, max.y, max.z, 1.0f)),
            glm::vec3(model * glm::vec4(max.x, min.y, min.z, 1.0f)),
            glm::vec3(model * glm::vec4(max.x, min.y, max.z, 1.0f)),
            glm::vec3(model * glm::vec4(max.x, max.y, min.z, 1.0f)),
            glm::vec3(model * glm::vec4(max.x, max.y, max.z, 1.0f))
        };

        glm::vec3 worldMin = corners[0];
        glm::vec3 worldMax = corners[0];
        for (const glm::vec3& corner : corners) {
            worldMin = glm::min(worldMin, corner);
            worldMax = glm::max(worldMax, corner);
        }
        
        if (!frustum.isBoxInFrustum(worldMin, worldMax)) {
            continue;
        }

        model = glm::scale(model, node.scale);
        shader->setMat4("model", model);
        shader->setMat4("normalMatrix", glm::transpose(glm::inverse(model)));
        shader->setBool("distort", node.distort);
        shader->setFloat("material.shininess", node.shininess);
        applyTexture(node);
        applySpecTexture(node);
        node.mesh->draw(*shader);
    }
}

void applyTexture(REngine::SceneNode& node, glm::vec3 defColor) {
    REngine::Texture* tex = new REngine::Texture();
    if (node.texturePath.empty()) {
        node.texturePath = "gen_color_" + std::to_string(defColor.x) + "_" + std::to_string(defColor.y) + "_" + std::to_string(defColor.z);
        tex->genFromColor(defColor.x, defColor.y, defColor.z);
        node.mesh->texture = tex;
        REngine::Texture::textures[node.texturePath] = tex;
        return;
    }
    if (REngine::Texture::textures.count(node.texturePath) > 0) {
        node.mesh->texture = REngine::Texture::textures[node.texturePath];
    } else if (tex->loadBMP(node.texturePath)) {
        REngine::Texture::textures[node.texturePath] = tex;
        node.mesh->texture = tex;
    } else if (tex->loadBMP("./textures/" + node.texturePath)) {
        REngine::Texture::textures[node.texturePath] = tex;
        node.mesh->texture = tex;
    } else if (tex->loadBMP("../textures/" + node.texturePath)) {
        REngine::Texture::textures[node.texturePath] = tex;
        node.mesh->texture = tex;
    } else {
        ERROR("Failed to load texture: " + node.texturePath);
        node.texturePath = "gen_color_" + std::to_string(defColor.x) + "_" + std::to_string(defColor.y) + "_" + std::to_string(defColor.z);
        tex->genFromColor(defColor.x, defColor.y, defColor.z);
        node.mesh->texture = tex;
        REngine::Texture::textures[node.texturePath] = tex;
    }
}

void applySpecTexture(REngine::SceneNode& node, glm::vec3 defColor) {
    REngine::Texture* tex = new REngine::Texture();
    if (node.specularPath.empty()) {
        node.specularPath = "gen_color_" + std::to_string(defColor.x) + "_" + std::to_string(defColor.y) + "_" + std::to_string(defColor.z);
        tex->genFromColor(defColor.x, defColor.y, defColor.z);
        node.mesh->specularTexture = tex;
        REngine::Texture::textures[node.specularPath] = tex;
        return;
    }
    if (REngine::Texture::textures.count(node.specularPath) > 0) {
        node.mesh->specularTexture = REngine::Texture::textures[node.specularPath];
    } else if (tex->loadBMP(node.specularPath)) {
        REngine::Texture::textures[node.specularPath] = tex;
        node.mesh->specularTexture = tex;
    } else if (tex->loadBMP("./textures/" + node.specularPath)) {
        REngine::Texture::textures[node.specularPath] = tex;
        node.mesh->specularTexture = tex;
    } else if (tex->loadBMP("../textures/" + node.specularPath)) {
        REngine::Texture::textures[node.specularPath] = tex;
        node.mesh->specularTexture = tex;
    } else {
        ERROR("Failed to load specular texture: " + node.specularPath);
        node.specularPath = "gen_color_" + std::to_string(defColor.x) + "_" + std::to_string(defColor.y) + "_" + std::to_string(defColor.z);
        tex->genFromColor(defColor.x, defColor.y, defColor.z);
        node.mesh->specularTexture = tex;
        REngine::Texture::textures[node.specularPath] = tex;
    }
}
