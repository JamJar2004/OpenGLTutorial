#pragma once

#include "Transformation.hpp"

class Camera
{
private:
public:
	Camera(const Transformation& transformation, 
		   const glm::mat4&      projection) :
		Transformation(transformation), Projection(projection) {}

	Transformation Transformation;
	glm::mat4      Projection;

	glm::mat4 GetViewMatrix() const
	{
		glm::mat4 positionMatrix = glm::translate(glm::identity<glm::mat4>(), -Transformation.Position);
		glm::mat4 rotationMatrix = glm::toMat4(glm::conjugate(Transformation.Rotation));
		glm::mat4    scaleMatrix = glm::scale(glm::identity<glm::mat4>(), 1.0f / Transformation.Scale);

		return scaleMatrix * rotationMatrix * positionMatrix;
	}

	glm::mat4 GetViewProjection() const
	{
		return Projection * GetViewMatrix();
	}
};