#version 330 core
//输入
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
//输出
out vec4 FragColor;
//结构体
struct Material            //材质
{
	vec3 ambient;              //环境光照颜色分量（保留，但是一般认为环境光与漫反射一致）
	sampler2D diffuse;         //漫反射光照颜色分量
	sampler2D specular;        //镜面反射光照颜色分量
	sampler2D emission;        //外发光
	float     shininess;       //反光度：影响镜面高光的散射/半径。
};
struct DirLight            //定向光
{
	//光照方向
	vec3 direction;

	//光照参数
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
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
struct SpotLight           //聚光
{
	//聚光参数
	vec3 position;     //聚光光源位置
	vec3  direction;   //照射方向
	float cutOff;      //照射范围内切角的cos值
	float outerCutOff; //照射范围外切角的cos值(渐变边缘)

	//光照参数
	vec3 ambient;     
	vec3 diffuse;
	vec3 specular;

	//光线衰弱参数
	float constant;  
	float linear;    
	float quadratic; 
};
//常量
#define NR_POINT_LIGHTS 4  //点光源数量
//传入变量
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;     
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
//函数声明
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//定向光
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//点光源
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//聚光

//主函数
void main()
{
	//属性
	vec3 norm = normalize(Normal);                //法线方向
	vec3 viewDir = normalize(viewPos - FragPos);  //视线方向

	//定向光照
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	//点光源
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
	//聚光
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

	FragColor = vec4(result, 1.0);
}
//函数定义
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 合并结果
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 衰减
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
					light.quadratic * (distance * distance));
	// 合并结果
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
	vec3 lightDir = normalize(light.position - FragPos);//光线方向向量

	//环境光
	vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
	//漫反射
	vec3 norm = normalize(Normal);//法线向量
	float diff = max(dot(norm, lightDir), 0.0f);//求得diff值与两者形成的夹角相关
	vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	//镜面反射
	vec3 reflectDir = reflect(-lightDir, norm);//光线反射方向向量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);//指数是高光的反光度(Shininess)
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	//外发光
	vec3 emission = vec3(0.5f) * vec3(texture(material.emission, TexCoords));

	//聚光照射约束
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;

	//光线衰弱
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	diffuse  *= attenuation;
	specular *= attenuation;
		
	return (ambient + diffuse + specular); //+ emission
}