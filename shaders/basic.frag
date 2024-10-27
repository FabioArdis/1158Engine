#version 460 core

// Structure to hold light properties
struct Light {
	vec3 position;		// Light position in world space
	vec3 color;			// Light color
	float intensity;	// Light intensity
};

#define MAX_LIGHTS 4				// Maximum number of lights allowed
uniform Light lights[MAX_LIGHTS];	// Array of lights
uniform vec3 viewPos;				// Camera position

in vec3 FragPos;	// Fragment position in world space
in vec3 Normal;		// Normal vector at the fragment

out vec4 FragColor;	// Final color output of the fragment

void main()
{
	vec3 ambient = vec3(0.1);	// Basic ambient light contribution

	vec3 result = ambient;		// Initialize result with ambient light

	// Loop through each light and compute its contribution
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		vec3 lightDir = normalize(lights[i].position - FragPos);		// Direction to the light
		float diff = max(dot(Normal, lightDir), 0.0);					// Diffuse intensity based on angle
		vec3 diffuse = lights[i].color * diff * lights[i].intensity;	// Calculate diffuse contribution

		result += diffuse;	// Add diffuse contribution to result
	}

	FragColor = vec4(result, 1.0);			// Set the final fragment color
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Debug: Red color

}