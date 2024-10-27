#version 460 core

// Input texture coordinates
in vec2 TexCoords;

// Output color of the fragment
out vec4 color;

// Uniforms for texture and color
uniform sampler2D text; // Texture sampler
uniform vec3 textColor; // Color to tint the text

void main()
{
	// Sample the texture and create a color with alpha based on texture
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);

	// Combine the sampled color with the text color
	color = vec4(textColor, 1.0) * sampled;
}
