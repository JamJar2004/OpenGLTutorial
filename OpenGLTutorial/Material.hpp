#pragma once

#include "Shader.hpp"
#include "Texture.hpp"

#include <memory>

class Material
{
public:
	Material(std::shared_ptr<Shader> shader) : Shader(shader) {}

	std::shared_ptr<Shader> Shader;

	virtual void UpdateUniforms() = 0;
};

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalMap, const glm::vec3& color = glm::vec3(1), const glm::vec2& tilingFactor = glm::vec2(1)) :
		Material(Shader::Load("Diffuse_VS.glsl", "Diffuse_FS.glsl")),
		Diffuse(texture), NormalMap(normalMap), Color(color), TilingFactor(tilingFactor) {}

	std::shared_ptr<Texture> Diffuse;
	std::shared_ptr<Texture> NormalMap;
	glm::vec3 Color;
	glm::vec2 TilingFactor;

	virtual void UpdateUniforms() override
	{
		Diffuse->Bind(0);
		NormalMap->Bind(1);

		Shader->SetUniform("u_texture", 0);
		Shader->SetUniform("u_normalMap", 1);
		Shader->SetUniform("u_color", Color);
		Shader->SetUniform("u_tilingFactor", TilingFactor);
	}
};

class SkyboxMaterial : public Material
{
public:
	SkyboxMaterial(std::shared_ptr<Texture> cubeMap) :
		Material(Shader::Load("Skybox_VS.glsl", "Skybox_FS.glsl")),
		CubeMap(cubeMap) {}

	std::shared_ptr<Texture> CubeMap;

	virtual void UpdateUniforms() override
	{
		CubeMap->Bind(0);
		Shader->SetUniform("u_cubeMap", 0);
	}
};