	#version 330 core
	in vec3 FragPos;
	in vec3 Normal;
	in vec2 TexCoords;

	out vec4 FragColor;

	struct Material
	{
		vec3 ambient;  //环境光照颜色分量（保留，但是一般认为环境光与漫反射一致）
		sampler2D diffuse;//漫反射光照颜色分量
		sampler2D specular;//镜面反射光照颜色分量
		sampler2D emission;//外发光
		float     shininess;//反光度：影响镜面高光的散射/半径。
	};

	struct Light 
	{
		vec3 position;//点光源位置
		vec3 direction;//定向光方向

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	uniform Light light;
	uniform Material material;
	uniform vec3 viewPos;

	void main()
	{
		//环境光
		 vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
		//漫反射
		vec3 norm = normalize(Normal);//法线向量
		//vec3 lightDir = normalize(light.position - FragPos);//简单点光源光线方向向量
		vec3 lightDir = normalize(-light.direction);//定向光光线方向向量
		float diff = max(dot(norm, lightDir), 0.0f);//求得diff值与两者形成的夹角相关
		vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
		//镜面反射
		vec3 viewDir = normalize(viewPos - FragPos);//视线方向向量
		vec3 reflectDir = reflect(-lightDir, norm);//光线反射方向向量
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);//指数是高光的反光度(Shininess)
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
		//外发光
		vec3 emission = vec3(0.5f) * vec3(texture(material.emission, TexCoords));

		vec3 result = ambient + diffuse + specular ; //+ emission
		FragColor = vec4(result, 1.0f);
	}