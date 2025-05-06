#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "Mesh.h"

class SphereMesh : public Mesh {
public:
    ~SphereMesh();
    void init() override;
    void init(int vslices, int hslices);
    void draw(const Shader& shader) override;
private:
    unsigned int VAO, VBO, EBO, indexSize;
};

#endif
