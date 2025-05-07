#ifndef CAMERA_H
#define CAMERA_H

#include <SDL_stdinc.h>

#include <glm/glm.hpp>

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
    /// @brief Угол наклона камеры
    float pitch;
    /// @brief Угол поворота камеры
    float yaw;
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

    /// @brief Обработка клавиатурного ввода
    /// @param deltaTime Время между кадрами
    /// @param keystate Состояние клавиш
    void processKeyboard(float deltaTime, const Uint8* keystate);

    /// @brief Обработка ввода мыши
    /// @param dx Смещение по X
    /// @param dy Смещение по Y
    void processMouse(int dx, int dy);
};

#endif
