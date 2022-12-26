#include "Window.hpp"

Window::Window(uint32_t width, uint32_t height, const std::string& title) : Width(width), Height(height)
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

    m_context = SDL_GL_CreateContext(m_window);
}

Window::~Window()
{
    SDL_GL_DeleteContext(m_context);

    SDL_DestroyWindow(m_window);
}

void Window::SwapBuffers()
{
    SDL_GL_SwapWindow(m_window);
}