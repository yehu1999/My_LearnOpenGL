#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct PointLight          //点光源
{
	//光源位置
	vec3 position;

	//光照参数
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//光线衰弱参数
	float constant;
	float linear;
	float quadratic;
};

uniform vec3 viewPos;
uniform PointLight light;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;
//下面这貌似两个没用上
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{    
	//属性
	vec3 norm = normalize(Normal);                //法线方向
	vec3 viewDir = normalize(viewPos - FragPos);  //视线方向
	vec3 lightDir = normalize(light.position - FragPos);

	// 漫反射着色
	float diff = max(dot(norm, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// 衰减
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
					light.quadratic * (distance * distance));
	// 合并结果
	vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0);
}