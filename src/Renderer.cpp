#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>

#include "CubeMesh.h"
#include "SphereMesh.h"
#include "Logging.h"
#include "Texture.h"

#define POINT_LIGHTS_MAX 8

void applyTexture(REngine::SceneNode& node, glm::vec3 defColor = glm::vec3(1.0f));
void applySpecTexture(REngine::SceneNode& node, glm::vec3 defColor = glm::vec3(0.5f));

REngine::Renderer::Renderer(int width, int height)
    : width(width), height(height), camera(width, height) {}

void REngine::Renderer::setScene(Scene* scene) {
    this->scene = *scene;
    camera.position = scene->camera.position;
    camera.setRotation(scene->camera.rotation.x, scene->camera.rotation.y, scene->camera.rotation.z);
    camera.fov = scene->camera.fov;
}

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
    glClearColor(scene.skyColor.x, scene.skyColor.y, scene.skyColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();
    shader->setMat4("view", camera.getViewMatrix());
    shader->setMat4("projection", camera.getProjectionMatrix());
    shader->setFloat("u_time", ticks / 1000.0f);
    shader->setVec3("u_camera_position", camera.position);
    shader->setVec3("dirLight.direction", scene.dirLight.direction);
    shader->setVec3("dirLight.ambient", scene.dirLight.ambient);
    shader->setVec3("dirLight.diffuse", scene.dirLight.diffuse);
    shader->setVec3("dirLight.specular", scene.dirLight.specular);
    for (int i = 0; i < std::min((int)scene.pointLights.size(), POINT_LIGHTS_MAX); i++) {
        shader->setVec3("pointLights[" + std::to_string(i) + "].position", scene.pointLights[i].position);
        shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", scene.pointLights[i].ambient);
        shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", scene.pointLights[i].diffuse);
        shader->setVec3("pointLights[" + std::to_string(i) + "].specular", scene.pointLights[i].specular);
        shader->setFloat("pointLights[" + std::to_string(i) + "].constant", scene.pointLights[i].constant);
        shader->setFloat("pointLights[" + std::to_string(i) + "].linear", scene.pointLights[i].linear);
        shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", scene.pointLights[i].quadratic);
    }
    if (scene.pointLights.size() < POINT_LIGHTS_MAX) {
        for (int i = scene.pointLights.size(); i < POINT_LIGHTS_MAX; i++) {
            shader->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0.0f));
            shader->setVec3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.0f));
            shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.0f));
            shader->setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0.0f));
            shader->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shader->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.0f);
            shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.0f);
        }
    }

    for (SceneNode& node : scene.nodes) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, node.position);
        model = glm::rotate(model, glm::radians(node.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(node.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
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
