#include <iostream>
#include <chrono>

#define GLEW_STATIC

#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Transformation.hpp"
#include "Camera.hpp"

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
        glClearColor(0.5f, 0.0f, 1.0f, 1.0f);

        /*std::vector<glm::vec3> vertices =
        {
            glm::vec3(-1, -1, 0),
            glm::vec3(-1,  1, 0),
            glm::vec3( 1, -1, 0),
			glm::vec3( 1,  1, 0)
        };

		std::vector<GLuint> indices =
		{
			0, 1, 3,
			0, 3, 2
		};

        Mesh mesh(vertices, indices);*/

		std::shared_ptr<Mesh> cube = Mesh::CreateCube();

		Transformation transformation;

		Camera camera(Transformation(glm::vec3(0, 0, 5)),
			glm::perspective(glm::radians(70.0f), window.Width / float(window.Height), 0.1f, 1000.0f));

        std::shared_ptr<Shader> shader = 
            Shader::Load("Basic_VS.glsl", "Basic_FS.glsl");

        shader->Bind();

		float x = 0;

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

				x += float(frameTime);
				//transformation.Position.x = sinf(x);
				transformation.Rotation = glm::angleAxis(glm::radians(sinf(x) * 180.0f), glm::vec3(0, 1, 0));

				if(fpsTimeCounter >= 1.0f)
				{
					std::cout << "FPS: " << fps << std::endl;
					fpsTimeCounter = 0.0f;
					fps = 0;
				}
			}

			if(shouldRender)
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				shader->SetUniform("u_WVP", camera.GetViewProjection() * transformation.ToMatrix());
				cube->Draw();
				window.SwapBuffers();
				fps++;
			}
		}

        SDL_Quit();
    }
    return 0;
}

