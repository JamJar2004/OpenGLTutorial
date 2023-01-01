#include <iostream>
#include <chrono>

#define GLEW_STATIC

#include "Window.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Transformation.hpp"
#include "Camera.hpp"
#include "Scene.hpp"

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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		Scene scene(window);

        const double frameTime = 1.0f / 60.0f;

		uint32_t fps = 0;
		std::chrono::time_point lastTime = std::chrono::system_clock::now();
		double fpsTimeCounter = 0;
		double updateTimer = 1;

		bool isClosed = false;
		while(!isClosed)
		{
			std::chrono::time_point currentTime = std::chrono::system_clock::now();
			double passedTime = std::chrono::duration<double>(currentTime - lastTime).count();
			lastTime = currentTime;

			updateTimer    += passedTime;
			fpsTimeCounter += passedTime;

			bool shouldRender = false;
			while(updateTimer >= frameTime)
			{
				shouldRender = true;

				updateTimer -= frameTime;

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

				scene.Update(frameTime);

				if(fpsTimeCounter >= 1.0f)
				{
					std::cout << "FPS: " << fps << std::endl;
					fpsTimeCounter = 0.0f;
					fps = 0;
				}
			}

			if(shouldRender)
			{
				scene.Render(window);
				window.SwapBuffers();
				fps++;
			}
		}

        SDL_Quit();
    }
    return 0;
}

