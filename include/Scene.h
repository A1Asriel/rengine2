#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

/// @brief Типы примитивов для отрисовки
enum class MeshType {
    /// @brief Куб
    Cube,
    /// @brief Сфера
    Sphere,
    /// @brief Не реализованный тип
    NotImplemented
};

/// @brief Структура для хранения информации о сценном объекте
struct SceneNode {
    /// @brief Тип примитива
    MeshType mesh;
    /// @brief Позиция объекта
    glm::vec3 position;
    /// @brief Поворот объекта
    glm::vec3 rotation;
    /// @brief Масштаб объекта
    glm::vec3 scale;
    /// @brief Путь к текстуре
    std::string texturePath;
};

/// @brief Класс для управления сценой
/// @details Хранит и управляет коллекцией сценных объектов
class Scene {
public:
    /// @brief Список объектов сцены
    std::vector<SceneNode> nodes;
};

#endif
