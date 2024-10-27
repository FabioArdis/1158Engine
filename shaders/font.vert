#version 460 core

// Input vertex attribute: position and texture coordinates
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;         // Output texture coordinates for fragment shader

uniform mat4 projection;    // Projection matrix for transforming vertex positions

void main()
{
	// Transform the vertex position using the projection matrix
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);

	// Pass texture coordinates to the fragment shader
	TexCoords = vertex.zw;
}