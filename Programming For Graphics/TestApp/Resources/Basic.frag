#version 450
uniform sampler2D texture_diffuse;

in vec2 FragTextureCoord;
out vec4 frag_colour;
//"uniform vec3 Color;"

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

float ambientStrength = 0.5;
vec3 ambient = ambientStrength * FragLightColor;

void main()
{
	//frag_colour = vec4(vec3(FragTextureCoord.x, FragTextureCoord.y,0), 1.0f);
	//frag_colour = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb,1);
	vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb * ambient, 1);
}