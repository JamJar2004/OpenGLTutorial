#include "Scene.hpp"

float randf()
{
	return rand() / float(RAND_MAX);
}

Scene::Scene(const Window& window) :
	camera(Transformation(glm::vec3(0, 2, 0)), glm::perspective(glm::radians(70.0f), window.Width / float(window.Height), 0.1f, 1000.0f)),
	x(0),
	lightDirection(glm::vec3(1, -1, -1)),
	ambientLight(glm::vec3(0.2f))
{
	std::shared_ptr<Material> material1 = std::make_shared<DiffuseMaterial>(Texture::Load("bricks.png"), Texture::Load("bricks normal.png"), glm::vec3(1), glm::vec2(20));
	std::shared_ptr<Material> material2 = std::make_shared<DiffuseMaterial>(Texture::Load("blank.png"), Texture::Load("blank normal.png"), glm::vec3(0.5f, 0.0f, 1.0f));

	float terrainSize = 20;
	CreateEntity(Transformation(glm::vec3(), glm::quat(1, 0, 0, 0), glm::vec3(terrainSize)), Mesh::LoadTerrain("heightMap.png"), material1);

	std::shared_ptr<Mesh> monkeyMesh = Mesh::Load("monkey.obj");
	constexpr size_t MONKEY_COUNT = 10;
	for(size_t i = 0; i < MONKEY_COUNT; i++)
	{
		float x = (i / float(MONKEY_COUNT)) * 2.0f - 1.0f;
		monkeys.push_back(CreateEntity(Transformation(glm::vec3(x * terrainSize, 5, sinf(i) * 5)), monkeyMesh, material2));
	}
}

const float MOUSE_SENSITIVITY = 0.5;

void Scene::Update(float delta, KeyboardDevice& keyboard, MouseDevice& mouse)
{
	for(size_t i = 0; i < monkeys.size(); i++)
		monkeys[i]->Transformation.Rotate(glm::angleAxis(delta, glm::vec3(0, 1, 0)));

	x += delta;

	if(mouse.IsButtonDown(LEFT))
	{
		glm::vec2 deltaPos = mouse.GetPosition();

        bool rotY = deltaPos.x != 0;
        bool rotX = deltaPos.y != 0;

        if(rotY)
			camera.Transformation.Rotate(glm::angleAxis(-deltaPos.x * MOUSE_SENSITIVITY * 180.0f * delta, glm::vec3(0, 1, 0)));

        if(rotX)
			camera.Transformation.Rotate(glm::angleAxis(deltaPos.y * MOUSE_SENSITIVITY * 180.0f * delta, glm::rotate(camera.Transformation.Rotation, glm::vec3(1, 0, 0))));

        if(rotY || rotX)
			mouse.SetPosition(glm::vec2(0, 0));
	}

	if(keyboard.IsKeyDown(SDL_SCANCODE_W))
		camera.Transformation.Position += 10.0f * glm::rotate(camera.Transformation.Rotation, glm::vec3(0, 0, -1)) * delta;

	if(keyboard.IsKeyDown(SDL_SCANCODE_S))
		camera.Transformation.Position += 10.0f * glm::rotate(camera.Transformation.Rotation, glm::vec3(0, 0, 1)) * delta;
}

void Scene::Render(Window& window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewProjection = camera.GetViewProjection();
	for(std::shared_ptr<Entity> entity : entities)
		entity->Render(viewProjection, ambientLight, lightDirection);
}

