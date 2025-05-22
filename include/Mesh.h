#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Texture.h"

namespace REngine {
/// @brief Абстрактный класс для геометрических примитивов
/// @details Предоставляет интерфейс для инициализации и отрисовки 3D объектов
class Mesh {
public:
    /// @brief Виртуальный деструктор
    virtual ~Mesh() = default;

    /// @brief Инициализация примитива
    /// @details Вызывается один раз при создании примитива
    /// @note Должен быть реализован в производных классах
    virtual void init() = 0;

    /// @brief Отрисовка примитива
    /// @param shader Используемый шейдер
    /// @note Должен быть реализован в производных классах
    virtual void draw(const Shader& shader) = 0;

    /// @brief Указатель на текстуру
    Texture* texture = nullptr;
};
}

#endif
