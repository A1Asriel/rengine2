#include "CubeMesh.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

REngine::CubeMesh::CubeMesh() {
    static const float vertices[] = {
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

    static const unsigned indices[] = {
        2, 1, 0, 0, 3, 2,  // Фронт
        4, 5, 6, 6, 7, 4,  // Тыл
        10, 9, 8, 8, 11, 10,  // Лево
        14, 13, 12, 12, 15, 14,  // Право
        18, 17, 16, 16, 19, 18,  // Верх
        20, 21, 22, 22, 23, 20  // Низ
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

REngine::CubeMesh::~CubeMesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void REngine::CubeMesh::draw(const Shader& shader) {
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
