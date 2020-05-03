#version 450
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

in vec2 FragTextureCoord;
in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;

out vec4 frag_colour;

void main()
{
	float ambientStrength = 1.0f;
	vec3 ambient = ambientStrength * FragLightColor;

	vec3 normal = normalize(texture(texture_normal, FragTextureCoord).rgb);
	normal = normalize(normal * 2.0-1.0);
	normal = normalize(TBN*normal);
	
	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, lightDir),0.0);
	vec3 diffuse = diff * FragLightColor;

	//Specular
	float specularStrength = 80f;
	vec3 viewDir = normalize(FragCamPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0),32.0);
	vec3 specular = vec3(specularStrength * spec);

	vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb * (ambient + diffuse + specular), 1);
	frag_colour = result;
}