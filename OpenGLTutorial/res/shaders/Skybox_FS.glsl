#version 330

in vec3 v_texCoord;

uniform samplerCube u_cubeMap;

out vec4 o_color;

void main()
{
	o_color = texture(u_cubeMap, v_texCoord);
}