#pragma once

#include "Window.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Entity.hpp"
#include "KeyboardDevice.hpp"
#include "MouseDevice.hpp"
#include "FrameBuffer.hpp"

#include <memory>

class Scene
{
private:
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Entity>> monkeys;

	std::shared_ptr<Entity> skybox;
	std::shared_ptr<Entity> water;

	std::shared_ptr<FrameBuffer> reflectionFrame;
	std::shared_ptr<FrameBuffer> refractionFrame;

	Camera mainCamera;

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

	void RenderEntities(Camera& camera, bool clipping, const glm::vec4& clippingPlane = glm::vec4());

	void Render(Window& window);
};