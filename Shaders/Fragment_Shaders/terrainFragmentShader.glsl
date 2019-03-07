#version 400 core 

in vec2 pass_texture_coords;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D bTexture;
uniform sampler2D gTexture;
uniform sampler2D blendMap;

uniform vec3 lightColor[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

void main(void) {
	vec4 blendMapColor = texture(blendMap, pass_texture_coords);

	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = pass_texture_coords * 256.0;
	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;

	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;

	vec3 unitNormal = normalize(surfaceNormal);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);
	for (int i=0; i < 4; i++) {
		float d = length(toLightVector[i]);
		float attFactor = attenuation[i].x + (attenuation[i].y * d) + (attenuation[i].z * d * d);

		vec3 unitLight = normalize(toLightVector[i]);

		float nDotl = dot(unitNormal, unitLight);
		// make sure dot products range from 0 to 1
		float brightness = max(nDotl, 0.1);
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attFactor;

		vec3 unitVectorToCamera = normalize(toCameraVector);
		vec3 lightDirection = -unitLight;
		vec3 reflectedLightVector = reflect(lightDirection, unitNormal);

		float specularFactor = dot(reflectedLightVector, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.1);
		float dampedFactor = pow(specularFactor, shineDamper);
		totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]) / attFactor;
	}

	totalDiffuse = max(totalDiffuse, 0.2);

	out_Color = (vec4(totalDiffuse, 1.0) + vec4(totalSpecular, 1.0)) * totalColor;
	out_Color = mix(vec4(skyColor, 1.0), out_Color, visibility);
}