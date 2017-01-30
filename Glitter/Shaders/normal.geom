#version 330 core

out vec3 normal;

layout(triangles) in;
layout(line_strip,max_vertices = 8) out;

in VS_OUT {
	vec3 MfragNormal;
	vec3 normal;
} gs_in[];

const float MAG = 0.1f;

void GenerateLine(int index) {
	normal = gs_in[index].MfragNormal;
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal,0.0f)*MAG;
	EmitVertex();
	EndPrimitive();
}

void main() {
	GenerateLine(0); // First vertex normal
	//GenerateLine(1); // Second vertex normal
	//GenerateLine(2); // Third vertex normal
}