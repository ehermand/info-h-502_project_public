#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 itexCoord;

out vec4 vertexColor;
out vec2 texCoord;

uniform mat4 MVP;

void main() {
    //gl_Position = vec4(position.x,position.y,position.z,1.0f);
    gl_Position = MVP*vec4(position,1.0f);
    
    vertexColor = vec4(color, 1.0f);
    texCoord = vec2(itexCoord.x,1-itexCoord.y);
}