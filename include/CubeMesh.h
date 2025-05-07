#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"

/// @brief Класс для отрисовки куба
/// @details Наследуется от базового класса Mesh и реализует отрисовку куба
class CubeMesh : public Mesh {
public:
    /// @brief Деструктор куба
    ~CubeMesh();

    /// @brief Инициализация куба
    /// @details Создает VAO, VBO и EBO для отрисовки куба
    void init() override;

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

#endif