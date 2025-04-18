#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <glm/glm.hpp>

class Cube {
public:
    Cube();
    ~Cube();

    void draw();
    void update(float deltaTime);

    glm::mat4 getModelMatrix() const;

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float rotationAngle;
};

#endif