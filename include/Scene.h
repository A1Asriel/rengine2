#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Mesh.h"

namespace REngine {
/// @brief Типы примитивов для отрисовки
enum class MeshType {
    /// @brief Куб
    Cube,
    /// @brief Сфера
    Sphere,
    /// @brief Пользовательский объект
    Custom
};

/// @brief Структура для хранения информации о сценном объекте
struct SceneNode {
    /// @brief Указатель на объект
    Mesh* mesh;
    /// @brief Позиция объекта
    glm::vec3 position;
    /// @brief Поворот объекта
    glm::vec3 rotation;
    /// @brief Масштаб объекта
    glm::vec3 scale;
    /// @brief Искажение текстуры
    bool distort = false;
    /// @brief Путь к текстуре
    std::string texturePath;
};

/// @brief Структура для хранения информации о камере
struct CameraNode {
    /// @brief Позиция камеры
    glm::vec3 position;
    /// @brief Поворот камеры
    glm::vec3 rotation;
    /// @brief Угол обзора камеры
    float fov;
};

/// @brief Класс для управления сценой
/// @details Хранит и управляет коллекцией сценных объектов
class Scene {
public:
    /// @brief Список объектов сцены
    std::vector<SceneNode> nodes;
    /// @brief Камера
    CameraNode camera;
};
}

#endif
