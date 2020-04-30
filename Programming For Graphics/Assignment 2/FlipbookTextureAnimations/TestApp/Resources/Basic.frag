#version 450
uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

in vec2 FragTextureCoord;
out vec4 result;
//"uniform vec3 Color;"

in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

float ambientStrength = 1;
vec3 ambient = ambientStrength * FragLightColor;

void main()
{
	//vec3 normal = normalize(FragNormal);
	vec3 normal = normalize(texture2D(texture_normal, FragTextureCoord).rgb);
	normal = normalize(normal *2.0-1.0);
	normal = normalize(TBN*normal);

	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, lightDir),0.0);
	vec3 diffuse = diff * FragLightColor;
	//frag_colour = vec4(vec3(FragTextureCoord.x, FragTextureCoord.y,0), 1.0f);
	//frag_colour = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb,1);
	result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb * (ambient + diffuse), 1);
}