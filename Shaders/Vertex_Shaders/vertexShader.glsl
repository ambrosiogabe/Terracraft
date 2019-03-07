#version 400 core 

in vec3 position;
in vec2 textureCoordinates;
in vec3 normal;

out vec2 pass_texture_coords;
out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition[4];

uniform float useFakeLighting;

const float density = 0.002;
const float gradient = 8.5;

void main(void) {
	vec4 worldPosition = transformationMatrix * vec4(position, 1.0);
	vec4 positionRelativeToCam = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCam;
	pass_texture_coords = textureCoordinates;

	vec3 actualNormal = normal;
	if (useFakeLighting > 0.5) {
		actualNormal = vec3(0.0, 1.0, 0.0);
	}

	// We multiply the transformation matrix so that the normals 
	// are correct relative to the objects transform e.g (rotation, position...)
	surfaceNormal = (transformationMatrix * vec4(actualNormal, 0.0)).xyz;

	for (int i=0; i < 4; i++) {
		toLightVector[i] = lightPosition[i] - worldPosition.xyz;
	}
	toCameraVector = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	float dist = length(positionRelativeToCam.xyz);
	visibility = exp( -pow((dist * density), gradient) );
	visibility = clamp(visibility, 0.0, 1.0);
}