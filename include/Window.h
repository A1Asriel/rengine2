#ifndef WINDOW_H
#define WINDOW_H

#include "Renderer.h"

namespace REngine {
    enum WindowError {
        WINDOW_ALREADY_EXISTS = -1,
        WINDOW_CREATE_FAILED = -2,
        GL_CONTEXT_CREATE_FAILED = -3,
        RENDERER_INIT_FAILED = -4
    };

    extern Renderer* renderer;

    /// @brief Создание окна
    /// @param title Название окна
    /// @param width Ширина окна
    /// @param height Высота окна
    /// @return 0 при успехе, код ошибки при неудаче
    int createWindow(const char* title, int width = 0, int height = 0);
    /// @brief Основной цикл
    void mainLoop();
    /// @brief Уничтожение окна
    void destroyWindow();
}
#endif