#include "Scene.hpp"

float randf()
{
	return rand() / float(RAND_MAX);
}

Scene::Scene(const Window& window) :
	camera(Transformation(glm::vec3(0, 0, 0)), glm::perspective(glm::radians(70.0f), window.Width / float(window.Height), 0.1f, 1000.0f)),
	x(0)
{
	for(size_t i = 0; i < 100; i++)
		CreateEntity(Transformation(glm::vec3(randf() * 20.0f - 10.0f, randf() * 20.0f - 10.0f, -randf() * 20.0f - 5.0f)), Mesh::CreateCube(), std::make_shared<BasicMaterial>(Texture::Load("bricks.png")));
}

void Scene::Update(float delta)
{
	x += delta;

	for(std::shared_ptr<Entity> entity : entities)
		entity->Transformation.Rotation = glm::angleAxis(glm::radians(sinf(x) * 180.0f), glm::vec3(0, 1, 0));
}

void Scene::Render(Window& window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewProjection = camera.GetViewProjection();
	for(std::shared_ptr<Entity> entity : entities)
		entity->Render(viewProjection);
}

