#version 330 core

layout (location = 0) in vec3 MvertPos;
layout (location = 1) in vec3 MvertNormal;
layout (location = 2) in vec2 texCoords;

out VS_OUT {
	vec4 WfragNormal;
	vec4 WfragPos;
	vec2 texCoords;
} vs_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform mat4 MVP;

void main() {
    gl_Position = MVP*vec4(MvertPos,1.0f);
    
    vs_out.WfragPos = M*vec4(MvertPos,1.0f);
    vs_out.WfragNormal = M*vec4(MvertNormal,0.0f);

	vs_out.texCoords = texCoords;
}