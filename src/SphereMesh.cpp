#include "SphereMesh.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <math.h>

void SphereMesh::init() {
    init(10, 10);
}

void SphereMesh::init(int vslices, int hslices) {
    std::vector<float> vertices((vslices + 1) * (hslices + 1) * 8);
    std::vector<unsigned> indices(vslices * hslices * 6);

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

            // Координаты
            vertices[vindex++] = x/M_PI;
            vertices[vindex++] = y/M_PI;
            vertices[vindex++] = z/M_PI;

            // Цвета
            vertices[vindex++] = sin(theta) * cos(phi);
            vertices[vindex++] = cos(theta);
            vertices[vindex++] = sin(theta) * sin(phi);

            // UV
            float u = phi / (2 * M_PI);
            float v = theta / M_PI;

            vertices[vindex++] = u;
            vertices[vindex++] = v;
        }
    }

    for (int v = 0; v < vslices; v++) {
        for (int h = 0; h < hslices; h++) {
            int v0 = v * (hslices + 1) + h;
            int v1 = v * (hslices + 1) + h + 1;
            int v2 = (v + 1) * (hslices + 1) + h;
            int v3 = (v + 1) * (hslices + 1) + h + 1;

            indices[iindex++] = v0;
            indices[iindex++] = v1;
            indices[iindex++] = v2;
            indices[iindex++] = v2;
            indices[iindex++] = v1;
            indices[iindex++] = v3;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Цвет
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    if (texture && texture->isValid()) {
        shader.setBool("useTexture", true);
        shader.setInt("textureSampler", 0);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
    } else {
        shader.setBool("useTexture", false);
    }
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    if (texture && texture->isValid()) {
        Texture::unbind();
    }
    glBindVertexArray(0);
}
