#ifndef ENGINE_H
#define ENGINE_H

#include "Scene.h"

namespace REngine {
    enum WindowError {
        WINDOW_ALREADY_EXISTS = -1,
        WINDOW_CREATE_FAILED = -2,
        GL_CONTEXT_CREATE_FAILED = -3,
        RENDERER_INIT_FAILED = -4
    };

    /// @brief Создание окна
    /// @param title Название окна
    /// @param width Ширина окна
    /// @param height Высота окна
    /// @return 0 при успехе, код ошибки при неудаче
    int createWindow(const char* title, int width = 0, int height = 0);

    /// @brief Основной цикл
    void mainLoop();

    /// @brief Установка сцены
    /// @param scene Указатель на сцену
    void setScene(REngine::Scene* scene);

    /// @brief Получение указателя на сцену
    /// @return Указатель на сцену
    REngine::Scene* getScene();

    /// @brief Установка шейдера
    /// @param vertexPath Путь к вершинному шейдеру
    /// @param fragmentPath Путь к фрагментному шейдеру
    void setShader(const char* vertexPath, const char* fragmentPath);

    /// @brief Получение указателя на шейдер
    /// @return Указатель на шейдер
    REngine::Shader* getShader();

    /// @brief Уничтожение окна
    void destroyWindow();
}
#endif