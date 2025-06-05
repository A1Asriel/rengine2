#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace REngine {
/// @brief Класс для управления камерой
/// @details Предоставляет функционал для управления камерой в 3D пространстве
/// @note Использует glm для математических вычислений
class Camera {
private:
    /// @brief Направление камеры
    glm::vec3 front;
    /// @brief Вертикальный вектор камеры
    glm::vec3 up;

public:
    /// @brief Позиция камеры
    glm::vec3 position;
    /// @brief Поле зрения камеры
    float fov;
    /// @brief Ширина окна
    int w;
    /// @brief Высота окна
    int h;

    /// @brief Конструктор камеры
    Camera();

    /// @brief Конструктор камеры
    /// @param width Ширина окна
    /// @param height Высота окна
    Camera(int width, int height);

    /// @brief Установка углов поворота камеры
    /// @param rx Угол поворота по X в градусах
    /// @param ry Угол поворота по Y в градусах
    /// @param rz Угол поворота по Z в градусах
    void setRotation(float rx, float ry, float rz);

    /// @brief Получение углов поворота камеры
    /// @return glm::vec3 Углы поворота по X, Y и Z в градусах
    glm::vec3 getRotation() const;

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
    /// @param drx Поворот по X в градусах
    /// @param dry Поворот по Y в градусах
    /// @param drz Поворот по Z в градусах
    void rotateRelative(float drx, float dry, float drz);
};
}

#endif
