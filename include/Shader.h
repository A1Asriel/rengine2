#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>
#include <string>

/// @brief Класс для работы с шейдерами
/// @details Предоставляет функционал для загрузки и использования шейдерных программ
class Shader {
public:
    /// @brief Идентификатор шейдера
    unsigned int ID;
    
    /// @brief Конструктор шейдера
    /// @param vertexPath Путь к вершинному шейдеру
    /// @param fragmentPath Путь к фрагментному шейдеру
    Shader(const char* vertexPath, const char* fragmentPath);
    
    /// @brief Активация шейдера
    void use();
    
    /// @brief Установка булевого значения
    /// @param name Имя переменной
    /// @param value Значение
    void setBool(const std::string &name, bool value) const;
    
    /// @brief Установка целочисленного значения
    /// @param name Имя переменной
    /// @param value Значение
    void setInt(const std::string &name, int value) const;
    
    /// @brief Установка числового значения
    /// @param name Имя переменной
    /// @param value Значение
    void setFloat(const std::string &name, float value) const;
    
    /// @brief Установка матрицы 4x4
    /// @param name Имя переменной
    /// @param mat Матрица
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
    /// @brief Установка вектора из 3 компонентов
    /// @param name Имя переменной
    /// @param value Вектор
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    
    /// @brief Проверка валидности шейдера
    /// @return true если шейдер валиден, false в противном случае
    bool isValid() const { return ID != 0; }
    
private:
    /// @brief Проверка ошибок компиляции шейдера
    /// @param shader Идентификатор шейдера
    /// @param type Тип шейдера
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif