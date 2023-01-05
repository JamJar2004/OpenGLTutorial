#version 330

out vec4 o_color;

in vec2 v_texCoord;
in mat3 v_tbnMatrix;

uniform vec3 u_ambientLight;
uniform vec3 u_lightDirection;

uniform sampler2D u_texture;
uniform sampler2D u_normalMap;
uniform vec3 u_color;
uniform vec2 u_tilingFactor;

float CalcLight(vec3 lightDirection, vec3 surfaceNormal)
{
	float result = dot(normalize(-lightDirection), surfaceNormal);
	if(result < 0)
		return 0.0;
	
	return result;
}

void main()
{
	vec4 normalMapColor = texture(u_normalMap, v_texCoord * u_tilingFactor);
	vec3 normal = normalize(v_tbnMatrix * (normalMapColor.xyz * 2 - 1));

	o_color = vec4(u_color, 1.0) * texture(u_texture, v_texCoord * u_tilingFactor) * (CalcLight(u_lightDirection, normal) + vec4(u_ambientLight, 1.0));
}