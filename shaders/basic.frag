#version 460 core

// Structure to hold light properties
struct Light {
	vec3 position;		// Light position in world space
	vec3 color;			// Light color
	float intensity;	// Light intensity
};

#define MAX_LIGHTS 16				// Maximum number of lights allowed
uniform Light lights[MAX_LIGHTS];	// Array of lights
uniform vec3 viewPos;				// Camera position

uniform int numLights; // Actual number of lights

in vec3 FragPos;	// Fragment position in world space
in vec3 Normal;		// Normal vector at the fragment

out vec4 FragColor;	// Final color output of the fragment

void main()
{
    vec3 ambient = vec3(0.1);
    vec3 result = ambient;

    int lightsToProcess = min(numLights, MAX_LIGHTS);
    for (int i = 0; i < lightsToProcess; i++)
    {
        // Calculate distance to light
        vec3 lightDir = lights[i].position - FragPos;
        float distance = length(lightDir);
        lightDir = normalize(lightDir);
        
        // Calculate attenuation
        float constant = 1.0;
        float linear = 0.09;
        float quadratic = 0.032;
        float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
        
        // Calculate diffuse lighting
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = lights[i].color * diff * lights[i].intensity * attenuation;
        
        result += diffuse;
    }
    
    FragColor = vec4(result, 1.0);
}