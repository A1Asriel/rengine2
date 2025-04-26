#include <SDL2/SDL.h>
#include "Shader.h"
#include "Cube.h"
#include <memory>

class RE_Window
{
private:
    SDL_Window* sdl_window;
    SDL_GLContext sdl_glcontext;
    int width, height;
    std::string title;
public:
    RE_Window(std::string title, int width, int height);
    ~RE_Window();
    SDL_Window* getSDL_Window();
    SDL_GLContext getSDL_GLContext();
    int Init();
    void Draw(double deltaTime);

    Shader* shader;
    std::vector<std::unique_ptr<Cube>> vecCube;  // FIXME: Перенести кубы в отдельную структуру
};
