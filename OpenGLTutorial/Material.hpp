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
	DiffuseMaterial(std::shared_ptr<Texture> texture, const glm::vec3& color = glm::vec3(1)) :
		Material(Shader::Load("Diffuse_VS.glsl", "Diffuse_FS.glsl")),
		Texture(texture), Color(color) {}

	std::shared_ptr<Texture> Texture;
	glm::vec3 Color;

	virtual void UpdateUniforms() override
	{
		Texture->Bind(0);
		Shader->SetUniform("u_texture", 0);
		Shader->SetUniform("u_color", Color);
	}
};