#version 330 core
//����
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
//���
out vec4 FragColor;
//�ṹ��
struct Material            //����
{
	vec3 ambient;              //����������ɫ����������������һ����Ϊ��������������һ�£�
	sampler2D diffuse;         //�����������ɫ����
	sampler2D specular;        //���淴�������ɫ����
	sampler2D emission;        //�ⷢ��
	float     shininess;       //����ȣ�Ӱ�쾵��߹��ɢ��/�뾶��
};
struct DirLight            //�����
{
	//���շ���
	vec3 direction;

	//���ղ���
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
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
struct SpotLight           //�۹�
{
	//�۹����
	vec3 position;     //�۹��Դλ��
	vec3  direction;   //���䷽��
	float cutOff;      //���䷶Χ���нǵ�cosֵ
	float outerCutOff; //���䷶Χ���нǵ�cosֵ(�����Ե)

	//���ղ���
	vec3 ambient;     
	vec3 diffuse;
	vec3 specular;

	//����˥������
	float constant;  
	float linear;    
	float quadratic; 
};
//����
#define NR_POINT_LIGHTS 4  //���Դ����
//�������
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;     
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
//��������
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//�����
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//���Դ
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//�۹�

//������
void main()
{
	//����
	vec3 norm = normalize(Normal);                //���߷���
	vec3 viewDir = normalize(viewPos - FragPos);  //���߷���

	//�������
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	//���Դ
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
	//�۹�
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

	FragColor = vec4(result, 1.0);
}
//��������
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// �ϲ����
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// ˥��
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
					light.quadratic * (distance * distance));
	// �ϲ����
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
vec3 CalcSpotLight(SpotLight light, vec3 Normal, vec3 FragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - FragPos);//���߷�������

	//������
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	//������
	vec3 norm = normalize(Normal);//��������
	float diff = max(dot(norm, lightDir), 0.0f);//���diffֵ�������γɵļн����
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	//���淴��
	vec3 reflectDir = reflect(-lightDir, norm);//���߷��䷽������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);//ָ���Ǹ߹�ķ����(Shininess)
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	//�ⷢ��
	vec3 emission = vec3(0.5f) * vec3(texture(material.emission, TexCoords));

	//�۹�����Լ��
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;

	//����˥��
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	diffuse  *= attenuation;
	specular *= attenuation;
		
	return (ambient + diffuse + specular); //+ emission
}