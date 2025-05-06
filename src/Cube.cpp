#include "Cube.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Cube::Cube() {
    vertices = {
        -0.5f, -0.5f, -0.5f,  // Координаты
        1.0f, 0.0f, 0.0f,  // Цвета

        0.5f, -0.5f, -0.5f,
        0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, -0.5f,
        0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f,
        1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,
        1.0f, 0.0f, 1.0f,

        0.5f, -0.5f, 0.5f,
        0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f,
        1.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f
    };

    indices = {
        0, 1, 2, 2, 3, 0,  // ф
        1, 5, 6, 6, 2, 1,  // п
        5, 4, 7, 7, 6, 5,  // т
        4, 0, 3, 3, 7, 4,  // л
        3, 2, 6, 6, 7, 3,  // в
        4, 5, 1, 1, 0, 4  // н
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Цвет
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Cube::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
