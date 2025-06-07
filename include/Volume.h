#ifndef VOLUME_H
#define VOLUME_H

#include <glm/glm.hpp>
#include "Camera.h"

namespace REngine {

/// @brief Плоскость
struct Plane {
    /// @brief Нормаль
    glm::vec3 normal = {0.0f, 1.0f, 0.0f};
    /// @brief Расстояние
    float distance = 0.0f;

    /// @brief Установка нормали и точки
    /// @param projView Проекционная матрица
    /// @param axis Ось
    /// @param positive Положительное
    void setNormalAndDistance(const glm::mat4& projView, uint8_t axis, bool positive);
};

/// @brief Область видимости камеры
struct Frustum {
    /// @brief Конструктор
    /// @param camera Камера
    /// @param aspect Соотношение сторон
    Frustum(const Camera& camera, float aspect);
    /// @brief Верхняя плоскость
    Plane top;
    /// @brief Нижняя плоскость
    Plane bottom;
    /// @brief Левая плоскость
    Plane left;
    /// @brief Правая плоскость
    Plane right;
    /// @brief Ближняя плоскость
    Plane near;
    /// @brief Дальняя плоскость
    Plane far;
    /// @brief Проверка, находится ли коробка в области видимости
    /// @param min Минимальные координаты коробки
    /// @param max Максимальные координаты коробки
    /// @return true, если коробка находится в области видимости
    bool isBoxInFrustum(const glm::vec3& min, const glm::vec3& max) const;
};

}
#endif