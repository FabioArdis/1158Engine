#version 460 core

// Vertex attributes
layout (location = 0) in vec3 aPos;		// Vertex position
layout (location = 1) in vec3 aNormal;	// Vertex normal

// Outputs to fragment shader
out vec3 FragPos;	// Position of the fragment in world space
out vec3 Normal;	// Normal vector of the fragment

// Transformation matrices
uniform mat4 model;			// Model matrix
uniform mat4 view;			// View matrix
uniform mat4 projection;	// Projection matrix

void main()
{
	// Calculate the fragment position in world space
	FragPos = vec3(model * vec4(aPos, 1.0));

    // Calculate the transformed normal (accounting for non-uniform scaling)
	Normal = mat3(transpose(inverse(model))) * aNormal;

    // Compute the final position of the vertex in clip space
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}