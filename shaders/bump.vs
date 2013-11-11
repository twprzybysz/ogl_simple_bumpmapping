#version 330

layout(location=0) in vec3 inPos;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;
layout(location=3) in vec3 inTangent;
layout(location=4) in vec3 inBiTangent;

out vec2 vTexCoord;
out vec3 vPos;

//out vec3 EyeDirection_cameraspace;
//out vec3 LightDirection_cameraspace;
out vec3 eDir_tan;
out vec3 lDir_tan;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

uniform vec4 light;

void main() {
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(inPos,1.0);
	vPos =  vec3(ModelViewMatrix * vec4(inPos,1.0)).xyz;
	vTexCoord = inTexCoord;

	vec3 vertexPosition_cameraspace = (ModelViewMatrix * vec4(inPos,1)).xyz;
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = (ViewMatrix * vec4(light.xyz,1)).xyz;
	vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;	
	
	mat3 m3x3 = mat3(ModelViewMatrix);
		
	mat3 TBN = inverse(mat3(
		m3x3 * inTangent,
		m3x3 * inBiTangent,
		m3x3 * inNormal	
	));

	lDir_tan = TBN * LightDirection_cameraspace;
	eDir_tan =  TBN * EyeDirection_cameraspace;
}