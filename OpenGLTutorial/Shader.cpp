#include "Shader.hpp"

#include <iostream>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

void Shader::CheckProgramError(GLenum errorType, const std::string& message)
{
	GLint status = 0;
	GLchar error[1024] = { 0 };

	glGetProgramiv(m_programID, errorType, &status);

	if(status == GL_FALSE)
	{
		glGetProgramInfoLog(m_programID, sizeof(error), nullptr, error);
		std::cerr << message << ": '" << error << "'" << std::endl;
	}
}

void Shader::CheckShaderError(GLuint shader, GLenum errorType, const std::string& message)
{
	GLint status = 0;
	GLchar error[1024] = { 0 };

	glGetShaderiv(shader, errorType, &status);

	if(status == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
		std::cerr << message << ": '" << error << "'" << std::endl;
	}
}

void Shader::Compile()
{
	glLinkProgram(m_programID);
	CheckProgramError(GL_LINK_STATUS, "Failed to link program");

	glValidateProgram(m_programID);
	CheckProgramError(GL_VALIDATE_STATUS, "Invalid program");
}

void Shader::AddShader(const std::string& text, GLenum type)
{
	GLuint shaderID = glCreateShader(type);

	if(shaderID == 0)
		std::cerr << "Failed to allocate memory for shader" << std::endl;

	const GLchar* textChars[] { text.c_str() };
	GLint textLengths[] { text.length() };
	glShaderSource(shaderID, 1, textChars, textLengths);
	glCompileShader(shaderID);

	CheckShaderError(shaderID, GL_COMPILE_STATUS, 
		"Failed to compile shader!");

	glAttachShader(m_programID, shaderID);
	m_shaders.push_back(shaderID);
}

std::shared_ptr<Shader> Shader::Load(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName)
{
	std::ifstream vsStream;
	vsStream.open("./res/shaders/" + vertexShaderFileName);

	std::ifstream fsStream;
	fsStream.open("./res/shaders/" + fragmentShaderFileName);

	std::string line;

	std::string vertexShaderText;
	while(std::getline(vsStream, line)) { vertexShaderText += line + "\n"; }

	std::string fragmentShaderText;
	while(std::getline(fsStream, line)) { fragmentShaderText += line + "\n"; }
		
	vsStream.close();
	fsStream.close();

	return std::make_shared<Shader>(vertexShaderText, fragmentShaderText);
}

Shader::Shader(const std::string& vertexShaderText,
	           const std::string& fragmentShaderText)
{
	m_programID = glCreateProgram();

	AddShader(vertexShaderText, GL_VERTEX_SHADER);
	AddShader(fragmentShaderText, GL_FRAGMENT_SHADER);

	Compile();
}

Shader::~Shader()
{
	for(GLuint shaderID : m_shaders)
		glDeleteShader(shaderID);

	glDeleteProgram(m_programID);
}

void Shader::Bind()
{
	glUseProgram(m_programID);
}

void Shader::SetUniform(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	glUniform1i(location, value);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}


void Shader::SetUniform(const std::string& name, const glm::vec2& value)
{
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value)
{
	GLint location = glGetUniformLocation(m_programID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
