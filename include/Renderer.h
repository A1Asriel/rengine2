#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include "Camera.h"
#include "Shader.h"
#include "Scene.h"

namespace REngine {
/// @brief Класс для управления рендерингом
/// @details Предоставляет функционал для рендеринга сцены
class Renderer {
private:
    /// @brief Ширина окна
    int width;
    /// @brief Высота окна
    int height;
    /// @brief Сцена для рендеринга
    Scene* scene;
    /// @brief Указатель на шейдер для рендеринга
    Shader* shader;
public:
    /// @brief Конструктор движка
    /// @param width Ширина окна
    /// @param height Высота окна
    Renderer(int width, int height);

    /// @brief Получение сцены
    /// @return Указатель на сцену
    Scene* getScene() { return scene; }

    /// @brief Установка сцены
    /// @param scene Указатель на сцену
    void setScene(Scene* scene);

    /// @brief Установка шейдера
    /// @param vertexPath Путь к вершинному шейдеру
    /// @param fragmentPath Путь к фрагментному шейдеру
    void setShader(const char* vertexPath, const char* fragmentPath);

    /// @brief Получение ширины окна
    /// @return Ширина окна
    int getWidth() { return width; }

    /// @brief Получение высоты окна
    /// @return Высота окна
    int getHeight() { return height; }

    /// @brief Отрисовка сцены
    /// @param ticks Текущее время в миллисекундах
    void draw(unsigned long ticks);
};

/// @brief Инициализация движка
/// @param width Ширина окна
/// @param height Высота окна
/// @return Указатель на инициализированный движок
Renderer* initRenderer(int width, int height);
}

#endif
