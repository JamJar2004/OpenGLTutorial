#version 330

out vec4 o_color;

void main()
{
	o_color = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
}