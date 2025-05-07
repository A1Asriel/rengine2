#ifndef RE_WINDOW_H
#define RE_WINDOW_H

#include <SDL2/SDL.h>

#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Scene.h"

/// @brief Класс для управления окном и рендерингом
/// @details Предоставляет функционал для создания и управления окном, а также рендеринга сцены
/// @note Использует SDL2 для создания окна и OpenGL контекста
class RE_Window {
private:
    /// @brief Указатель на SDL окно
    SDL_Window* sdl_window;
    /// @brief OpenGL контекст
    SDL_GLContext sdl_glcontext;
    /// @brief Ширина окна
    int width;
    /// @brief Высота окна
    int height;
    /// @brief Заголовок окна
    std::string title;
public:
    /// @brief Конструктор окна
    /// @param title Заголовок окна
    /// @param width Ширина окна
    /// @param height Высота окна
    RE_Window(std::string title, int width, int height);

    /// @brief Деструктор окна
    ~RE_Window();

    /// @brief Получение указателя на SDL окно
    /// @return SDL_Window*
    SDL_Window* getSDL_Window();

    /// @brief Получение OpenGL контекста
    /// @return SDL_GLContext
    SDL_GLContext getSDL_GLContext();

    /// @brief Указатель на шейдер для рендеринга
    Shader* shader;
    /// @brief Указатель на сцену для рендеринга
    Scene* scene;
    /// @brief Камера для просмотра сцены
    Camera camera;

    /// @brief Инициализация окна и OpenGL контекста
    /// @return 0 при успешной инициализации, код ошибки в противном случае
    int Init();

    /// @brief Отрисовка сцены
    /// @param deltaTime Время между кадрами
    void Draw(double deltaTime);
};

#endif
