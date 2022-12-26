#version 330

out vec4 o_color;

in vec3 v_position;

void main()
{
	o_color = vec4(v_position, 1.0);
}