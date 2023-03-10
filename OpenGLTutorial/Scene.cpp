#include "Scene.hpp"

float randf()
{
	return rand() / float(RAND_MAX);
}

Scene::Scene(const Window& window) :
	mainCamera(Transformation(glm::vec3(0, 2, 0)), glm::perspective(glm::radians(70.0f), window.Width / float(window.Height), 0.1f, 1000.0f)),
	altCamera(Transformation(), glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f)),
	lightDirection(glm::vec3(1, -1, -1)),
	ambientLight(glm::vec3(0.2f))
{
	reflectionFrame = std::make_shared<FrameBuffer>(1024, 1024, true, false, GL_LINEAR, GL_CLAMP_TO_EDGE);
	refractionFrame = std::make_shared<FrameBuffer>(1024, 1024, true, true, GL_LINEAR, GL_CLAMP_TO_EDGE);

	shadowFrame = std::make_shared<FrameBuffer>(1024, 1024, false, true, GL_NEAREST, GL_CLAMP_TO_EDGE);

	shadowMapShader = Shader::Load("ShadowMapShader_VS.glsl", "ShadowMapShader_FS.glsl");

	std::shared_ptr<Material> material1 = std::make_shared<DiffuseMaterial>(Texture::Load("bricks.png"), Texture::Load("bricks normal.png"), shadowFrame->GetDepthAttachment(), glm::vec3(1), glm::vec2(20));
	std::shared_ptr<Material> material2 = std::make_shared<DiffuseMaterial>(Texture::Load("blank.png"), Texture::Load("blank normal.png"), shadowFrame->GetDepthAttachment(), glm::vec3(0.5f, 0.0f, 1.0f));

	float terrainSize = 20;
	CreateEntity(Transformation(glm::vec3(), glm::quat(1, 0, 0, 0), glm::vec3(terrainSize)), Mesh::LoadTerrain("heightMap.png"), material1);

	std::shared_ptr<Mesh> monkeyMesh = Mesh::Load("monkey.obj");
	constexpr size_t MONKEY_COUNT = 10;
	for(size_t i = 0; i < MONKEY_COUNT; i++)
	{
		float x = (i / float(MONKEY_COUNT)) * 2.0f - 1.0f;
		monkeys.push_back(CreateEntity(Transformation(glm::vec3(x * terrainSize, 5, sinf(i) * 5)), monkeyMesh, material2));
	}

	std::shared_ptr<Texture> cubeMap = Texture::LoadCubeMap("left.png", "right.png", "bottom.png", "top.png", "back.png", "front.png");
	std::shared_ptr<Mesh> cube = Mesh::CreateCube();
	std::shared_ptr<Material> skyMaterial = std::make_shared<SkyboxMaterial>(cubeMap);
	
	skybox = std::make_shared<Entity>(Transformation(glm::vec3(), glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)), glm::vec3(100)), cube, skyMaterial);
	
	std::shared_ptr<Mesh> quad = Mesh::CreateQuad();

	std::shared_ptr<WaterMaterial> waterMaterial = std::make_shared<WaterMaterial>(reflectionFrame->GetColorAttachment(), refractionFrame->GetColorAttachment(), refractionFrame->GetDepthAttachment(), Texture::Load("dudvMap.png"), Texture::Load("normalMap.png"), glm::vec2(4));
	waterOffset = &waterMaterial->Offset;
	water = std::make_shared<Entity>(Transformation(glm::vec3(0, -4, 0), glm::quat(1, 0, 0, 0), glm::vec3(terrainSize)), quad, waterMaterial);
}

const float MOUSE_SENSITIVITY = 0.5;

void Scene::Update(float delta, KeyboardDevice& keyboard, MouseDevice& mouse)
{
	for(size_t i = 0; i < monkeys.size(); i++)
		monkeys[i]->Transformation.Rotate(glm::angleAxis(delta, glm::vec3(0, 1, 0)));

	*waterOffset += delta * 0.04f;
	*waterOffset = fmodf(*waterOffset, 1.0f);

	if(mouse.IsButtonDown(LEFT))
	{
		glm::vec2 deltaPos = mouse.GetPosition();

        bool rotY = deltaPos.x != 0;
        bool rotX = deltaPos.y != 0;

        if(rotY)
			mainCamera.Transformation.Rotate(glm::angleAxis(-deltaPos.x * MOUSE_SENSITIVITY * 180.0f * delta, glm::vec3(0, 1, 0)));

        if(rotX)
			mainCamera.Transformation.Rotate(glm::angleAxis(deltaPos.y * MOUSE_SENSITIVITY * 180.0f * delta, glm::rotate(mainCamera.Transformation.Rotation, glm::vec3(1, 0, 0))));

        if(rotY || rotX)
			mouse.SetPosition(glm::vec2(0, 0));
	}

	if(keyboard.IsKeyDown(SDL_SCANCODE_W))
		mainCamera.Transformation.Position += 10.0f * glm::rotate(mainCamera.Transformation.Rotation, glm::vec3(0, 0, -1)) * delta;

	if(keyboard.IsKeyDown(SDL_SCANCODE_S))
		mainCamera.Transformation.Position += 10.0f * glm::rotate(mainCamera.Transformation.Rotation, glm::vec3(0, 0, 1)) * delta;

	skybox->Transformation.Position = mainCamera.Transformation.Position;
}

void Scene::RenderEntities(Camera& camera, const glm::mat4& lightMatrix, std::shared_ptr<Shader> altShader, bool clipping, const glm::vec4& clippingPlane)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 viewProjection = camera.GetViewProjection();
	for(std::shared_ptr<Entity> entity : entities)
		entity->Render(altShader, lightMatrix, viewProjection, ambientLight, lightDirection, camera.Transformation.Position, false, clipping, clippingPlane);

	skybox->Render(altShader, lightMatrix, viewProjection, ambientLight, lightDirection, camera.Transformation.Position, true);
}

void Scene::Render(Window& window)
{
	altCamera.Transformation.Rotation = glm::quatLookAt(lightDirection, glm::vec3(0, 1, 0));
	glm::mat4 lightMatrix = altCamera.GetViewProjection();
	shadowFrame->Bind();
	RenderEntities(altCamera, lightMatrix, shadowMapShader, false);

	float waterHeight = water->Transformation.Position.y;

	glm::vec3& cameraPosition = mainCamera.Transformation.Position;
	glm::quat& cameraRotation = mainCamera.Transformation.Rotation;

	float distance = cameraPosition.y - waterHeight;

	cameraPosition.y -= distance * 2;
	cameraRotation.x = -cameraRotation.x;
	cameraRotation.z = -cameraRotation.z;

	reflectionFrame->Bind();
	RenderEntities(mainCamera, lightMatrix, nullptr, true, glm::vec4(0, 1, 0, -waterHeight));

	cameraPosition.y += distance * 2;
	cameraRotation.x = -cameraRotation.x;
	cameraRotation.z = -cameraRotation.z;

	refractionFrame->Bind();
	RenderEntities(mainCamera, lightMatrix, nullptr, true, glm::vec4(0, -1, 0, waterHeight + 0.1f));

	window.BindFrameBuffer();
	RenderEntities(mainCamera, lightMatrix, nullptr, false);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	water->Render(nullptr, lightMatrix, mainCamera.GetViewProjection(), ambientLight, lightDirection, mainCamera.Transformation.Position);
	glDisable(GL_BLEND);
}

