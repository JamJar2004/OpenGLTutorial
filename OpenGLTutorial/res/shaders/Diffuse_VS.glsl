#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_tangent;

out vec2 v_texCoord;
out mat3 v_tbnMatrix;

uniform mat4 u_world;
uniform mat4 u_WVP;

void main()
{
	gl_Position = u_WVP * vec4(a_position, 1.0);
	v_texCoord = a_texCoord;
	
	vec3 n = normalize((u_world * vec4(a_normal, 0.0)).xyz);
	vec3 t = normalize((u_world * vec4(a_tangent, 0.0)).xyz);
	
	vec3 biTangent = cross(t, n);
	
	v_tbnMatrix = mat3(t, biTangent, n);
}