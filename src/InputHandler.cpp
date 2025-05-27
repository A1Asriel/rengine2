#include "InputHandler.h"
#include "Logging.h"

namespace REngine {

// Инициализация статических членов класса
std::unordered_map<SDL_Keycode, bool> InputHandler::keyStates_;
std::unordered_map<SDL_Keycode, InputHandler::KeyCallback> InputHandler::keyDownCallbacks_;
std::unordered_map<SDL_Keycode, InputHandler::KeyCallback> InputHandler::keyUpCallbacks_;
std::unordered_map<SDL_Keycode, InputHandler::HoldKeyCallback> InputHandler::keyHoldCallbacks_;
std::unordered_map<Uint8, InputHandler::MouseButtonCallback> InputHandler::mouseButtonDownCallbacks_;
std::unordered_map<Uint8, InputHandler::MouseButtonCallback> InputHandler::mouseButtonUpCallbacks_;
InputHandler::MouseMotionCallback InputHandler::mouseMotionCallback_;
InputHandler::MouseWheelCallback InputHandler::mouseWheelCallback_;
glm::vec2 InputHandler::mousePosition_;
glm::vec2 InputHandler::mouseRelativeMotion_;

void InputHandler::init() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    
    // Инициализация позиции мыши
    int x, y;
    SDL_GetMouseState(&x, &y);
    mousePosition_ = glm::vec2(x, y);
    mouseRelativeMotion_ = glm::vec2(0, 0);
}

bool InputHandler::handleEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN: {
            SDL_Keycode key = event.key.keysym.sym;
            keyStates_[key] = true;

            auto it = keyDownCallbacks_.find(key);
            if (it != keyDownCallbacks_.end() && it->second) {
                it->second();
                return true;
            }
            break;
        }
        case SDL_KEYUP: {
            SDL_Keycode key = event.key.keysym.sym;
            keyStates_[key] = false;

            auto it = keyUpCallbacks_.find(key);
            if (it != keyUpCallbacks_.end() && it->second) {
                it->second();
                return true;
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            mousePosition_.x = event.motion.x;
            mousePosition_.y = event.motion.y;
            mouseRelativeMotion_.x = event.motion.xrel;
            mouseRelativeMotion_.y = event.motion.yrel;

            if (mouseMotionCallback_) {
                mouseMotionCallback_(event.motion.x, event.motion.y, 
                                   event.motion.xrel, event.motion.yrel);
                return true;
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            Uint8 button = event.button.button;
            auto it = mouseButtonDownCallbacks_.find(button);
            if (it != mouseButtonDownCallbacks_.end() && it->second) {
                it->second(event.button.x, event.button.y, button);
                return true;
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            Uint8 button = event.button.button;
            auto it = mouseButtonUpCallbacks_.find(button);
            if (it != mouseButtonUpCallbacks_.end() && it->second) {
                it->second(event.button.x, event.button.y, button);
                return true;
            }
            break;
        }
        case SDL_MOUSEWHEEL: {
            if (mouseWheelCallback_) {
                mouseWheelCallback_(event.wheel.x, event.wheel.y);
                return true;
            }
            break;
        }
        default:
            return false;
    }
    return false;
}

void InputHandler::update(float deltaTime) {
    mouseRelativeMotion_ = glm::vec2(0, 0);

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        handleEvent(e);
    }

    // Обрабатываем удержание клавиш
    for (auto& [key, callback] : keyHoldCallbacks_) {
        if (keyStates_[key]) {
            callback(deltaTime);
        }
    }
}

void InputHandler::setKeyDownCallback(SDL_Keycode key, KeyCallback callback) {
    keyDownCallbacks_[key] = callback;
}

void InputHandler::setKeyUpCallback(SDL_Keycode key, KeyCallback callback) {
    keyUpCallbacks_[key] = callback;
}

void InputHandler::setKeyHoldCallback(SDL_Keycode key, HoldKeyCallback callback) {
    keyHoldCallbacks_[key] = callback;
}

void InputHandler::setMouseMotionCallback(MouseMotionCallback callback) {
    mouseMotionCallback_ = callback;
}

void InputHandler::setMouseButtonDownCallback(Uint8 button, MouseButtonCallback callback) {
    mouseButtonDownCallbacks_[button] = callback;
}

void InputHandler::setMouseButtonUpCallback(Uint8 button, MouseButtonCallback callback) {
    mouseButtonUpCallbacks_[button] = callback;
}

void InputHandler::setMouseWheelCallback(MouseWheelCallback callback) {
    mouseWheelCallback_ = callback;
}

bool InputHandler::isKeyDown(SDL_Keycode key) {
    auto it = keyStates_.find(key);
    return it != keyStates_.end() ? it->second : false;
}

glm::vec2 InputHandler::getMousePosition() {
    return mousePosition_;
}

glm::vec2 InputHandler::getMouseRelativeMotion() {
    return mouseRelativeMotion_;
}

}
