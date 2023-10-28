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
		//��������
		float ambientStrength = 0.1f;//��������ǿ��
		vec3 ambient = ambientStrength * lightColor;
		//���������
		float diffuseStrength = 1.0f;//������ǿ��
		vec3 norm = normalize(Normal);//��������
		vec3 lightDir = normalize(lightPos - FragPos);//���߷�������
		float diff = max(dot(norm, lightDir), 0.0f);//���diffֵ�������γɵļн����
		vec3 diffuse = diff * lightColor;
		//���淴��
		float specularStrength = 0.5f;//���淴��ǿ��
		vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
		vec3 reflectDir = reflect(-lightDir, norm);//���߷��䷽������
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);//ָ��32�Ǹ߹�ķ����(Shininess)
		vec3 specular = specularStrength * spec * lightColor;

		vec3 result = (ambient + diffuse + specular) * objectColor;
		FragColor = vec4(result, 1.0f);
	}