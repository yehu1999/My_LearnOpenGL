#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct PointLight          //���Դ
{
	//��Դλ��
	vec3 position;

	//���ղ���
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//����˥������
	float constant;
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform PointLight light;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;
//������ò������û����
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{    
	//����
	vec3 norm = normalize(Normal);                //���߷���
	vec3 viewDir = normalize(viewPos - FragPos);  //���߷���
	vec3 lightDir = normalize(light.position - FragPos);

	// ��������ɫ
	float diff = max(dot(norm, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// ˥��
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
					light.quadratic * (distance * distance));
	// �ϲ����
	vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}