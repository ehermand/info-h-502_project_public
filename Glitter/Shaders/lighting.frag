#version 330 core

out vec4 color;

in VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
} fs_in;

uniform vec4 someColor;

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

bool blinnSwitch = true;

void main() {
	color = someColor;
	
	// Lighting
	vec3 Wnormal = normalize(vec3(fs_in.WfragNormal));
	vec3 WlightDir = normalize(WlightPos-vec3(fs_in.WfragPos));
	diff = max(dot(Wnormal,WlightDir),0.0f);
	
	vec3 WeyeDir = normalize(WeyePos-vec3(fs_in.WfragPos));

	if (blinnSwitch) {
		// Blinn-Phong: halfway vector method
		vec3 WhalfWay = normalize(WlightDir+WeyeDir);
		spec = pow(max(dot(Wnormal,WhalfWay),0.0f),gamma);
	} else {
		// Phong: reflection vector method
		vec3 WreflectDir = 2*dot(Wnormal,WlightDir)*Wnormal-WlightDir; //vec3 WreflectDir = reflect(-WlightDir,Wnormal);
		spec = pow(max(dot(WeyeDir,WreflectDir),0.0f),gamma);
	}
	
	// Light attenuation
	float d = distance(vec3(fs_in.WfragPos),WlightPos);
	float att = 1/(kc+kl*d+kq*pow(d,2));
	
	color = emit*color+amb*color+lightColor*att*(diff+spec)*color;
}