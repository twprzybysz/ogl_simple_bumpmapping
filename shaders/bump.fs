#version 330
in vec3 vPos;
in vec2 vTexCoord;

//in vec3 EyeDirection_cameraspace;
//in vec3 LightDirection_cameraspace;

in vec3 eDir_tan;
in vec3 lDir_tan;

out vec4 vColor;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

uniform vec4 light;

uniform sampler2D diffuseSampler;
uniform sampler2D normalSampler; 

void main() {

	// Material properties
	vec3 MaterialDiffuseColor = texture2D( diffuseSampler, vTexCoord ).rgb;
	vec3 MaterialSpecularColor = vec3(1.0);

	vec3 TextureNormal_tangentspace = normalize(texture2D( normalSampler, vTexCoord ).rgb*2.0 - 1.0);
	
	// Normal of the computed fragment, in camera space
	vec3 n = TextureNormal_tangentspace;
	vec3 l = normalize(lDir_tan);

    float nl = dot( n,l );
	vec3 diffuseColor = clamp( nl, 0, 1) * MaterialDiffuseColor ;

	// Eye vector (towards the camera)
    vec3 specularColor = vec3(0.0);

    if (nl > 0.0f) {
        
        vec3 E = normalize(eDir_tan);
        vec3 R = reflect(-l,n);
        float specular =  pow(clamp( dot( E,R ), 0, 1),25);
        specularColor =  MaterialSpecularColor * specular;
	}

    vColor = vec4( diffuseColor + specularColor, 1);
}