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

uniform vec3 lightPositions[4];
uniform vec3 viewPos;
uniform bool Blinn;
uniform bool gamma;
uniform PointLight light;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;
//下面这貌似两个没用上
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{    
	vec3 color = vec3(texture(texture_diffuse1, TexCoords));
	vec3 ambient = 0.01 * color;
	vec3 lighting = vec3(0.0);
	for(int i = 0; i < 4; i++)
	{
		//属性
		vec3 norm = normalize(Normal);                //法线方向
		vec3 viewDir = normalize(viewPos - FragPos);  //视线方向
		vec3 lightDir = normalize(lightPositions[i] - FragPos);
		//漫反射着色
		float diff = max(dot(norm, lightDir), 0.0);
		//镜面光着色(Phong/BuLinPhong)
		float spec = 0.0;
		if(Blinn)
		{
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
		}
		else
		{
			vec3 reflectDir = reflect(-lightDir, norm);
			spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
		}
		// 衰减
		float distance    = length(lightPositions[i] - FragPos);
		float attenuation = 1.0 / (gamma ? distance * distance : distance);
		// 合并结果
		vec3 ambient  = light.ambient  * diff ;
		vec3 diffuse  = light.diffuse  * diff ;
		vec3 specular = light.specular * spec ;
		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;

		lighting += (diffuse + specular);
	}

	lighting += ambient;
	color *= lighting;

	if(gamma)
        color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
}