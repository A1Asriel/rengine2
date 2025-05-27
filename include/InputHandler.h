#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <functional>
#include <unordered_map>
#include <SDL.h>
#include <glm/glm.hpp>

namespace REngine {

/// @brief Класс для обработки пользовательского ввода
class InputHandler {
public:
    /// @brief Метод, вызываемый при нажатии клавиши
    using KeyCallback = std::function<void()>;
    /// @brief Метод, вызываемый при удержании клавиши
    using HoldKeyCallback = std::function<void(float)>;
    /// @brief Метод, вызываемый при движении мыши
    using MouseMotionCallback = std::function<void(int, int, int, int)>;
    /// @brief Метод, вызываемый при нажатии кнопки мыши
    using MouseButtonCallback = std::function<void(int, int, int)>;
    /// @brief Метод, вызываемый при прокрутке колесика мыши
    using MouseWheelCallback = std::function<void(int, int)>;

    /// @brief Инициализация обработчика ввода
    static void init();

    /// @brief Обработка событий ввода
    /// @param event Событие SDL
    /// @return true если событие было обработано, иначе false
    static bool handleEvent(const SDL_Event& event);

    /// @brief Обновление состояния ввода
    /// @param deltaTime Время с прошлого кадра
    static void update(float deltaTime);

    /// @brief Установка метода для нажатия клавиши
    /// @param key Код клавиши SDL
    /// @param callback Функция-обработчик
    static void setKeyDownCallback(SDL_Keycode key, KeyCallback callback);

    /// @brief Установка метода для отжатия клавиши
    /// @param key Код клавиши SDL
    /// @param callback Функция-обработчик
    static void setKeyUpCallback(SDL_Keycode key, KeyCallback callback);

    /// @brief Установка метода для удержания клавиши
    /// @param key Код клавиши SDL
    /// @param callback Функция-обработчик
    static void setKeyHoldCallback(SDL_Keycode key, HoldKeyCallback callback);

    /// @brief Установка метода для движения мыши
    /// @param callback Функция-обработчик (x, y, xrel, yrel)
    static void setMouseMotionCallback(MouseMotionCallback callback);

    /// @brief Установка метода для нажатия кнопки мыши
    /// @param button Код кнопки мыши SDL
    /// @param callback Функция-обработчик (x, y, button)
    static void setMouseButtonDownCallback(Uint8 button, MouseButtonCallback callback);

    /// @brief Установка метода для отжатия кнопки мыши
    /// @param button Код кнопки мыши SDL
    /// @param callback Функция-обработчик (x, y, button)
    static void setMouseButtonUpCallback(Uint8 button, MouseButtonCallback callback);

    /// @brief Установка метода для прокрутки колесика мыши
    /// @param callback Функция-обработчик (x, y)
    static void setMouseWheelCallback(MouseWheelCallback callback);

    /// @brief Проверка нажатия клавиши
    /// @param key Код клавиши SDL
    /// @return true, если клавиша нажата
    static bool isKeyDown(SDL_Keycode key);

    /// @brief Получение позиции мыши
    /// @return Позиция мыши в виде glm::vec2
    static glm::vec2 getMousePosition();

    /// @brief Получение относительного перемещения мыши
    /// @return Перемещение мыши в виде glm::vec2
    static glm::vec2 getMouseRelativeMotion();

private:
    static std::unordered_map<SDL_Keycode, bool> keyStates_;
    static std::unordered_map<SDL_Keycode, KeyCallback> keyDownCallbacks_;
    static std::unordered_map<SDL_Keycode, KeyCallback> keyUpCallbacks_;
    static std::unordered_map<SDL_Keycode, HoldKeyCallback> keyHoldCallbacks_;
    static std::unordered_map<Uint8, MouseButtonCallback> mouseButtonDownCallbacks_;
    static std::unordered_map<Uint8, MouseButtonCallback> mouseButtonUpCallbacks_;
    static MouseMotionCallback mouseMotionCallback_;
    static MouseWheelCallback mouseWheelCallback_;
    static glm::vec2 mousePosition_;
    static glm::vec2 mouseRelativeMotion_;
};

}

#endif
