#pragma once

#include "Transformation.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Material.hpp"

#include <memory>

class Entity
{
private:
public:
	Transformation            Transformation;
	std::shared_ptr<Mesh>     Mesh;
	std::shared_ptr<Material> Material;

	void Render(const glm::mat4& viewProjection, const glm::vec3& ambientLight, const glm::vec3& lightDirection, bool renderInside = false)
	{
		if(renderInside)
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);

		std::shared_ptr<Shader> shader = Material->Shader;
		shader->Bind();

		glm::mat4 worldMatrix = Transformation.ToMatrix();
		shader->SetUniform("u_world", worldMatrix);
		shader->SetUniform("u_WVP", viewProjection * worldMatrix);
		shader->SetUniform("u_ambientLight", ambientLight);
		shader->SetUniform("u_lightDirection", lightDirection);
		Material->UpdateUniforms();
		Mesh->Draw();
	}
};

