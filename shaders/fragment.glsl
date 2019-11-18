#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 ObjectColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	vec3 objectColor = ObjectColor;

	// Helpers
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// Ambient
	vec3 ambient = vec3(0.1f, 0.1f, 0.1f) * objectColor;

	// Diffuse
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * objectColor;

	// Specular
	float specStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specStrength * spec * objectColor;

	FragColor = vec4((ambient + diffuse + specular) * lightColor, 1.0f);
}
