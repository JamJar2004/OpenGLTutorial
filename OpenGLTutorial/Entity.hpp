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

	void Render(const glm::mat4& viewProjection, bool renderInside = false)
	{
		if(renderInside)
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);

		std::shared_ptr<Shader> shader = Material->Shader;
		shader->Bind();
		shader->SetUniform("u_WVP", viewProjection * Transformation.ToMatrix());
		Material->UpdateUniforms();
		Mesh->Draw();
	}
};

