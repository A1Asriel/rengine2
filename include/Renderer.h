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
public:
    /// @brief Конструктор движка
    /// @param width Ширина окна
    /// @param height Высота окна
    Renderer(int width, int height);

    /// @brief Указатель на шейдер для рендеринга
    Shader* shader;
    /// @brief Сцена для рендеринга
    Scene scene;
    /// @brief Камера для просмотра сцены
    Camera camera;

    /// @brief Инициализация движка
    /// @param procAddress Функция загрузки процедур
    /// @return 0 при успешной инициализации, код ошибки в противном случае
    int Init(GLADloadproc procAddress);

    /// @brief Отрисовка сцены
    /// @param ticks Текущее время в миллисекундах
    void Draw(unsigned long ticks);
};
}

#endif
