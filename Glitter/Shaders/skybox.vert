#version 330 core

out vec3 texCoords;

layout (location = 0) in vec3 position;

uniform mat4 V;
uniform mat4 P;

void main() {
    gl_Position = P*V*vec4(position,1.0f);
    texCoords = position;
}