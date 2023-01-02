#pragma once

#include "Window.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Entity.hpp"
#include "KeyboardDevice.hpp"
#include "MouseDevice.hpp"

#include <memory>

class Scene
{
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> monkeys;

	Camera camera;

	glm::vec3 ambientLight;
	glm::vec3 lightDirection;

	float x;

	std::shared_ptr<Entity> CreateEntity(
		const Transformation& transformation,
		std::shared_ptr<Mesh> mesh,
		std::shared_ptr<Material> material)
	{
		std::shared_ptr<Entity> result = 
			std::make_shared<Entity>(transformation, mesh, material);
		entities.push_back(result);
		return result;
	}
public:
	Scene(const Window& window);

	void Update(float delta, KeyboardDevice& keyboard, MouseDevice& mouse);

	void Render(Window& window);
};