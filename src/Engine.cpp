#include "Engine.h"

#include <SDL.h>

#include "InputHandler.h"
#include "Renderer.h"
#include "Logging.h"

SDL_Window* window = nullptr;
SDL_GLContext glContext = nullptr;
REngine::Renderer* renderer = nullptr;

void moveCamera(SDL_Keycode key, float deltaTime) {
    glm::vec3 viewPos = glm::vec3(0);
    float moveSpeed = 1.0f;

    if (REngine::InputHandler::isKeyDown(SDLK_LSHIFT)) {
        moveSpeed *= 2.0f;  // Ускорение с зажатым Shift
    }

    if (key == SDLK_w)
        viewPos.z += moveSpeed * deltaTime;
    if (key == SDLK_s)
        viewPos.z -= moveSpeed * deltaTime;
    if (key == SDLK_a)
        viewPos.x -= moveSpeed * deltaTime;
    if (key == SDLK_d)
        viewPos.x += moveSpeed * deltaTime;
    if (key == SDLK_SPACE)
        viewPos.y += moveSpeed * deltaTime;
    if (key == SDLK_LCTRL)
        viewPos.y -= moveSpeed * deltaTime;

    renderer->getScene()->camera.moveRelative(viewPos.x, viewPos.y, viewPos.z);
}

int REngine::createWindow(const char* title, int width, int height) {
    if (window) {
        FATAL("Window already exists");
        return WINDOW_ALREADY_EXISTS;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FATAL("SDL could not initialize! SDL_Error: " << SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    bool fullscreen = false;
    if (width == 0 || height == 0) {
        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);
        width = displayMode.w;
        height = displayMode.h;
        fullscreen = true;
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    );

    if (!window) {
        FATAL("Window could not be created! SDL_Error: " << SDL_GetError());
        destroyWindow();
        return WINDOW_CREATE_FAILED;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        FATAL("OpenGL context could not be created! SDL_Error: " << SDL_GetError());
        destroyWindow();
        return GL_CONTEXT_CREATE_FAILED;
    }

    renderer = REngine::initRenderer(width, height);
    if (!renderer) {
        FATAL("Couldn't initialize renderer");
        destroyWindow();
        return RENDERER_INIT_FAILED;
    }

    // Инициализация системы ввода
    InputHandler::init();

    if (SDL_GL_SetSwapInterval(1) != 0) {
        ERROR("Couldn't set up Vsync: " << SDL_GetError());
    }

    return 0;
}

void REngine::mainLoop() {
    bool quit = false;  // Флаг выхода из цикла

    Uint32 previousTime = SDL_GetTicks();  // Последнее время в миллисекундах
    double deltaTime = 0.0f;  // Время с последнего кадра в секундах
    int frames = 0;  // Количество кадров за секунду

    // Закрытие окна при нажатии на Q
    InputHandler::setKeyDownCallback(SDLK_q, [&quit]() {
        quit = true;
    });

    // Перемещение камеры при удержании клавиш
    InputHandler::setKeyHoldCallback(SDLK_w, [](float deltaTime) { moveCamera(SDLK_w, deltaTime); });
    InputHandler::setKeyHoldCallback(SDLK_a, [](float deltaTime) { moveCamera(SDLK_a, deltaTime); });
    InputHandler::setKeyHoldCallback(SDLK_s, [](float deltaTime) { moveCamera(SDLK_s, deltaTime); });
    InputHandler::setKeyHoldCallback(SDLK_d, [](float deltaTime) { moveCamera(SDLK_d, deltaTime); });
    InputHandler::setKeyHoldCallback(SDLK_SPACE, [](float deltaTime) { moveCamera(SDLK_SPACE, deltaTime); });
    InputHandler::setKeyHoldCallback(SDLK_LCTRL, [](float deltaTime) { moveCamera(SDLK_LCTRL, deltaTime); });

    // Обработка движения мыши с зажатой правой кнопкой
    InputHandler::setMouseMotionCallback([&](int x, int y, int xrel, int yrel) {
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) {
            float dx = xrel / 5.0f;
            float dy = yrel / 5.0f;
            renderer->getScene()->camera.rotateRelative(dx, dy, 0);
        }
    });

    // Захват мыши при нажатии левой кнопки мыши
    InputHandler::setMouseButtonDownCallback(SDL_BUTTON_LEFT, [](int x, int y, int button) {
        if (SDL_SetRelativeMouseMode(SDL_TRUE) == 0) {
            InputHandler::setMouseMotionCallback([&](int x, int y, int xrel, int yrel) {
                float dx = xrel / 5.0f;
                float dy = yrel / 5.0f;
                renderer->getScene()->camera.rotateRelative(dx, dy, 0);
            });
        } else {
            ERROR("Failed to set relative mouse mode: " << SDL_GetError());
        }
    });

    // Отпускание мыши при нажатии клавиши ESC
    InputHandler::setKeyDownCallback(SDLK_ESCAPE, []() {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        InputHandler::setMouseMotionCallback([](int x, int y, int xrel, int yrel) {
            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) {
                float dx = xrel / 5.0f;
                float dy = yrel / 5.0f;
                renderer->getScene()->camera.rotateRelative(dx, dy, 0);
            }
        });
    });

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();  // Текущее время в миллисекундах
        deltaTime = (currentTime - previousTime) / 1000.0f;

        // Обработка ввода
        InputHandler::update(deltaTime);

        // Вывод FPS
        if (currentTime / 1000 - previousTime / 1000 >= 1) {
            INFO("FPS: " << std::to_string(frames));
            frames = 0;
        }
        frames++;
        previousTime = currentTime;

        // Отрисовка кадра
        renderer->draw(currentTime);
        SDL_GL_SwapWindow(window);
    }
}

void REngine::setScene(REngine::Scene* scene) {
    scene->camera.w = renderer->getWidth();
    scene->camera.h = renderer->getHeight();
    renderer->setScene(scene);
}

REngine::Scene* REngine::getScene() {
    return renderer->getScene();
}

void REngine::setShader(const char* vertexPath, const char* fragmentPath) {
    renderer->setShader(vertexPath, fragmentPath);
}

REngine::Shader* REngine::getShader() {
    return renderer->getShader();
}

void REngine::destroyWindow() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete renderer;
    window = NULL;
    glContext = NULL;
    renderer = NULL;
}
