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

    /// @brief Отрисовка примитива
    /// @param shader Используемый шейдер
    /// @note Должен быть реализован в производных классах
    virtual void draw(const Shader& shader) = 0;

    /// @brief Указатель на текстуру
    Texture* texture = nullptr;
    /// @brief Указатель на текстуру отражений
    Texture* specularTexture = nullptr;
};
}

#endif
