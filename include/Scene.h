#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Mesh.h"

#define POINT_LIGHTS_MAX 128

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
    /// @brief Степень блеска
    float shininess = 32.0f;
    /// @brief Искажение текстуры
    bool distort = false;
    /// @brief Путь к текстуре
    std::string texturePath = "";
    /// @brief Путь к текстуре отражений
    std::string specularPath = "";
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

/// @brief Структура для хранения информации о направленном источнике света
struct DirLight {
    /// @brief Направление света
    glm::vec3 direction;
    /// @brief Цвет окружающего освещения
    glm::vec3 ambient;
    /// @brief Цвет освещения
    glm::vec3 diffuse;
    /// @brief Цвет отраженного света
    glm::vec3 specular;
};

/// @brief Структура для хранения информации о точечном источнике света
struct PointLight {
    /// @brief Позиция источника света
    glm::vec3 position;
    /// @brief Константа затухания
    float constant;
    /// @brief Линейный коэффициент затухания
    float linear;
    /// @brief Квадратичный коэффициент затухания
    float quadratic;
    /// @brief Цвет окружающего освещения
    glm::vec3 ambient;
    /// @brief Цвет освещения
    glm::vec3 diffuse;
    /// @brief Цвет отраженного света
    glm::vec3 specular;
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
    /// @brief Направленный источник света
    DirLight dirLight;
    /// @brief Точечные источники света
    std::vector<PointLight> pointLights;
};
}

#endif
