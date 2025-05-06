#ifndef MESH_H
#define MESH_H

#include "Shader.h"

class Mesh {
public:
    virtual ~Mesh() = default;
    virtual void init() = 0;
    virtual void draw(const Shader& shader) = 0;
};
#endif
