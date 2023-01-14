#version 330

in vec3 v_worldPosition;
in vec2 v_texCoord;
in vec4 v_projectedTexCoords;

uniform vec3 u_cameraPosition;
uniform vec3 u_lightDirection;

uniform sampler2D u_reflection;
uniform sampler2D u_refraction;
uniform sampler2D u_depthMap;

uniform sampler2D u_dudvMap;
uniform sampler2D u_normalMap;

uniform vec2 u_tilingFactor;

uniform float u_waveStrength;
uniform float u_offset;

uniform float u_specularIntensity;
uniform float u_specularPower;

out vec4 o_color;

float CalcSpecularLight(vec3 lightDirection, vec3 surfaceNormal, vec3 toCameraVector, float specularIntensity, float specularPower)
{
	vec3 reflectedDirection = normalize(reflect(lightDirection, surfaceNormal));
	float result = dot(toCameraVector, reflectedDirection);
	result = pow(result, specularPower);
	result *= specularIntensity;

	if(result > 0.0)
		return result;
		
	return 0.0;
}

void main()
{
	vec2 distortion1 = (texture(u_dudvMap, v_texCoord * u_tilingFactor + u_offset).rg * 2.0 - 1.0) * u_waveStrength;
	vec2 distortion2 = (texture(u_dudvMap, v_texCoord * u_tilingFactor + vec2(u_offset, -u_offset * 0.5)).rg * 2.0 - 1.0) * u_waveStrength;
	
	vec2 distortion = distortion1 + distortion2;

	vec2 texCoords = (v_projectedTexCoords.xy / v_projectedTexCoords.w) * 0.5 + 0.5;
	
	float near = 0.1;
	float far = 1000;
	
	float depth = texture(u_depthMap, texCoords).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	
	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = floorDistance - waterDistance;
	
	vec4 reflectionColor = texture(u_reflection, vec2(texCoords.x, 1.0 - texCoords.y) + distortion);
	vec4 refractionColor = texture(u_refraction, texCoords + distortion);
	
	vec4 normalMapColor = texture(u_normalMap, v_texCoord * u_tilingFactor + u_offset + distortion);
	
	vec3 normal = normalize(vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0));
	
	vec3 toCameraVector = normalize(u_cameraPosition - v_worldPosition);
	
	float refractiveFactor = dot(toCameraVector, normal);
	
	o_color = mix(reflectionColor, refractionColor, refractiveFactor) + CalcSpecularLight(u_lightDirection, normal, toCameraVector, u_specularIntensity, u_specularPower);
	o_color.a = clamp(waterDepth / 5.0, 0.0, 1.0);
}