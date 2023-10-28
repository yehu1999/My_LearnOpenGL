	#version 330 core
	in vec3 FragPos;
	in vec3 Normal;
	in vec2 TexCoords;

	out vec4 FragColor;

	struct Material
	{
		vec3 ambient;  //����������ɫ����������������һ����Ϊ��������������һ�£�
		sampler2D diffuse;//�����������ɫ����
		sampler2D specular;//���淴�������ɫ����
		sampler2D emission;//�ⷢ��
		float     shininess;//����ȣ�Ӱ�쾵��߹��ɢ��/�뾶��
	};

	struct Light 
	{
		//λ��
		vec3 position;
		//һ�����
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		//����˥������
		float constant;
		float linear;
		float quadratic;
	};

	uniform Light light;
	uniform Material material;
	uniform vec3 viewPos;

	void main()
	{
		//������
		 vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
		//������
		vec3 norm = normalize(Normal);//��������
		vec3 lightDir = normalize(light.position - FragPos);//���߷�������
		float diff = max(dot(norm, lightDir), 0.0f);//���diffֵ�������γɵļн����
		vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
		//���淴��
		vec3 viewDir = normalize(viewPos - FragPos);//���߷�������
		vec3 reflectDir = reflect(-lightDir, norm);//���߷��䷽������
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);//ָ���Ǹ߹�ķ����(Shininess)
		vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
		//�ⷢ��
		vec3 emission = vec3(0.5f) * vec3(texture(material.emission, TexCoords));

		//����˥��
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
					light.quadratic * (distance * distance));
		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;

		vec3 result = ambient + diffuse + specular ; //+ emission
		FragColor = vec4(result, 1.0f);
	}