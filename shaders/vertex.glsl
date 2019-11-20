#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 Normal;
out vec3 FragPos;
out vec3 ObjectColor;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0f);
	Normal = vec3(model * vec4(aNormal, 1.0f));
	FragPos = vec3(model * vec4(aPos, 1.0f));
	TexCoords = normalize(aPos.xy);
	ObjectColor = aColor;
}
