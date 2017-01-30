#version 330 core

in vec4 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform vec4 someColor;
uniform sampler2D catTexture;


void main() {
	//color = vec4(0.0f,0.5f,0.0f,1.0f);
	//color = vertexColor;
	color = someColor;
	//color = texture(catTexture,texCoord);
}