#include "SphereMesh.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <math.h>

void SphereMesh::init() {
    init(10, 10);
}

void SphereMesh::init(int vslices, int hslices) {
    std::vector<float> vertices;
    std::vector<unsigned> indices;

    int vindex = 0;
    int iindex = 0;

    float theta, phi, x, y, z;

    for (int v = 0; v <= vslices; v++) {
        for (int h = 0; h <= hslices; h++) {
            theta = M_PI * (float)v / (float)vslices;
            phi = M_PI*2 * (float)h / (float)hslices;

            x = sin(theta) * cos(phi);
            y = cos(theta);
            z = sin(theta) * sin(phi);

            vertices.push_back(x/M_PI);
            vertices.push_back(y/M_PI);
            vertices.push_back(z/M_PI);
            vertices.push_back(sin(theta) * cos(phi));
            vertices.push_back(cos(theta));
            vertices.push_back(sin(theta) * sin(phi));
        }
    }

    for (int v = 0; v < vslices; v++) {
        for (int h = 0; h < hslices; h++) {
            int v0 = v * (hslices + 1) + h;
            int v1 = v * (hslices + 1) + h + 1;
            int v2 = (v + 1) * (hslices + 1) + h;
            int v3 = (v + 1) * (hslices + 1) + h + 1;

            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v2);
            indices.push_back(v1);
            indices.push_back(v3);
        }
    }

    indexSize = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Положение
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Цвет
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SphereMesh::~SphereMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void SphereMesh::draw(const Shader& shader) {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
