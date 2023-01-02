#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec3 a_normal;

out vec2 v_texCoord;
out vec3 v_normal;

uniform mat4 u_world;
uniform mat4 u_WVP;

void main()
{
	gl_Position = u_WVP * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
	v_normal   = normalize((u_world * vec4(a_normal, 0.0)).xyz);
}