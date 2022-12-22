#pragma once

#include <SDL2/SDL.h>
#include <string>

class Window
{
private:
    SDL_Window*   m_window;
    SDL_GLContext m_context;
public:
    Window(uint32_t width, uint32_t height, const std::string& title);

    ~Window();

    void SwapBuffers();
};