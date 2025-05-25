#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>

namespace REngine {
/// @brief Заголовок файла BMP
struct BMPFileHeader {
    uint16_t signature;      // Сигнатура файла
    uint32_t fileSize;       // Размер файла в байтах
    uint32_t reserved;       // Зарезервированные поля
    uint32_t dataOffset;     // Смещение до данных пикселей
};

/// @brief Заголовок информации BMP
struct BMPInfoHeader {
    uint32_t headerSize;     // Размер заголовка
    int32_t width;           // Ширина изображения
    int32_t height;          // Высота изображения
    uint16_t planes;         // Количество плоскостей (должно быть 1)
    uint16_t bitsPerPixel;   // Количество битов на пиксель
    uint32_t compression;    // Метод сжатия
    uint32_t imageSize;      // Размер изображения
    int32_t xPixelsPerMeter; // Горизонтальное разрешение
    int32_t yPixelsPerMeter; // Вертикальное разрешение
    uint32_t colorsUsed;     // Количество используемых цветов
    uint32_t colorsImportant;// Количество важных цветов
};

/// @brief Класс текстуры
class Texture {
public:
    /// @brief Конструктор по умолчанию
    Texture();

    /// @brief Конструктор, загружающий текстуру из файла
    /// @param path Путь к файлу текстуры
    Texture(const std::string& path);

    /// @brief Деструктор
    ~Texture();

    /// @brief Загрузка текстуры из файла
    /// @param path Путь к файлу текстуры
    /// @return true на успех, false на неудачу
    bool loadBMP(const std::string& path);

    /// @brief Генерация текстуры из цвета
    /// @param r Красный канал
    /// @param g Зеленый канал
    /// @param b Синий канал
    /// @return true на успех, false на неудачу
    bool genFromColor(float r, float g, float b);

    /// @brief Привязка текстуры для использования
    void bind() const;

    /// @brief Отвязка текстуры
    static void unbind();

    /// @brief Проверка валидности текстуры
    /// @return true если текстура валидна, false в противном случае
    bool isValid() const { return textureID != 0; }

    /// @brief Получение ширины текстуры
    /// @return Ширина в пикселях
    int getWidth() const { return width; }

    /// @brief Получение высоты текстуры
    /// @return Высота в пикселях
    int getHeight() const { return height; }

    /// @brief Хранение текстур
    static std::unordered_map<std::string, Texture*> textures;

private:
    /// @brief ID текстуры
    unsigned int textureID;

    /// @brief Ширина текстуры
    int width;

    /// @brief Высота текстуры
    int height;

    /// @brief Количество битов на пиксель
    int bpp;

    /// @brief Загрузка текстуры в OpenGL
    /// @param rgbData Данные текстуры
    /// @param minFilter Фильтрация при уменьшении
    /// @param magFilter Фильтрация при увеличении
    void loadToGL(std::vector<unsigned char>& rgbData, int minFilter, int magFilter);

    /// @brief Очистка текстуры
    void clear();
};
}

#endif
