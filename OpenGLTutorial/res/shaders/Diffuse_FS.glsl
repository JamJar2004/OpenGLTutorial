#version 330

out vec4 o_color;

in vec2 v_texCoord;
in mat3 v_tbnMatrix;
in vec4 v_shadowMapCoords;

uniform vec3 u_ambientLight;
uniform vec3 u_lightDirection;

uniform sampler2D u_texture;
uniform sampler2D u_normalMap;
uniform sampler2D u_shadowMap;
uniform vec3 u_color;
uniform vec2 u_tilingFactor;
uniform vec2 u_shadowMapSize;

float CalcLight(vec3 lightDirection, vec3 surfaceNormal)
{
	float result = dot(normalize(-lightDirection), surfaceNormal);
	if(result < 0)
		return 0.0;
	
	return result;
}

float SampleShadowMap(sampler2D shadowMap, vec2 coords, float currDepthValue)
{
	float depthValue = texture(shadowMap, coords).r;
	if(depthValue < currDepthValue - 0.01)
		return 0.0;
		
	return 1.0;
}

float SampleShadowMapLinear(sampler2D shadowMap, vec2 coords, float currDepthValue, vec2 pixelSize)
{
	vec2 pixelPosition = coords / pixelSize + 0.5;
	vec2 fractionalPart = fract(pixelPosition);
	vec2 startPixel = (pixelPosition - fractionalPart) * pixelSize;
	
	float bl = SampleShadowMap(shadowMap, startPixel, currDepthValue);
	float br = SampleShadowMap(shadowMap, startPixel + vec2(pixelSize.x, 0), currDepthValue);
	float tl = SampleShadowMap(shadowMap, startPixel + vec2(0, pixelSize.y), currDepthValue);
	float tr = SampleShadowMap(shadowMap, startPixel + pixelSize, currDepthValue);
	
	float bottom = mix(bl, br, fractionalPart.x);
	float top    = mix(tl, tr, fractionalPart.x);
	
	return mix(bottom, top, fractionalPart.y);
}

float SampleShadowMapPCF(sampler2D shadowMap, vec2 coords, float compare, vec2 pixelSize, float samples)
{
	float samplesStart = (samples - 1.0) * 0.5;
	
	float result = 0.0;
	
	for(float y = -samplesStart; y <= samplesStart; y += 1.0)
	{
		for(float x = -samplesStart; x <= samplesStart; x += 1.0)
		{
			vec2 coordsOffset = vec2(x, y) * pixelSize;
			result += SampleShadowMapLinear(shadowMap, coords + coordsOffset, compare, pixelSize);
		}
	}
	
	return result / (samples * samples);
}

void main()
{
	vec4 normalMapColor = texture(u_normalMap, v_texCoord * u_tilingFactor);
	vec3 normal = normalize(v_tbnMatrix * (normalMapColor.xyz * 2 - 1));

	float lightEffect = CalcLight(u_lightDirection, normal);
	vec3 shadowMapCoords = (v_shadowMapCoords.xyz / v_shadowMapCoords.w) * 0.5 + 0.5;
	lightEffect *= SampleShadowMapPCF(u_shadowMap, shadowMapCoords.xy, shadowMapCoords.z, 1.0 / u_shadowMapSize, 5.0);

	o_color = vec4(u_color, 1.0) * texture(u_texture, v_texCoord * u_tilingFactor) * (lightEffect + vec4(u_ambientLight, 1.0));
}