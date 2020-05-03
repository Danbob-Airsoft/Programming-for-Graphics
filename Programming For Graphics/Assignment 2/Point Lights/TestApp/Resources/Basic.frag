#version 450
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

in vec2 FragTextureCoord;
out vec4 result;

in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;

//in float LightIntensity;

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

float ambientStrength;

void main()
{
	ambientStrength = 1f;
	float LightIntensity = 0f;

	vec3 ambient = ambientStrength * FragLightColor;
	vec3 normal = normalize(texture2D(texture_normal, FragTextureCoord).rgb);
	normal = normalize(normal *2.0-1.0);
	normal = normalize(TBN*normal);

	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, lightDir),0.0);
	diff -= LightIntensity;
	vec3 diffuse = diff * FragLightColor;

	//Specular
	float specularStrength = 10f - LightIntensity;
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(normal, reflectDir), 0.0),32.0);
	vec3 specular = vec3(specularStrength * spec);

	result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb * (ambient + diffuse + specular), 1);
}