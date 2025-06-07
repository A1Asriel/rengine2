#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Texture.h"

namespace REngine {
/// @brief Класс для геометрических примитивов
/// @details Предоставляет интерфейс для инициализации и отрисовки 3D объектов
class Mesh {
public:
    /// @brief Конструктор
    /// @param vertices Вектор вершин
    /// @param indices Вектор индексов
    Mesh(std::vector<float> vertices, std::vector<unsigned> indices);

    /// @brief Деструктор
    ~Mesh();

    /// @brief Указатель на текстуру
    Texture* texture = nullptr;
    /// @brief Указатель на текстуру отражений
    Texture* specularTexture = nullptr;

    /// @brief Отрисовка
    /// @param shader Используемый шейдер
    void draw(const Shader& shader);

    /// @brief Вычисление AABB
    void computeAABB();

    /// @brief Получение минимальных координат AABB
    /// @return Минимальные координаты AABB
    const glm::vec3& getMin() const { return min; }
    /// @brief Получение максимальных координат AABB
    /// @return Максимальные координаты AABB
    const glm::vec3& getMax() const { return max; }

    /// @brief Создание куба
    /// @return Куб
    static Mesh createCube();
    /// @brief Создание сферы
    /// @param vslices Количество вертикальных сегментов
    /// @param hslices Количество горизонтальных сегментов
    /// @return Сфера
    static Mesh createSphere(int vslices = 100, int hslices = 100);
private:
    /// @brief Вектор вершин
    std::vector<float> vertices;
    /// @brief Vertex Array Object
    unsigned int VAO;
    /// @brief Vertex Buffer Object
    unsigned int VBO;
    /// @brief Element Buffer Object
    unsigned int EBO;
    /// @brief Количество индексов
    unsigned int indexSize;
    /// @brief Минимальные координаты AABB
    glm::vec3 min;
    /// @brief Максимальные координаты AABB
    glm::vec3 max;
};
}

#endif
