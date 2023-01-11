#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_texCoord;
out vec4 v_projectedTexCoords;

uniform mat4 u_WVP;

void main()
{
	v_projectedTexCoords = u_WVP * vec4(a_position, 1.0);
	gl_Position = v_projectedTexCoords;
	v_texCoord = a_texCoord;
}