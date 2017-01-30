#version 330 core

out vec4 color;

in VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
	vec3 MfragNormal;
} fs_in;

uniform bool lightingSwitch;

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

void main() {
	color = vec4(fs_in.MfragNormal,1.0f);

	if (lightingSwitch) {
		// Lighting
		vec3 Wnormal = normalize(vec3(fs_in.WfragNormal));
		vec3 WlightDir = normalize(WlightPos-vec3(fs_in.WfragPos));
		diff = max(dot(Wnormal,WlightDir),0.0f);
	
		vec3 WeyeDir = normalize(WeyePos-vec3(fs_in.WfragPos));
		vec3 WhalfWay = normalize(WlightDir+WeyeDir);
		spec = pow(max(dot(Wnormal,WhalfWay),0.0f),gamma);

		color = emit*color+amb*color+lightColor*(diff+spec)*color;
	}
}