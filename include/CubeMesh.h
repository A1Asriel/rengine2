#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"

namespace REngine {
/// @brief Класс для отрисовки куба
/// @details Наследуется от базового класса Mesh и реализует отрисовку куба
class CubeMesh : public Mesh {
public:
    /// @brief Деструктор куба
    ~CubeMesh();

    /// @brief Инициализация куба
    CubeMesh();

    /// @brief Отрисовка куба
    /// @param shader Используемый шейдер
    void draw(const Shader& shader) override;
private:
    /// @brief Vertex Array Object
    unsigned int VAO;
    /// @brief Vertex Buffer Object
    unsigned int VBO;
    /// @brief Element Buffer Object
    unsigned int EBO;
};
}

#endif