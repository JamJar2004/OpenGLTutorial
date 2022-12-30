#version 330

out vec4 o_color;

in vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
	o_color = texture(u_texture, v_texCoord * 2);
}