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

	void Render(std::shared_ptr<Shader> altShader,
				const glm::mat4& lightMatrix,
				const glm::mat4& viewProjection, 
				const glm::vec3& ambientLight, 
				const glm::vec3& lightDirection,
				const glm::vec3& cameraPosition,
				bool renderInside = false,
				bool clipping = false,
				const glm::vec4& clippingPlane = glm::vec4())
	{
		if(renderInside)
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);


		if(clipping)
			glEnable(GL_CLIP_DISTANCE0);
		else
			glDisable(GL_CLIP_DISTANCE0);

		glm::mat4 worldMatrix = Transformation.ToMatrix();

		std::shared_ptr<Shader> shader = altShader;
		if(!shader)
		{
			shader = Material->Shader;
			shader->Bind();

			shader->SetUniform("u_world", worldMatrix);
			shader->SetUniform("u_ambientLight", ambientLight);
			shader->SetUniform("u_lightDirection", lightDirection);
			shader->SetUniform("u_cameraPosition", cameraPosition);
			shader->SetUniform("u_clippingPlane", clippingPlane);
			shader->SetUniform("u_lightMatrix", lightMatrix);

			Material->UpdateUniforms();
		}
		else
			shader->Bind();
		
		shader->SetUniform("u_WVP", viewProjection * worldMatrix);

		Mesh->Draw();
	}
};

