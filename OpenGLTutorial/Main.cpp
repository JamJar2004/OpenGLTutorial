#include <iostream>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

        bool isClosed = false;
        while(!isClosed)
        {
            SDL_Event e;
            while(SDL_PollEvent(&e))
            {
                switch(e.type)
                {
                    case SDL_QUIT:
                        isClosed = true;
                        break;
                }
            }
        }

        SDL_DestroyWindow(window);

        SDL_Quit();
    }
    return 0;
}