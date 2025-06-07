#include "Mesh.h"
#include <glad/glad.h>

REngine::Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned> indices) : vertices(vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    indexSize = indices.size();

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

REngine::Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void REngine::Mesh::draw(const Shader& shader) {
    glBindVertexArray(VAO);
    if (texture && texture->isValid()) {
        shader.setBool("useTexture", true);
        shader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
    } else {
        shader.setBool("useTexture", false);
    }
    if (specularTexture && specularTexture->isValid()) {
        shader.setBool("useSpecularTexture", true);
        shader.setInt("material.specular", 1);
        glActiveTexture(GL_TEXTURE1);
        specularTexture->bind();
    } else {
        shader.setBool("useSpecularTexture", false);
    }
    glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, 0);
    if (texture && texture->isValid()) {
        Texture::unbind();
    }
    if (specularTexture && specularTexture->isValid()) {
        Texture::unbind();
    }
    glBindVertexArray(0);
}

void REngine::Mesh::computeAABB() {
    min = glm::vec3(vertices[0], vertices[1], vertices[2]);
    max = glm::vec3(vertices[0], vertices[1], vertices[2]);
    for (int i = 0; i < vertices.size(); i += 8) {
        min = glm::min(min, glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
        max = glm::max(max, glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
    }
}

REngine::Mesh REngine::Mesh::createCube() {
    static const std::vector<float> vertices = {
        // Координаты         // Нормали           // UV
        // Фронт
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.25f, 0.5f,  // 0
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.25f, 0.75f, // 1
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.75f, // 2
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  // 3

        // Тыл
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.75f, 0.5f,  // 4
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.75f, 0.75f, // 5
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.75f, // 6
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.5f,  // 7

        // Лево
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.5f,  // 8
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.75f, // 9
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.25f, 0.75f, // 10
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.25f, 0.5f,  // 11

        // Право
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  // 12
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.75f, // 13
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.75f, 0.75f, // 14
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.75f, 0.5f,  // 15

        // Верх
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.25f, 0.75f, // 16
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.25f, 1.0f,  // 17
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,  // 18
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.75f, // 19

        // Низ
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.25f, 0.25f, // 20
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.25f, 0.5f,  // 21
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.5f,  0.5f,  // 22
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.5f,  0.25f  // 23
    };

    static const std::vector<unsigned> indices = {
        2, 1, 0, 0, 3, 2,  // Фронт
        4, 5, 6, 6, 7, 4,  // Тыл
        10, 9, 8, 8, 11, 10,  // Лево
        14, 13, 12, 12, 15, 14,  // Право
        18, 17, 16, 16, 19, 18,  // Верх
        20, 21, 22, 22, 23, 20  // Низ
    };
    return Mesh(vertices, indices);
}

REngine::Mesh REngine::Mesh::createSphere(int vslices, int hslices) {
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

            // Нормали
            vertices[vindex++] = x;
            vertices[vindex++] = y;
            vertices[vindex++] = z;

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
    return Mesh(vertices, indices);
}
