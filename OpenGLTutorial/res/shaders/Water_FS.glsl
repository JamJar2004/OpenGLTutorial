#version 330

in vec2 v_texCoord;
in vec4 v_projectedTexCoords;

uniform sampler2D u_reflection;
uniform sampler2D u_refraction;

out vec4 o_color;

void main()
{
	vec2 texCoords = (v_projectedTexCoords.xy / v_projectedTexCoords.w) * 0.5 + 0.5;

	vec4 reflectionColor = texture(u_reflection, vec2(texCoords.x, 1.0 - texCoords.y));
	vec4 refractionColor = texture(u_refraction, texCoords);

	o_color = mix(reflectionColor, refractionColor, 0.5);
}