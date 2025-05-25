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
    glm::vec3 position = glm::vec3(0.0f);
    /// @brief Поворот объекта
    glm::vec3 rotation = glm::vec3(0.0f);
    /// @brief Масштаб объекта
    glm::vec3 scale = glm::vec3(1.0f);
    /// @brief Коэффициент окружающей среды
    glm::vec3 ambient = glm::vec3(1.0f);
    /// @brief Коэффициент диффузного освещения
    glm::vec3 diffuse = glm::vec3(1.0f);
    /// @brief Коэффициент зеркального отражения
    glm::vec3 specular = glm::vec3(1.0f);
    /// @brief Степень блеска
    float shininess = 32.0f;
    /// @brief Искажение текстуры
    bool distort = false;
    /// @brief Путь к текстуре
    std::string texturePath = "";
};

/// @brief Структура для хранения информации о камере
struct CameraNode {
    /// @brief Позиция камеры
    glm::vec3 position = glm::vec3(0.0f);
    /// @brief Поворот камеры
    glm::vec3 rotation = glm::vec3(0.0f);
    /// @brief Угол обзора камеры
    float fov = 45.0f;
};

/// @brief Класс для управления сценой
/// @details Хранит и управляет коллекцией сценных объектов
class Scene {
public:
    /// @brief Список объектов сцены
    std::vector<SceneNode> nodes = {};
    /// @brief Камера
    CameraNode camera;
    /// @brief Цвет неба
    glm::vec3 skyColor = glm::vec3(0.63f, 0.63f, 0.85f);
    /// @brief Цвет освещения
    glm::vec3 lightingColor = glm::vec3(1.0f);
    /// @brief Позиция источника света
    glm::vec3 lightingPosition = glm::vec3(0.0f);
};
}

#endif
