	#version 330 core
	in vec3 FragPos;
	in vec3 Normal;

	out vec4 FragColor;

	uniform vec3 objectColor;
	uniform vec3 lightColor;
	uniform vec3 lightPos;
	uniform vec3 viewPos;

	void main()
	{
		//环境光照
		float ambientStrength = 0.1f;//环境反射强度
		vec3 ambient = ambientStrength * lightColor;
		//漫反射光照
		float diffuseStrength = 1.0f;//漫反射强度
		vec3 norm = normalize(Normal);//法线向量
		vec3 lightDir = normalize(lightPos - FragPos);//光线方向向量
		float diff = max(dot(norm, lightDir), 0.0f);//求得diff值与两者形成的夹角相关
		vec3 diffuse = diff * lightColor;
		//镜面反射
		float specularStrength = 0.5f;//镜面反射强度
		vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
		vec3 reflectDir = reflect(-lightDir, norm);//光线反射方向向量
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);//指数32是高光的反光度(Shininess)
		vec3 specular = specularStrength * spec * lightColor;

		vec3 result = (ambient + diffuse + specular) * objectColor;
		FragColor = vec4(result, 1.0f);
	}