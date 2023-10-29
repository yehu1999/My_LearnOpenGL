//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<stb_image.h>

void how()
{
	Shader shader("", "");
	//���Դ
	/*
	������ǰ��Ľ̳����Ѿ�ѧϰ��������OpenGL�й��յ�֪ʶ��
	���а���������ɫ(Phong Shading)������(Material)��������ͼ(Lighting Map)�Լ���ͬ�����Ͷ����(Light Caster)��
	����һ���У����ǽ����֮ǰѧ��������֪ʶ������һ������������Դ�ĳ�����
	���ǽ�ģ��һ������̫���Ķ����(Directional Light)��Դ���ĸ���ɢ�ڳ����еĵ��Դ(Point Light)���Լ�һ���ֵ�Ͳ(Flashlight)��
	
	Ϊ���ڳ�����ʹ�ö����Դ������ϣ�������ռ����װ��GLSL�����С�
	��������ԭ���ǣ�ÿһ�ֹ�Դ����Ҫһ�ֲ�ͬ�ļ��㷽������һ��������Զ����Դ���й��ռ���ʱ������ܿ�ͻ��÷ǳ����ӡ�
	�������ֻ��main�����н������е���Щ���㣬����ܿ�ͻ���������⡣

	GLSL�еĺ�����C���������ƣ�����һ����������һ������ֵ���ͣ��������������main����֮ǰ�����ģ����ǻ������ڴ����ļ���������һ��ԭ�͡�
	���Ƕ�ÿ���������Ͷ�����һ����ͬ�ĺ���������⡢���Դ�;۹⡣

	�������ڳ�����ʹ�ö����Դʱ��ͨ��ʹ�����·�����������Ҫ��һ����������ɫ��������Ƭ�ε������ɫ��
	����ÿһ����Դ������Ƭ�εĹ�����ɫ����ӵ�Ƭ�ε������ɫ�����ϡ�
	���Գ����е�ÿ����Դ����������Ǹ��Զ�Ƭ�ε�Ӱ�죬�����Ϊһ�����յ������ɫ��
	����Ľṹ������������
	out vec4 FragColor;

	void main()
	{
	  // ����һ�������ɫֵ
	  vec3 output;
	  // �������Ĺ��׼ӵ������
	  output += someFunctionToCalculateDirectionalLight();
	  // �����еĵ��ԴҲ����ͬ������
	  for(int i = 0; i < nr_of_point_lights; i++)
		output += someFunctionToCalculatePointLight();
	  // Ҳ���������Ĺ�Դ������۹⣩
	  output += someFunctionToCalculateSpotLight();

	  FragColor = vec4(output, 1.0);
	}

	ʵ�ʵĴ����ÿһ��ʵ�ֶ����ܲ�ͬ��������Ľṹ���ǲ��ġ�
	���Ƕ����˼�����������������ÿ����Դ��Ӱ�죬�������յĽ����ɫ�ӵ������ɫ�����ϡ�
	���磬���������Դ���ܿ���һ��Ƭ�Σ���ô��������ϵĹ��׽����γ�һ���ȵ�����Դ����ʱ����������Ƭ�Ρ�
	*/

	//�����
	/*
	������Ҫ��Ƭ����ɫ���ж���һ�����������㶨������ӦƬ�εĹ��ף�������һЩ����������һ�����������ɫ��

	���ȣ�������Ҫ����һ�������Դ��������Ҫ�ı�����
	���ǿ��Խ���Щ����������һ������DirLight�Ľṹ���У�����������Ϊһ��uniform��
	��Ҫ�ı�������һ���ж����ܹ���
	struct DirLight 
	{
		vec3 direction;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};  
	uniform DirLight dirLight;
	
	���������ǿ��Խ�dirLight����һ����������ԭ�͵ĺ�����
	vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

	��C/C++һ����������������һ����������������main�����е��ã������������Ҫ�ڵ����ߵ�����֮ǰ���������
	��������������Ǹ�ϲ����main�������¶��庯������������Ҫ��Ͳ������ˡ�
	���ԣ�������Ҫ��main����֮�϶��庯����ԭ�ͣ����C��������һ���ġ�

	����Կ��������������Ҫһ��DirLight�ṹ��������������������м��㡣
	����������������һ�ڵĻ����������������Ӧ��������������ף�
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
	���ǻ�����ֻ�Ǵ���һ���и����˴��룬��ʹ�ú����������������������㶨���Ĺ���������
	���ջ����⡢������;����Ĺ��׽���ϲ�Ϊ������ɫ�������ء�
	*/

	//���Դ
	/*
	�Ͷ����һ��������Ҳϣ������һ�����ڼ�����Դ����ӦƬ�ι��ף��Լ�˥���ĺ�����
	ͬ�������Ƕ���һ�������˵��Դ�������б����Ľṹ�壺
	
	struct PointLight
	{
		vec3 position;

		float constant;
		float linear;
		float quadratic;

		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};
	#define NR_POINT_LIGHTS 4
	uniform PointLight pointLights[NR_POINT_LIGHTS];
	
	����Կ�����������GLSL��ʹ����Ԥ����ָ�������������ǳ����е��Դ��������
	��������ʹ�������NR_POINT_LIGHTS������������һ��PointLight�ṹ������顣
	GLSL�е������C����һ��������ʹ��һ�Է�������������
	�����������ĸ���������ݵ�PointLight�ṹ�塣

	����Ҳ���Զ���һ����Ľṹ�壨������Ϊÿ�����͵Ĺ�Դ���岻ͬ�Ľṹ�壩��
	�������в�ͬ�ֹ�����������ı�������������ṹ���õ����еĺ����У�
	ֻ��Ҫ�����ò����ı��������ˡ�
	Ȼ�����Ҹ��ˣ����ߣ����õ�ǰ�ķ������ֱ��һ�㣬�����ܹ���ʡһЩ���룬
	�������ڲ������й������Ͷ���Ҫ���еı���������Ҳ�ܽ�ʡһЩ�ڴ档

	���Դ������ԭ�����£�
	vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

	��������Ӳ����л�ȡ������������ݣ�������һ������õ��Դ��Ƭ�ε���ɫ���׵�vec3��
	������һ�δ����ش�֮ǰ�Ľ̳��и���ճ�����룬��������������ĺ�����
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

	����Щ���ܳ�������һ�������е��ŵ��ǣ������ܹ������ظ��Ĵ���������׵ؼ��������Դ�Ĺ����ˡ�
	��main�����У�����ֻ��Ҫ����һ��ѭ���������������Դ���飬��ÿ�����Դ����CalcPointLight�Ϳ����ˡ�
	*/

	//�ϲ����
	/*
	���������Ѿ�������һ�����㶨���ĺ�����һ��������Դ�ĺ����ˣ����ǿ��Խ����Ǻϲ��ŵ�main�����С�

	void main()
	{
		// ����
		vec3 norm = normalize(Normal);
		vec3 viewDir = normalize(viewPos - FragPos);

		// ��һ�׶Σ��������
		vec3 result = CalcDirLight(dirLight, norm, viewDir);
		// �ڶ��׶Σ����Դ
		for(int i = 0; i < NR_POINT_LIGHTS; i++)
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
		// �����׶Σ��۹�
		//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

		FragColor = vec4(result, 1.0);
	}
	
	ÿ����Դ���Ͷ������ǵĹ��׼ӵ������յ������ɫ�ϣ�ֱ�����еĹ�Դ���������ˡ�
	���յ���ɫ�����˳��������й�Դ����ɫӰ�����ϲ��Ľ����
	�������Ļ�����Ҳ����ʵ��һ���۹⣬��������Ч���ӵ������ɫ�С�
	���ǻὫCalcSpotLight��������������Ϊ��ϰ��
	*/
	//���ö����ṹ���uniformӦ�÷ǳ���Ϥ�ˣ���������ܻ��������Ǹ�������õ��Դ��uniformֵ��
	//��Ϊ���Դ��uniform������һ��PointLight�������ˡ��Ⲣ����������ǰ���۹��Ļ��⡣

	//�����˵��ǣ��Ⲣ���Ǻܸ��ӣ�����һ���ṹ�������uniform������һ���ṹ���uniform�Ǻ����Ƶģ�
	//������һ���ڷ���uniformλ�õ�ʱ��������Ҫ�����Ӧ�������±�ֵ��
	shader.setFloat("pointLights[0].constant", 1.0f);

	//����������������pointLights�����еĵ�һ��PointLight������ȡ��constant������λ�á�
	//����Ҳ��ζ�Ų��ҵ������Ǳ�������ĸ����Դ�ֶ�����uniformֵ�����õ��Դ����Ͳ�����28��uniform���ã��ǳ��߳���
	//��Ҳ���Գ��Խ���Щ�����ȥһ�㣬����һ�����Դ�࣬������Ϊ������uniformֵ�����������ȻҪ�����ַ�ʽ�������й�Դ��uniformֵ��
	glm::vec3 pointLightPositions[] = 
	{
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	//���������Ǵ�pointLights������������Ӧ��PointLight��������positionֵ����Ϊ�ոն����λ��ֵ�����е�����һ����
	//ͬʱ���ǻ�Ҫ��֤���ڻ��Ƶ����ĸ�������������ǽ���һ����
	//ֻҪ��ÿ�������崴��һ����ͬ��ģ�;���Ϳ����ˣ�������֮ǰ�����ӵĴ������ơ�


}