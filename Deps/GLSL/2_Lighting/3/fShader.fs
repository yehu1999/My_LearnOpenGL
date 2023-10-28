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
		vec3 ambient;  //����������ɫ����
		vec3 diffuse;  //�����������ɫ����
		vec3 specular;  //���淴�������ɫ����
		float shininess;  //����ȣ�Ӱ�쾵��߹��ɢ��/�뾶��
	}; 
	uniform Material material;

	uniform vec3 viewPos;

	void main()
	{
		//������
		vec3 ambient = light.ambient * material.ambient;
		//������
		vec3 norm = normalize(Normal);//��������
		vec3 lightDir = normalize(light.position - FragPos);//���߷�������
		float diff = max(dot(norm, lightDir), 0.0f);//���diffֵ�������γɵļн����
		vec3 diffuse = light.diffuse * (diff * material.diffuse);
		//���淴��
		vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
		vec3 reflectDir = reflect(-lightDir, norm);//���߷��䷽������
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);//ָ���Ǹ߹�ķ����(Shininess)
		vec3 specular = light.specular * (spec * material.specular);

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
	}