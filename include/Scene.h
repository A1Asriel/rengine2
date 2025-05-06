#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

enum class MeshType { Cube, Sphere, NotImplemented };

struct SceneNode {
    MeshType mesh;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    std::string texturePath;
};

class Scene {
public:
    std::vector<SceneNode> nodes;
};

#endif
