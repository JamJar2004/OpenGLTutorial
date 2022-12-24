#include <iostream>

#define GLEW_STATIC

#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        Window window(800, 600, "Test");

        if(glewInit() != GLEW_OK)
        {
            std::cout << "Error: Glew failed to initialize." << std::endl;
            return -1;
        }

        glClearColor(0.5f, 0.0f, 1.0f, 1.0f);

        std::vector<glm::vec3> vertices =
        {
            glm::vec3(-1, -1, 0),
            glm::vec3( 0,  1, 0),
            glm::vec3( 1, -1, 0)
        };

        Mesh mesh(vertices);

        std::shared_ptr<Shader> shader = 
            Shader::Load("Basic_VS.glsl", "Basic_FS.glsl");

        shader->Bind();

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

            mesh.Draw();

            window.SwapBuffers();
        }

        SDL_Quit();
    }
    return 0;
}

