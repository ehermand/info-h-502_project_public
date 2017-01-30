#version 330 core

layout (location = 0) in vec3 MvertPos;
layout (location = 1) in vec3 MvertNormal;

out VS_OUT {
	vec3 MfragNormal;
	vec3 normal;
} vs_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;

void main() {
    gl_Position = MVP*vec4(MvertPos,1.0f);

	vs_out.MfragNormal = MvertNormal;

	mat3 normalMatrix = mat3(transpose(inverse(V*M)));
	vs_out.normal = vec3(P*vec4(normalMatrix*MvertNormal,1.0f));
}