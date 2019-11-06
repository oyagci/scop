#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
//	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));


	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;


	// specular
	vec3 reflect_dir = normalize(reflect(-lightDir, Normal));
	vec3 view_dir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;


	// emission
	vec3 emission = texture(material.emission, TexCoords).rgb;

	//vec3 result = ambient + diffuse + specular + emission;
	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
}
