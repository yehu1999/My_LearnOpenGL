#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
//������ò������û����
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_reflect1;

void main()
{    
	vec3 ambient  = vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse  = vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = vec3(texture(texture_specular1, TexCoords));


	//û�ҵ��������£����Լ��������ý�reflectֵ���ھ��淴����Ч�����
	vec3 result = 0.01 * ambient + diffuse;

	FragColor = vec4(result, 1.0);
}