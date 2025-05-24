#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "Mesh.h"

namespace REngine {
/// @brief Класс для отрисовки сферы
/// @details Наследуется от базового класса Mesh и реализует отрисовку сферы
class SphereMesh : public Mesh {
public:
    /// @brief Деструктор сферы
    ~SphereMesh();

    /// @brief Инициализация сферы с заданным количеством сегментов
    /// @param vslices Количество вертикальных сегментов
    /// @param hslices Количество горизонтальных сегментов
    /// @details Создает VAO, VBO и EBO для отрисовки сферы
    SphereMesh(int vslices = 10, int hslices = 10);

    /// @brief Отрисовка сферы
    /// @param shader Используемый шейдер
    void draw(const Shader& shader) override;
private:
    /// @brief Vertex Array Object
    unsigned int VAO;
    /// @brief Vertex Buffer Object
    unsigned int VBO;
    /// @brief Element Buffer Object
    unsigned int EBO;
    /// @brief Количество индексов
    unsigned int indexSize;
};
}

#endif
