#version 330

out vec4 o_color;

in vec2 v_texCoord;
in vec3 v_normal;

uniform vec3 u_ambientLight;
uniform vec3 u_lightDirection;

uniform sampler2D u_texture;
uniform vec3 u_color;

float CalcLight(vec3 lightDirection, vec3 surfaceNormal)
{
	float result = dot(normalize(-lightDirection), surfaceNormal);
	if(result < 0)
		return 0.0;
	
	return result;
}

void main()
{
	o_color = vec4(u_color, 1.0) * texture(u_texture, v_texCoord * 2) * (CalcLight(u_lightDirection, v_normal) + vec4(u_ambientLight, 1.0));
}