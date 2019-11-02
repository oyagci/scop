#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambient_strength = 0.08f;
	vec3 ambient = ambient_strength * lightColor;


	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diffuse_raw = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diffuse_raw * lightColor;


	float specular_strength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float specular_intensity = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specular_intensity * specular_strength * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0f);

}
