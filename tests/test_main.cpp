#include <SDL.h>
#include <gtest/gtest.h>

#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "RE_Window.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "Shader.h"

TEST(SceneLoader, LoadScene) {
    const std::string fname = "test_scene.csv";
    std::ofstream f(fname);
    f << "[RENGINE MAP FORMAT V1.0]\n";
    f << "camera,0,1,3,not working\n";
    f << "camera,0,1,3,a,b,c,d\n";
    f << "camera,0,0,0,0,0,0,45\n";
    f << "cube,0,0,0,0,0,0,1,1,1,false,uv-test.bmp\n";
    f << "cube,7,8,9,0,0\n";
    f << "sphere,1,2,3,0,0,0,2,2,2,false\n";
    f << "notfound,4,5,6,0,0,0,3,3,3,false\n";
    f << "\n";
    f << "cube,10,11,12,0,a0,0,1,1,1,true\n";
    f.close();

    Scene scene;
    ASSERT_TRUE(SceneLoader::load(fname, &scene));
    ASSERT_EQ(scene.nodes.size(), 2);
    EXPECT_EQ(scene.nodes[0].mesh, MeshType::Cube);
    EXPECT_EQ(scene.nodes[1].mesh, MeshType::Sphere);

    std::remove(fname.c_str());
}

TEST(SceneLoader, InvalidScene) {
    const std::string fname = "test_scene.csv";
    std::ofstream f(fname);
    f << "[RENGINE MAP FORMAT V2.0]\n";
    f << "camera,0,1,3,not working\n";
    f << "camera,0,1,3,a,b,c,d\n";
    f << "camera,0,0,0,0,0,0,45\n";
    f << "cube,0,0,0,0,0,0,1,1,1,false,uv-test.bmp\n";
    f << "cube,7,8,9,0,0\n";
    f << "sphere,1,2,3,0,0,0,2,2,2,false\n";
    f << "notfound,4,5,6,0,0,0,3,3,3,false\n";
    f << "\n";
    f << "cube,10,11,12,0,a0,0,1,1,1,true\n";
    f.close();

    Scene scene;
    ASSERT_FALSE(SceneLoader::load(fname, &scene));
    std::remove(fname.c_str());
}

TEST(Camera, DefaultViewProjection) {
    const int w = 800, h = 600;
    Camera cam(w, h);
    glm::mat4 view = cam.getViewMatrix();
    glm::mat4 expectedView = glm::lookAt(cam.position, cam.position + cam.front, cam.up);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_NEAR(view[i][j], expectedView[i][j], 1e-5f);
        }
    }

    glm::mat4 proj = cam.getProjectionMatrix();
    float f = 1.0f / std::tan(glm::radians(cam.fov) / 2.0f);
    EXPECT_NEAR(proj[1][1], f, 1e-5f);
    EXPECT_NEAR(proj[0][0], f / (float(w) / float(h)), 1e-5f);
}

TEST(Renderer, InitAndDraw) {
    Renderer* window = new Renderer("Test Window", 640, 480);
    ASSERT_EQ(window->init(), 0) << "Window initialization failed";
    ASSERT_NE(window->getSDL_Window(), nullptr);
    ASSERT_NE(window->getSDL_GLContext(), nullptr);

    window->shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    ASSERT_TRUE(window->shader->isValid()) << "Shader initialization failed";

    Scene scene;
    scene.nodes.push_back({MeshType::Cube, glm::vec3(0), glm::vec3(0), glm::vec3(1), false});
    scene.nodes.push_back({MeshType::Sphere, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), false, "uv-test.bmp"});
    scene.nodes.push_back({MeshType::Cube, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), true, "uv-test.bmp"});
    scene.nodes.push_back({MeshType::Cube, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), true, "alpha.bmp"});
    scene.nodes.push_back({MeshType::Cube, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), true, "garbage.bmp"});
    scene.nodes.push_back({MeshType::Cube, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), true, "garbage_with_bm.bmp"});
    scene.nodes.push_back({MeshType::Cube, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), true, "16bit.bmp"});
    scene.nodes.push_back({MeshType::Cube, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2), true, "not_real.bmp"});
    window->scene = &scene;
    window->draw(0);

    delete window->shader;
    delete window;
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

    Shader* shader = new Shader(vert_path.c_str(), frag_path.c_str());
    ASSERT_TRUE(shader->isValid()) << "Shader compilation failed";

    delete shader;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    std::remove(vert_path.c_str());
    std::remove(frag_path.c_str());
    SDL_Quit();
}

TEST(Shader, InvalidPathShader) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FAIL() << "SDL could not initialize! SDL_Error: " << SDL_GetError();
    }

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    Shader* shader = new Shader("invalid.vert", "invalid.frag");
    ASSERT_TRUE(shader->isValid()) << "Shader should be valid";

    delete shader;
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST(Scene, Transformations) {
    Scene scene;

    scene.nodes.push_back({MeshType::Cube, glm::vec3(0), glm::vec3(0), glm::vec3(1)});
    scene.nodes.push_back({MeshType::Sphere, glm::vec3(1, 2, 3), glm::vec3(45, 0, 0), glm::vec3(2)});

    scene.nodes[0].position = glm::vec3(1, 2, 3);
    scene.nodes[0].rotation = glm::vec3(45, 0, 0);
    scene.nodes[0].scale = glm::vec3(2);

    EXPECT_EQ(scene.nodes[0].position, glm::vec3(1, 2, 3));
    EXPECT_EQ(scene.nodes[0].rotation, glm::vec3(45, 0, 0));
    EXPECT_EQ(scene.nodes[0].scale, glm::vec3(2));
    Camera cam(800, 600);
    cam.moveRelative(0.1f, SDL_GetKeyboardState(NULL));
    cam.rotateRelative(10, 10, 10);

    glm::mat4 view = cam.getViewMatrix();
    glm::mat4 proj = cam.getProjectionMatrix();

    glm::vec4 camOrigin = view * glm::vec4(cam.position, 1);
    EXPECT_TRUE(glm::all(glm::epsilonEqual(camOrigin, glm::vec4(0, 0, 0, 1), glm::epsilon<float>())));
    EXPECT_GT(proj[1][1], 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    SDL_Quit();
    return result;
}
