#version 330 core

out vec4 color;

in VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
} fs_in;

uniform samplerCube skybox;

uniform vec3 WeyePos;
uniform vec3 WlightPos;
uniform vec4 lightColor;

float emit = 0.0f;
float amb = 0.5f;
float diff = 0.0f;

float kc = 1.0f;
float kl = 0.01f;
float kq = 0.01f;
float att = 1.0f;

float gamma = 16.0f;
float spec = 0.0f;

// Refraction indices
float etaL = 1.0f;
float etaT = 1.52f;
float refractRatio = etaL/etaT;

bool blinnSwitch = true;

void main() {
	// Refraction
	vec3 Wnormal = normalize(vec3(fs_in.WfragNormal));
	vec3 WeyeDir = normalize(WeyePos-vec3(fs_in.WfragPos));
	float cosTheta = max(dot(Wnormal,WeyeDir),0.0);
	float k = refractRatio*cosTheta-sqrt(1.0f-pow(refractRatio,2)*(1.0f-pow(cosTheta,2)));
	vec3 WrefractDir = k*Wnormal-refractRatio*WeyeDir;
	color = texture(skybox,WrefractDir);
}