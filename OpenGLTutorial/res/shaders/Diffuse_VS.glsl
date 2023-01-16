#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_tangent;

out vec2 v_texCoord;
out mat3 v_tbnMatrix;
out vec4 v_shadowMapCoords;

uniform mat4 u_world;
uniform mat4 u_WVP;

uniform mat4 u_lightMatrix;

uniform vec4 u_clippingPlane;

void main()
{
	vec4 worldPosition = u_world * vec4(a_position, 1.0);
	v_shadowMapCoords = u_lightMatrix * worldPosition;
	
	gl_ClipDistance[0] = dot(u_clippingPlane, worldPosition);

	gl_Position = u_WVP * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
	
	vec3 n = normalize((u_world * vec4(a_normal, 0.0)).xyz);
	vec3 t = normalize((u_world * vec4(a_tangent, 0.0)).xyz);
	
	vec3 biTangent = cross(t, n);
	
	v_tbnMatrix = mat3(t, biTangent, n);
}