#version 400 core 

in vec2 pass_texture_coords;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in float visibility;

out vec4 out_Color;

uniform sampler2D texture_sampler;
uniform vec3 lightColor[4];
uniform vec3 attenuation[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

void main(void) {
	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 unitNormal = normalize(surfaceNormal);

	vec3 totalDiffuse = vec3(0.0);
	vec3 totalSpecular = vec3(0.0);

	for (int i=0; i < 4; i++) {
		float d = length(toLightVector[i]);
		float attFactor = attenuation[i].x + (attenuation[i].y * d) + (attenuation[i].z * d * d);

		vec3 unitLight = normalize(toLightVector[i]);
		float nDotl = dot(unitNormal, unitLight);
		// make sure dot products range from 0 to 1
		float brightness = max(nDotl, 0.0);
		vec3 lightDirection = -unitLight;
		vec3 reflectedLightVector = reflect(lightDirection, unitNormal);

		float specularFactor = dot(reflectedLightVector, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.1);
		float dampedFactor = pow(specularFactor, shineDamper);
		totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attFactor;
		totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]) / attFactor;
	}

	totalDiffuse = max(totalDiffuse, 0.2);

	vec4 textureColor = texture(texture_sampler, pass_texture_coords);

	if (textureColor.a < 0.5) {
		discard;
	}

	out_Color = (vec4(totalDiffuse, 1.0) + vec4(totalSpecular, 1.0)) * textureColor;
	out_Color = mix(vec4(skyColor, 1.0), out_Color, visibility);
}