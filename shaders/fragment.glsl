#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main()
{
	vec3 objectColor = vec3(1.0f, 0.1f, 0.1f);

	// Helpers
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// Ambient
	vec3 ambient = vec3(0.1f, 0.1f, 0.1f) * objectColor;

	// Diffuse
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * objectColor;

	// Specular

	FragColor = vec4((ambient + diffuse) * lightColor, 1.0f);
}
