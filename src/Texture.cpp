#include <fstream>
#include <vector>

#include <glad/glad.h>

#include "Texture.h"
#include "Logging.h"

std::unordered_map<std::string, REngine::Texture*> REngine::Texture::textures;

REngine::Texture::Texture() : textureID(0), width(0), height(0), bpp(0) {
}

REngine::Texture::Texture(const std::string& path) : textureID(0), width(0), height(0), bpp(0) {
    loadBMP(path);
}

REngine::Texture::~Texture() {
    clear();
}

void REngine::Texture::clear() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }
}

bool REngine::Texture::loadBMP(const std::string& path) {
    clear();

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        ERROR("Failed to open BMP file: " + path);
        return false;
    }
    BMPFileHeader fileHeader;
    char* headerPtr = reinterpret_cast<char*>(&fileHeader);
    file.read(headerPtr, sizeof(fileHeader.signature));
    headerPtr += sizeof(fileHeader.signature) * 2;
    file.read(headerPtr, sizeof(fileHeader)-sizeof(fileHeader.signature)*2);

    // Проверка сигнатуры "BM"
    if (fileHeader.signature != 0x4D42) {
        ERROR("Invalid BMP signature in file: " + path);
        return false;
    }

    BMPInfoHeader infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    width = infoHeader.width;
    height = infoHeader.height;
    bpp = infoHeader.bitsPerPixel;

    if (bpp != 24 && bpp != 32) {
        ERROR("Unsupported BMP format, only 24-bit and 32-bit formats are supported: " + path);
        return false;
    }

    int rowSize = ((width * bpp / 8) + 3) & ~3;
    int dataSize = rowSize * abs(height);

    file.seekg(fileHeader.dataOffset, std::ios::beg);

    std::vector<unsigned char> pixels(dataSize);
    file.read(reinterpret_cast<char*>(pixels.data()), dataSize);
    
    if (file.fail()) {
        ERROR("Failed to read BMP data: " + path);
        return false;
    }

    std::vector<unsigned char> rgbData(width * abs(height) * (bpp == 24 ? 3 : 4));

    int pixelSize = bpp / 8;
    for (int y = 0; y < abs(height); y++) {
        for (int x = 0; x < width; x++) {
            int bmpOffset = y * rowSize + x * pixelSize;
            int rgbOffset = y * width * (bpp == 24 ? 3 : 4) + x * (bpp == 24 ? 3 : 4);

            // Конвертация BGR в RGB
            rgbData[rgbOffset] = pixels[bmpOffset + 2]; // R
            rgbData[rgbOffset + 1] = pixels[bmpOffset + 1]; // G
            rgbData[rgbOffset + 2] = pixels[bmpOffset]; // B

            if (bpp == 32) {
                rgbData[rgbOffset + 3] = pixels[bmpOffset + 3]; // Альфа
            }
        }
    }

    loadToGL(rgbData, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);

    DEBUG("Loaded BMP texture: " + path + " (" + std::to_string(width) + "x" + std::to_string(height) + ", " + std::to_string(bpp) + " bpp)");
    return true;
}

bool REngine::Texture::genFromColor(float r, float g, float b) {
    clear();
    width = 1;
    height = 1;
    bpp = 24;
    std::vector<unsigned char> rgbData(width * height * 3);
    for (int i = 0; i < width * height; i++) {
        rgbData[i * 3] = r * 255;
        rgbData[i * 3 + 1] = g * 255;
        rgbData[i * 3 + 2] = b * 255;
    }
    loadToGL(rgbData, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
    DEBUG("Generated color texture: " << r << ", " << g << ", " << b);
    return true;
}

void REngine::Texture::loadToGL(std::vector<unsigned char>& rgbData, int minFilter, int magFilter) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    GLenum format = (bpp == 24) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, rgbData.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void REngine::Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void REngine::Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
