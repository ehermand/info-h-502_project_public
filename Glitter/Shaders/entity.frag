#version 330 core

out vec4 color;

in VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
	vec2 texCoords;
	mat3 TBN;
} fs_in;

uniform bool hasNormalMap;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec4 someColor;

uniform vec3 WeyePos;
uniform vec3 WlightPos[4];
uniform vec4 lightColor;

float emit = 0.0f;
float amb = 0.2f;
float diff = 0.0f;

float kc = 2.0f;
float kl = 0.01f;
float kq = 0.01f;
float att = 1.0f;

float gamma = 32.0f;
float spec = 0.0f;

bool blinnSwitch = true;

void main() {
	color = someColor;
	color = vec4(texture(texture_diffuse1,fs_in.texCoords));
	
	// Lighting
	vec3 Wnormal;
	if (hasNormalMap) {
		Wnormal = texture(texture_normal1, fs_in.texCoords).rgb;
		Wnormal = normalize(Wnormal*2.0 - 1.0);
		Wnormal = normalize(fs_in.TBN*Wnormal);
	} else {
		Wnormal = normalize(vec3(fs_in.WfragNormal));
	}

	vec3 WeyeDir = normalize(WeyePos-vec3(fs_in.WfragPos));
	float lightSum = 0.0f;

	for(int i = 0; i < 4; i++) {
		vec3 WlightDir = normalize(WlightPos[i]-vec3(fs_in.WfragPos));
		diff = max(dot(Wnormal,WlightDir),0.0f);
	
		if (blinnSwitch) {
			// Blinn-Phong: halfway vector method
			vec3 WhalfWay = normalize(WlightDir+WeyeDir);
			spec = pow(max(dot(Wnormal,WhalfWay),0.0f),4*gamma);
		} else {
			// Phong: reflection vector method
			vec3 WreflectDir = 2*dot(Wnormal,WlightDir)*Wnormal-WlightDir; //vec3 WreflectDir = reflect(-WlightDir,Wnormal);
			spec = pow(max(dot(WeyeDir,WreflectDir),0.0f),gamma);
		}
	
		// Light attenuation
		float d = distance(vec3(fs_in.WfragPos),WlightPos[i]);
		float att = 1/(kc+kl*d+kq*pow(d,2));
	
		lightSum += att*(diff+spec);
	}
	
	color = emit*color+amb*color+lightColor*lightSum*color;
}