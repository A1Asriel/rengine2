#include <SDL.h>
#include <gtest/gtest.h>

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Scene.h"
#include "Shader.h"
#include "Engine.h"

TEST(Camera, DefaultViewProjection) {
    const int w = 800, h = 600;
    REngine::Camera cam(w, h);

    glm::mat4 view = cam.getViewMatrix();

    glm::mat4 proj = cam.getProjectionMatrix();

    EXPECT_NE(glm::determinant(view), 0.0f);
    EXPECT_NE(glm::determinant(proj), 0.0f);

    glm::vec3 initialPos = cam.position;
    cam.moveRelative(1.0f, 0.0f, 0.0f);
    EXPECT_NE(glm::length(cam.position - initialPos), 0.0f);

    glm::vec3 initialRotation = cam.getRotation();
    cam.rotateRelative(10.0f, 10.0f, 10.0f);
    glm::vec3 newRotation = cam.getRotation();
    EXPECT_NE(initialRotation.x, newRotation.x);
}

TEST(Renderer, Initialization) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FAIL() << "SDL could not initialize! SDL_Error: " << SDL_GetError();
    }
    SDL_Window* window = SDL_CreateWindow("Test", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        800, 600, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    ASSERT_NE(window, nullptr) << "Window could not be created!";

    SDL_GLContext context = SDL_GL_CreateContext(window);
    ASSERT_NE(context, nullptr) << "OpenGL context could not be created!";

    REngine::Renderer* renderer = REngine::initRenderer(800, 600);
    ASSERT_NE(renderer, nullptr) << "Renderer initialization failed";

    REngine::Scene scene;
    renderer->setScene(&scene);

    renderer->setShader(NULL, NULL);

    REngine::Scene* scenePtr = renderer->getScene();
    EXPECT_NE(scenePtr, nullptr);
    
    EXPECT_EQ(renderer->getWidth(), 800);
    EXPECT_EQ(renderer->getHeight(), 600);

    delete renderer;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST(Shader, Compilation) {
    const std::string vert_shader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}\n";

    const std::string frag_shader =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    std::string vert_path = "test_vert.glsl";
    std::string frag_path = "test_frag.glsl";
    std::ofstream vert_file(vert_path);
    std::ofstream frag_file(frag_path);
    vert_file << vert_shader;
    frag_file << frag_shader;
    vert_file.close();
    frag_file.close();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FAIL() << "SDL could not initialize! SDL_Error: " << SDL_GetError();
    }

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    REngine::Shader* shader = new REngine::Shader(vert_path.c_str(), frag_path.c_str());
    ASSERT_TRUE(shader->isValid()) << "Shader compilation failed";

    delete shader;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    std::remove(vert_path.c_str());
    std::remove(frag_path.c_str());
    SDL_Quit();
}

TEST(Scene, NodeManipulation) {
    REngine::Scene scene;
    scene.camera = REngine::Camera(640, 480);
    
    REngine::SceneNode node1;
    node1.position = glm::vec3(1, 2, 3);
    node1.rotation = glm::vec3(45, 0, 0);
    node1.scale = glm::vec3(2);
    node1.shininess = 64.0f;
    node1.distort = true;
    node1.texturePath = "test_texture.bmp";
    
    scene.nodes.push_back(node1);
    ASSERT_EQ(scene.nodes.size(), 1);
    
    EXPECT_EQ(scene.nodes[0].position, glm::vec3(1, 2, 3));
    EXPECT_EQ(scene.nodes[0].rotation, glm::vec3(45, 0, 0));
    EXPECT_EQ(scene.nodes[0].scale, glm::vec3(2));
    EXPECT_FLOAT_EQ(scene.nodes[0].shininess, 64.0f);
    EXPECT_TRUE(scene.nodes[0].distort);
    EXPECT_EQ(scene.nodes[0].texturePath, "test_texture.bmp");
    
    scene.camera.position = glm::vec3(0, 0, 5);
    EXPECT_EQ(scene.camera.position, glm::vec3(0, 0, 5));

    scene.camera.setRotation(45.0f, 30.0f, 0.0f);
    glm::vec3 rotation = scene.camera.getRotation();
    EXPECT_FLOAT_EQ(rotation.x, 45.0f);
    EXPECT_FLOAT_EQ(rotation.y, 30.0f);

    glm::mat4 view = scene.camera.getViewMatrix();
    glm::mat4 proj = scene.camera.getProjectionMatrix();
    EXPECT_NE(glm::determinant(view), 0.0f);
    EXPECT_NE(glm::determinant(proj), 0.0f);

    scene.dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    scene.dirLight.ambient = glm::vec3(0.2f);
    scene.dirLight.diffuse = glm::vec3(0.5f);
    scene.dirLight.specular = glm::vec3(1.0f);
    
    EXPECT_EQ(scene.dirLight.direction, glm::vec3(-0.2f, -1.0f, -0.3f));
    EXPECT_EQ(scene.dirLight.ambient, glm::vec3(0.2f));
    EXPECT_EQ(scene.dirLight.diffuse, glm::vec3(0.5f));
    EXPECT_EQ(scene.dirLight.specular, glm::vec3(1.0f));
}

TEST(Engine, WindowCreation) {
    REngine::Scene scene;
    
    REngine::SceneNode n_sphere, n_noTexture, n_texture, n_specular, n_alpha, n_garbage, n_garbageWithBm, n_16bit, n_notReal;
    n_texture.texturePath = "uv-test.bmp";
    n_specular.specularPath = "uv-test.bmp";
    n_alpha.texturePath = "alpha.bmp";
    n_garbage.texturePath = "garbage.bmp";
    n_garbageWithBm.texturePath = "garbage_with_bm.bmp";
    n_16bit.texturePath = "16bit.bmp";
    n_notReal.texturePath = "not_real.bmp";

    std::vector<REngine::SceneNode> nodes = {n_noTexture, n_texture, n_specular, n_alpha, n_garbage, n_garbageWithBm, n_16bit, n_notReal};

    for (auto& node : nodes) {
        node.mesh = new REngine::Mesh(REngine::Mesh::createCube());
        node.mesh->computeAABB();
        scene.nodes.push_back(node);
    }
    n_sphere.mesh = new REngine::Mesh(REngine::Mesh::createSphere());
    n_sphere.mesh->computeAABB();
    scene.nodes.push_back(n_sphere);

    scene.dirLight.direction = glm::vec3(-1, -1, -1);
    scene.dirLight.ambient = glm::vec3(0.2f);
    scene.dirLight.diffuse = glm::vec3(0.5f);
    scene.dirLight.specular = glm::vec3(1.0f);

    scene.camera = REngine::Camera(800, 600);
    scene.camera.position = glm::vec3(0, 0, 5);
    scene.camera.setRotation(0.0f, -90.0f, 0.0f);

    REngine::createWindow("test", 800, 600);
    REngine::setScene(&scene);

    REngine::setShader(NULL, NULL);

    REngine::mainLoop();

    REngine::destroyWindow();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    SDL_Quit();
    return result;
}
