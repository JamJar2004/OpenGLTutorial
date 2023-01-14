#pragma once

#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
private:
	GLuint m_programID;

	std::vector<GLuint> m_shaders;

	void CheckProgramError(GLuint errorType, const std::string& message);
	void CheckShaderError(GLuint shaderID, GLuint errorType,
		const std::string& message);

	void Compile();

	void AddShader(const std::string& text, GLenum type);
public:
	static std::shared_ptr<Shader> Load(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);

	Shader(const std::string& vertexShaderText,
		   const std::string& fragmentShaderText);

	~Shader();

	void Bind();

	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, const glm::vec2& value);
	void SetUniform(const std::string& name, const glm::vec3& value);
	void SetUniform(const std::string& name, const glm::vec4& value);
	void SetUniform(const std::string& name, const glm::mat4& value);
};