#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"

class CubeMesh : public Mesh {
public:
    ~CubeMesh();
    void init() override;
    void draw(const Shader& shader) override;
private:
    unsigned int VAO, VBO, EBO;
};

#endif