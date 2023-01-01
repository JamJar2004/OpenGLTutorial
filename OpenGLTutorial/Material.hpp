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

class BasicMaterial : public Material
{
public:
	BasicMaterial(std::shared_ptr<Texture> texture) :
		Material(Shader::Load("Basic_VS.glsl", "Basic_FS.glsl")),
		Texture(texture) {}

	std::shared_ptr<Texture> Texture;

	virtual void UpdateUniforms() override
	{
		Texture->Bind(0);
		Shader->SetUniform("u_texture", 0);
	}
};