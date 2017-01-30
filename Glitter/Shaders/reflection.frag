#version 330 core

out vec4 color;

in VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
	vec2 texCoords;
} fs_in;

uniform samplerCube skybox;

uniform sampler2D texture_diffuse1;

uniform vec3 WeyePos;
uniform vec3 WlightPos[4];
uniform vec4 lightColor;

float emit = 0.0f;
float amb = 0.2f;
float diff = 0.0f;

float kc = 1.0f;
float kl = 0.01f;
float kq = 0.01f;
float att = 1.0f;

float gamma = 32.0f;
float spec = 0.0f;

void main() {
	vec4 objectColor = texture(texture_diffuse1,fs_in.texCoords);
	
	vec3 WreflectDir;

	vec3 Wnormal = normalize(vec3(fs_in.WfragNormal));
	vec3 WeyeDir = normalize(WeyePos-vec3(fs_in.WfragPos));

	// Lighting
	float lightSum = 0.0f;

	for(int i = 0; i < 4; i++) {
		vec3 WlightDir = normalize(WlightPos[i]-vec3(fs_in.WfragPos));
		diff = max(dot(Wnormal,WlightDir),0.0f);

		// Blinn-Phong: halfway vector method
		vec3 WhalfWay = normalize(WlightDir+WeyeDir);
		spec = pow(max(dot(Wnormal,WhalfWay),0.0f),4*gamma);
	
		// Light attenuation
		float d = distance(vec3(fs_in.WfragPos),WlightPos[i]);
		float att = 1/(kc+kl*d+kq*pow(d,2));
	
		lightSum += att*(diff+spec);
	}

	// Reflection
	WreflectDir = 2*max(dot(Wnormal,WeyeDir),0.0)*Wnormal-WeyeDir;
	vec4 reflectionColor = texture(skybox,WreflectDir)*0.2;
	
	color = objectColor+reflectionColor;

	color = emit*color+amb*color+lightColor*lightSum*color;
}