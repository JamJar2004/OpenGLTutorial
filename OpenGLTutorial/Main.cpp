#include <iostream>

#define GLEW_STATIC

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

        SDL_GLContext context = SDL_GL_CreateContext(window);

        if(glewInit() != GLEW_OK)
        {
            std::cout << "Error: Glew failed to initialize." << std::endl;
            return -1;
        }

        glClearColor(0.5f, 0.0f, 1.0f, 1.0f);

        GLuint vaoID;
        glGenVertexArrays(1, &vaoID);
        glBindVertexArray(vaoID);

        glm::vec3 vertices[] =
        {
            glm::vec3(-1, -1, 0),
            glm::vec3( 0,  1, 0),
            glm::vec3( 1, -1, 0)
        };

        GLuint vboID;
        glGenBuffers(1, &vboID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

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

            glClear(GL_COLOR_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            SDL_GL_SwapWindow(window);
        }

        SDL_GL_DeleteContext(context);

        SDL_DestroyWindow(window);

        SDL_Quit();
    }
    return 0;
}

