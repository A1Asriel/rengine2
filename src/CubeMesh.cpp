#include "CubeMesh.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

void CubeMesh::init() {
    std::vector<float> const vertices = {
        // Координаты
        -0.5f, -0.5f, -0.5f,  // 0
        // Цвета
        1.0f, 0.0f, 0.0f,
        // UV
        0.25f, 0.5f,

        -0.5f, 0.5f, -0.5f,  // 1
        0.0f, 1.0f, 0.0f,
        0.25f, 0.75f,

        0.5f, 0.5f, -0.5f,  // 2
        0.0f, 0.0f, 1.0f,
        0.5f, 0.75f,

        0.5f, -0.5f, -0.5f,  // 3
        1.0f, 1.0f, 0.0f,
        0.5f, 0.5f,

        -0.5f, -0.5f, 0.5f,  // 4
        1.0f, 0.0f, 1.0f,
        1.0f, 0.5f,

        -0.5f, 0.5f, 0.5f,  // 5
        0.0f, 1.0f, 1.0f,
        1.0f, 0.75f,

        0.5f, 0.5f, 0.5f,  // 6
        1.0f, 1.0f, 1.0f,
        0.75f, 0.75f,

        0.5f, -0.5f, 0.5f,  // 7
        0.5f, 0.5f, 0.5f,
        0.75f, 0.5f,

        -0.5f, 0.5f, 0.5f,  // 8
        0.0f, 1.0f, 1.0f,
        0.0f, 0.75f,

        -0.5f, -0.5f, 0.5f,  // 9
        1.0f, 0.0f, 1.0f,
        0.0f, 0.5f,

        -0.5f, 0.5f, 0.5f,  // 10
        0.0f, 1.0f, 1.0f,
        0.25f, 1.0f,

        0.5f, 0.5f, 0.5f,  // 11
        1.0f, 1.0f, 1.0f,
        0.5f, 1.0f,

        -0.5f, -0.5f, 0.5f,  // 12
        1.0f, 0.0f, 1.0f,
        0.25f, 0.25f,

        0.5f, -0.5f, 0.5f,  // 13
        0.5f, 0.5f, 0.5f,
        0.5f, 0.25f
    };

    std::vector<unsigned> const indices = {
        0, 1, 2, 2, 3, 0,  // ф
        3, 2, 6, 6, 7, 3,  // п
        7, 6, 5, 5, 4, 7,  // т
        9, 8, 1, 1, 0, 9,  // л
        1, 10, 11, 11, 2, 1,  // в
        12, 0, 3, 3, 13, 12  // н
    };

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

CubeMesh::~CubeMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void CubeMesh::draw(const Shader& shader) {
    glBindVertexArray(VAO);
    if (texture && texture->isValid()) {
        shader.setBool("useTexture", true);
        shader.setInt("textureSampler", 0);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
    } else {
        shader.setBool("useTexture", false);
    }
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    if (texture && texture->isValid()) {
        Texture::unbind();
    }
    glBindVertexArray(0);
}
