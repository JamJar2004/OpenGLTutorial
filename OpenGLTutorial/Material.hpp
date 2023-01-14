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

class WaterMaterial : public Material
{
public:
	WaterMaterial(std::shared_ptr<Texture> reflection,
				  std::shared_ptr<Texture> refraction,
				  std::shared_ptr<Texture> depthMap,
				  std::shared_ptr<Texture> dudvMap,
				  std::shared_ptr<Texture> normalMap,
				  const glm::vec2& tilingFactor = glm::vec2(1),
				  float waveStrength = 0.04f,
				  float specularIntensity = 1.0f,
				  float specularPower = 8.0f) :
		Material(Shader::Load("Water_VS.glsl", "Water_FS.glsl")),
		Reflection(reflection),
		Refraction(refraction),
		DepthMap(depthMap),
		DUDVMap(dudvMap),
		NormalMap(normalMap),
		TilingFactor(tilingFactor),
		WaveStrength(waveStrength),
		SpecularIntensity(specularIntensity),
		SpecularPower(specularPower),
		Offset(0) {}

	std::shared_ptr<Texture> Reflection;
	std::shared_ptr<Texture> Refraction;
	std::shared_ptr<Texture> DepthMap;

	std::shared_ptr<Texture> DUDVMap;
	std::shared_ptr<Texture> NormalMap;

	glm::vec2 TilingFactor;

	float WaveStrength;
	float Offset;

	float SpecularIntensity;
	float SpecularPower;

	virtual void UpdateUniforms() override
	{
		Reflection->Bind(0);
		Refraction->Bind(1);
		DepthMap->Bind(4);

		DUDVMap->Bind(2);
		NormalMap->Bind(3);

		Shader->SetUniform("u_reflection", 0);
		Shader->SetUniform("u_refraction", 1);
		Shader->SetUniform("u_depthMap", 4);

		Shader->SetUniform("u_dudvMap", 2);
		Shader->SetUniform("u_normalMap", 3);

		Shader->SetUniform("u_tilingFactor", TilingFactor);

		Shader->SetUniform("u_waveStrength", WaveStrength);
		Shader->SetUniform("u_offset", Offset);

		Shader->SetUniform("u_specularIntensity", SpecularIntensity);
		Shader->SetUniform("u_specularPower", SpecularPower);
	}
};