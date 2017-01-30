#version 330 core

layout (location = 0) in vec3 MvertPos;
layout (location = 1) in vec3 MvertNormal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 MvertTangent;
layout (location = 4) in vec3 MvertBitangent;

out VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
	vec2 texCoords;
	mat3 TBN;
} vs_out;

uniform bool hasNormalMap;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;

void main() {
    gl_Position = MVP*vec4(MvertPos,1.0f);
    
    vs_out.WfragPos = M*vec4(MvertPos,1.0f);
    vs_out.WfragNormal = M*vec4(MvertNormal,0.0f);

	if (hasNormalMap) {
		vec3 T = normalize(vec3(M*vec4(MvertTangent,0.0)));
		vec3 B = normalize(vec3(M*vec4(MvertBitangent,0.0)));
		vec3 N = normalize(vec3(M*vec4(MvertNormal,0.0)));
		// Fix for symetric UVs (?)
		if (dot(cross(N,T),B) < 0.0)
			T = T * -1.0;
		vs_out.TBN = mat3(T,B,N);
	}

	vs_out.texCoords = texCoords;
}