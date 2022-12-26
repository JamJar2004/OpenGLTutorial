#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Transformation
{
private:
public:
	Transformation(const glm::vec3& position = glm::vec3(0, 0, 0), 
				   const glm::quat& rotation = glm::quat(1, 0, 0, 0),
				   const glm::vec3& scale    = glm::vec3(1, 1, 1)) :
		Position(position), Rotation(rotation), Scale(scale) {}

	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	glm::mat4 ToMatrix() const
	{
		glm::mat4 positionMatrix = glm::translate(glm::identity<glm::mat4>(), Position);
		glm::mat4 rotationMatrix = glm::toMat4(Rotation);
		glm::mat4    scaleMatrix = glm::scale(glm::identity<glm::mat4>(), Scale);

		return positionMatrix * rotationMatrix * scaleMatrix;
	}

	void Rotate(const glm::quat& rotation)
	{
		Rotation = glm::normalize(rotation * Rotation);
	}
};