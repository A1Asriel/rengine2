#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace REngine {
/// @brief Класс для управления камерой
/// @details Предоставляет функционал для управления камерой в 3D пространстве
/// @note Использует glm для математических вычислений
class Camera {
public:
    /// @brief Позиция камеры
    glm::vec3 position;
    /// @brief Направление камеры
    glm::vec3 front;
    /// @brief Вертикальный вектор камеры
    glm::vec3 up;
    /// @brief Поле зрения камеры
    float fov;
    /// @brief Ширина окна
    int w;
    /// @brief Высота окна
    int h;

    /// @brief Конструктор камеры
    /// @param width Ширина окна
    /// @param height Высота окна
    Camera(int width, int height);

    /// @brief Получение матрицы просмотра
    /// @return glm::mat4 Матрица просмотра
    glm::mat4 getViewMatrix() const;

    /// @brief Получение матрицы проекции
    /// @return glm::mat4 Матрица проекции
    glm::mat4 getProjectionMatrix() const;

    /// @brief Относительное перемещение камеры
    /// @param dx Смещение по X
    /// @param dy Смещение по Y
    /// @param dz Смещение по Z
    void moveRelative(float dx, float dy, float dz);

    /// @brief Относительный поворот камеры
    /// @param drx Поворот по X
    /// @param dry Поворот по Y
    /// @param drz Поворот по Z
    void rotateRelative(int drx, int dry, int drz);
};
}

#endif
