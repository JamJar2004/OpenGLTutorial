#pragma once

#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>

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
};