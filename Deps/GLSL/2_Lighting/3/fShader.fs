	#version 330 core
	in vec3 FragPos;
	in vec3 Normal;

	out vec4 FragColor;

	struct Light 
	{
		vec3 position;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
	uniform Light light;

	struct Material 
	{
		vec3 ambient;  //环境光照颜色分量
		vec3 diffuse;  //漫反射光照颜色分量
		vec3 specular;  //镜面反射光照颜色分量
		float shininess;  //反光度：影响镜面高光的散射/半径。
	}; 
	uniform Material material;

	uniform vec3 viewPos;

	void main()
	{
		//环境光
		vec3 ambient = light.ambient * material.ambient;
		//漫反射
		vec3 norm = normalize(Normal);//法线向量
		vec3 lightDir = normalize(light.position - FragPos);//光线方向向量
		float diff = max(dot(norm, lightDir), 0.0f);//求得diff值与两者形成的夹角相关
		vec3 diffuse = light.diffuse * (diff * material.diffuse);
		//镜面反射
		vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
		vec3 reflectDir = reflect(-lightDir, norm);//光线反射方向向量
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);//指数是高光的反光度(Shininess)
		vec3 specular = light.specular * (spec * material.specular);

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
	}