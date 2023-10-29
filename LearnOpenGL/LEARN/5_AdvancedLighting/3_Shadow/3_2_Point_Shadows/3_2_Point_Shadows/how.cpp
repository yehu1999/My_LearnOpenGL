//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>

unsigned int lightViewMatrices[1];
unsigned int depthCubemap, depthMapFBO;
void RenderScene();
void BindViewMatrix(unsigned int);
void ConfigureShaderAndMatrices();
int SCR_WIDTH, SCR_HEIGHT, SHADOW_WIDTH, SHADOW_HEIGHT;
glm::vec3 lightPos;
Shader shader("", "");

//���Դ��Ӱ
namespace how
{
	//�ϸ��̳�����ѧ�������ʹ����Ӱӳ�似��������̬��Ӱ��
	//Ч����������ֻ�ʺ϶���⣬��Ϊ��Ӱֻ���ڵ�һ�����Դ�����ɵġ�
	//������Ҳ�ж�����Ӱӳ�䣬��ȣ���Ӱ����ͼ�����Զ������ӽǡ�

	//�������ǵĽ������ڸ��ַ������ɶ�̬��Ӱ��
	//����������������ڵ��Դ���������з����ϵ���Ӱ��
	//����������������Ӱ����ȥ��������������Ӱ��ͼ��omnidirectional shadow maps��������

	//���ڴ������ǰ�����Ӱӳ��̳̣����������Դ�ͳ��Ӱӳ�䲻��Ϥ�����ǽ����ȶ�һ����Ӱӳ��̡̳� 
	//�㷨�Ͷ�����Ӱӳ���ࣺ���Ǵӹ��͸��ͼ����һ�������ͼ�����ڵ�ǰfragmentλ�����������ͼ������
	//Ȼ���ô�������ֵ��ÿ��fragment���жԱȣ��������Ƿ�����Ӱ�С�
	//������Ӱӳ���������Ӱӳ�����Ҫ��ͬ���������ͼ��ʹ���ϡ�

	//���������ͼ��������Ҫ��һ�����Դ��������Ⱦ��������ͨ2D�����ͼ���ܹ�����
	//�������ʹ����������ͼ��������
	//��Ϊ��������ͼ���Դ���6����Ļ������ݣ������Խ�����������Ⱦ����������ͼ��ÿ�����ϣ�
	//�����ǵ������Դ���ܵ����ֵ��������
	// https://learnopengl-cn.github.io/img/05/03/02/point_shadows_diagram.png

	//���ɺ�������������ͼ�����ݵ�����������ɫ����������һ������������������������ͼ���Ӷ��õ���ǰ��fragment����ȣ��ӹ��͸��ͼ����
	//�󲿷ָ��ӵ������Ѿ�����Ӱӳ��̳������۹��ˡ�
	//�㷨ֻ���������������ͼ��������΢����һ�㡣

	//���������������ͼ
	void how()
	{
		//Ϊ����һ������Χ�����ֵ����������ͼ�����Ǳ�����Ⱦ����6�Σ�ÿ��һ���档
		//��Ȼ��Ⱦ����6����Ҫ6����ͬ����ͼ����ÿ�ΰ�һ����ͬ����������ͼ�渽�ӵ�֡��������ϡ�
		//�⿴�����������ģ�
		for (int i = 0; i < 6; i++)
		{
			GLuint face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, depthCubemap, 0);
			BindViewMatrix(lightViewMatrices[i]);
			RenderScene();
		}

		//���ܺķ�������Ϊһ�������ͼ����Ҫ���кܶ���Ⱦ���á�
		//����̳������ǽ�ת��ʹ�������һ��С������������£�
		//������ɫ����������ʹ��һ����Ⱦ���������������������ͼ��

		//���ȣ�������Ҫ����һ����������ͼ��
		GLuint depthCubemap;
		glGenTextures(1, &depthCubemap);

		//Ȼ��������������ͼ��ÿ���棬��������Ϊ2D���ֵ����ͼ��
		const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		for (GLuint i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		//��Ҫ�������ú��ʵ����������
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//��������£����ǰ���������ͼ�����һ���渽�ӵ�֡��������ϣ�
		//��Ⱦ����6�Σ�ÿ�ν�֡�������Ȼ���Ŀ��ĳɲ�ͬ��������ͼ�档
		//�������ǽ�ʹ��һ��������ɫ�������������ǰ���������һ��������Ⱦ��
		//���ǿ���ʹ��glFramebufferTextureֱ�Ӱ���������ͼ���ӳ�֡�������ȸ�����
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//��Ҫ�ǵõ���glDrawBuffer��glReadBuffer�������Ƕ��趨ΪGL_NONE��
		//������һ�������������ͼʱ����ֻ�������ֵ��
		//�������Ǳ�����ʽ����OpenGL���֡������󲻻���Ⱦ��һ����ɫ�����

		//������Ӱ��ͼ��������Ⱦ�׶Σ������������������ͼ��Ȼ����������ʹ�������ͼ��Ⱦ���ڳ����д�����Ӱ��
		//֡����������������ͼ�Ĵ������������ģ�
		// 1. first render to depth cubemap
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		ConfigureShaderAndMatrices();
		RenderScene();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// 2. then render scene as normal with shadow mapping (using depth cubemap)
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ConfigureShaderAndMatrices();
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		RenderScene();

		//������̺�Ĭ�ϵ���Ӱӳ��һ�����������������Ⱦ��ʹ�õ���һ����������ͼ�������������2D�������
		//������ʵ�ʿ�ʼ�ӹ���ӽǵ����з�����Ⱦ����֮ǰ�������ȵü�������ʵı任����
	}

	//��ռ�ı任
	void how()
	{
		//������֡�������������ͼ��������ҪһЩ�����������������м�����任��6����ķ�������Ӧ�Ĺ�ռ䡣
		//����Ӱӳ��̳����ƣ����ǽ���Ҫһ����ռ�ı任����T�����������ÿ���涼��һ����

		//ÿ����ռ�ı任���������ͶӰ����ͼ����
		//����ͶӰ������˵�����ǽ�ʹ��һ��͸��ͶӰ���󣻹�Դ����һ���ռ��еĵ㣬����͸��ͶӰ����������塣
		//ÿ����ռ�任����ʹ��ͬ����ͶӰ����
		GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
		GLfloat near = 1.0f;
		GLfloat far = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);
		//�ǳ���Ҫ��һ���ǣ�����glm::perspective����Ұ����������Ϊ90�ȡ�
		//90�����ǲ��ܱ�֤��Ұ�㹻�󵽿��Ժ��ʵ�������������ͼ��һ���棬
		//��������ͼ�������涼�����������ڱ�Ե���롣

		//��ΪͶӰ������ÿ�������ϲ�����ı䣬���ǿ�����6���任�������ظ�ʹ�á�
		//����ҪΪÿ�������ṩһ����ͬ����ͼ����
		//��glm::lookAt����6���۲췽��ÿ������˳��ע������������ͼ�ĵ�һ������
		//�ҡ����ϡ��¡�����Զ��
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		//�������Ǵ�����6����ͼ���󣬰����ǳ���ͶӰ�������õ�6����ͬ�Ĺ�ռ�任����
		//glm::lookAt��target��������ע�ӵ���������ͼ�����һ������

		//��Щ�任�����͵���ɫ����Ⱦ����������ͼ�
	}

	//�����ɫ��
	void how()
	{
		//Ϊ�˰�ֵ��Ⱦ�������������ͼ�����ǽ���Ҫ3����ɫ����
		//�����Ƭ����ɫ�����Լ�һ������֮��ļ�����ɫ����

		//������ɫ���Ǹ�����������ռ�Ķ���任��6����ͬ�Ĺ�ռ����ɫ����
		//��˶�����ɫ���򵥵ؽ�����任������ռ䣬Ȼ��ֱ�ӷ��͵�������ɫ����
		/*
		#version 330 core
		layout (location = 0) in vec3 position;

		uniform mat4 model;

		void main()
		{
			gl_Position = model * vec4(position, 1.0);
		}
		*/

		//�����ż�����ɫ����3�������εĶ�����Ϊ���룬������һ����ռ�任�����uniform���顣
		//������ɫ���������Ḻ�𽫶���任����ռ䣻��������ʼ�����Ȥ�ˡ�

		//������ɫ����һ���ڽ���������gl_Layer����ָ����ɢ������ͼ���͵���������ͼ���ĸ��档
		//��������ʱ��������ɫ���ͻ�������һ�������Ļ���ͼ�η��͵����͹ܵ�����һ�׶Σ�
		//�������Ǹ�������������ܿ���ÿ������ͼ�ν���Ⱦ����������ͼ����һ���档
		//��Ȼ��ֻ�е���������һ�����ӵ������֡�������������ͼ�������Ч��
		/*
		#version 330 core
		layout (triangles) in;                            //ÿ����һ��������
		layout (triangle_strip, max_vertices=18) out;     //���6��������(18������)����������ͼ��ÿ�����һ��

		uniform mat4 shadowMatrices[6];                   //��ռ�任����

		out vec4 FragPos; // FragPos from GS (output per emitvertex)

		void main()
		{
			for(int face = 0; face < 6; ++face)
			{
				gl_Layer = face; // built-in variable that specifies to which face we render.
				for(int i = 0; i < 3; ++i) // for each triangle's vertices
				{
					FragPos = gl_in[i].gl_Position;
					gl_Position = shadowMatrices[face] * FragPos;   //�任����ռ�����
					EmitVertex();
				}
				EndPrimitive();  //��������Ϣ����Ƭ����ɫ����������ȡ���ֵ
			}
		}
		*/
		//������ɫ����Լ򵥡���������һ�������Σ�����ܹ�6�������Σ�6*3���㣬�����ܹ�18�����㣩��
		//��main�����У����Ǳ�����������ͼ��6���棬����ÿ����ָ��Ϊһ������棬��������interger���������浽gl_Layer��
		//Ȼ������ͨ������Ĺ�ռ�任�������FragPos����ÿ������ռ䶥��任����صĹ�ռ䣬����ÿ�������Ρ�
		//ע�⣬���ǻ�Ҫ������FragPos�������͸�������ɫ����������Ҫ����һ�����ֵ��

		//�ϸ��̳̣�����ʹ�õ���һ���յ�������ɫ������OpenGL���������ͼ�����ֵ��
		//������ǽ������Լ�����ȣ������Ⱦ���ÿ��fragmentλ�ú͹�Դλ��֮������Ծ��롣
		//�����Լ������ֵʹ��֮�����Ӱ�������ֱ�ۡ�
		/*
		#version 330 core
		in vec4 FragPos;

		uniform vec3 lightPos;
		uniform float far_plane;

		void main()
		{
			// get distance between fragment and light source
			float lightDistance = length(FragPos.xyz - lightPos);

			// map to [0;1] range by dividing by far_plane
			lightDistance = lightDistance / far_plane;

			// write this as modified depth
			gl_FragDepth = lightDistance;
		}
		*/

		//������ɫ�������Լ�����ɫ����FragPos�����λ����������׶��Զƽ��ֵ��Ϊ���롣
		//�������ǰ�fragment�͹�Դ֮��ľ��룬ӳ�䵽0��1�ķ�Χ������д��Ϊfragment�����ֵ��

		//ʹ����Щ��ɫ����Ⱦ��������������ͼ���ӵ�֡������󼤻��Ժ�
		//���õ�һ����ȫ���������������ͼ���Ա��ڽ��еڶ��׶ε���Ӱ���㡣
	}

	//������Ӱ��ͼ
	void how()
	{
		//�������鶼�����ˣ���ʱ������Ⱦ������Ӱ(Omnidirectional Shadow)�ˡ�
		//������̺Ͷ�����Ӱӳ��̳����ƣ�
		//����������ǰ󶨵������ͼ��һ����������ͼ��������2D�������ҽ����ͶӰ��Զƽ�淢�͸�����ɫ����
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		// ... send uniforms to shader (including light's far_plane value)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		// ... bind other textures
		RenderScene();

		//�����renderScene��������Ⱦһ���������巿����һЩ�����壬����ɢ���ڴ��������������Դ�ڳ������롣

		//������ɫ����������ɫ����ԭ������Ӱӳ����ɫ���󲿷ֶ�һ����
		//��֮ͬ�����ڹ�ռ���������ɫ��������Ҫһ��fragmentλ�ã��������ǿ���ʹ��һ�����������������ֵ��

		//��Ϊ���������ɫ��������Ҫ������λ�������任����ռ䣬�������ǿ���ȥ��FragPosLightSpace������
		/*
		#version 330 core
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoords;

		out vec2 TexCoords;

		out VS_OUT {
			vec3 FragPos;
			vec3 Normal;
			vec2 TexCoords;
		} vs_out;

		uniform mat4 projection;
		uniform mat4 view;
		uniform mat4 model;

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0f);
			vs_out.FragPos = vec3(model * vec4(position, 1.0));
			vs_out.Normal = transpose(inverse(mat3(model))) * normal;
			vs_out.TexCoords = texCoords;
		}
		*/

		//Ƭ����ɫ����Blinn-Phong���մ��������֮ǰ��Ӱ��˵Ľ�β����һ����
		/*
		#version 330 core
		out vec4 FragColor;

		in VS_OUT {
			vec3 FragPos;
			vec3 Normal;
			vec2 TexCoords;
		} fs_in;

		uniform sampler2D diffuseTexture;
		uniform samplerCube depthMap;

		uniform vec3 lightPos;
		uniform vec3 viewPos;



		float ShadowCalculation(vec3 fragPos)
		{
			[...]
		}

		void main()
		{
			vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
			vec3 normal = normalize(fs_in.Normal);
			vec3 lightColor = vec3(0.3);
			// Ambient
			vec3 ambient = 0.3 * color;
			// Diffuse
			vec3 lightDir = normalize(lightPos - fs_in.FragPos);
			float diff = max(dot(lightDir, normal), 0.0);
			vec3 diffuse = diff * lightColor;
			// Specular
			vec3 viewDir = normalize(viewPos - fs_in.FragPos);
			vec3 reflectDir = reflect(-lightDir, normal);
			float spec = 0.0;
			vec3 halfwayDir = normalize(lightDir + viewDir);
			spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
			vec3 specular = spec * lightColor;
			// Calculate shadow
			float shadow = ShadowCalculation(fs_in.FragPos);
			vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

			FragColor = vec4(lighting, 1.0f);
		}
		*/
		//��һЩϸ΢�Ĳ�ͬ�����մ���һ������������������һ��uniform����samplerCube��
		//shadowCalculation������fragment��λ����Ϊ���Ĳ�����ȡ���˹�ռ��fragmentλ�á�
		//�������ڻ�Ҫ��������׶��Զƽ��ֵ���������ǻ���Ҫ����
		//������ɫ����������Ǽ������ӰԪ�أ���fragment����Ӱ��ʱ����1.0��������Ӱ��ʱ��0.0��
		//����ʹ�ü����������ӰԪ��ȥӰ����յ�diffuse��specularԪ�ء�

		//��ShadowCalculation�������кܶ಻֮ͬ���������Ǵ���������ͼ�н��в���������ʹ��2D�����ˡ�
		//������һ��һ��������һ�µ��������ݡ�

		//������Ҫ���ĵ�һ�����ǻ�ȡ��������ͼ����ȡ�
		//������Ѿ��ӽ̵̳���������ͼ�����뵽�������Ѿ�����ȴ���Ϊfragment�͹�λ��֮��ľ����ˣ�
		//��������������ƵĴ���ʽ��
		/*
		float ShadowCalculation(vec3 fragPos)
		{
			vec3 fragToLight = fragPos - lightPos;
			float closestDepth = texture(depthMap, fragToLight).r;
		}
		*/
		//��������ǵõ���fragment��λ������λ��֮��Ĳ�������
		//ʹ�����������Ϊһ����������ȥ����������ͼ���в�����
		//������������Ҫ�ǵ�λ��������������������б�׼����
		//����closestDepth�ǹ�Դ������ӽ��Ŀɼ�fragment֮��ı�׼�������ֵ��

		//closestDepthֵ������0��1�ķ�Χ���ˣ����������Ƚ���ת����0��far_plane�ķ�Χ������Ҫ��������far_plane��
		// closestDepth *= far_plane;

		//��һ�����ǻ�ȡ��ǰfragment�͹�Դ֮������ֵ�����ǿ��Լ򵥵�ʹ��fragToLight�ĳ�������ȡ����
		//��ȡ����������μ�����������ͼ�е����ֵ��
		// float currentDepth = length(fragToLight);
		//���ص��Ǻ�closestDepth��Χ��ͬ�����ֵ��

		//�������ǿ��Խ��������ֵ�Ա�һ�£�������һ�����ӽ����Դ˾�����ǰ��fragment�Ƿ�����Ӱ���С�
		//���ǻ�Ҫ����һ����Ӱƫ�ƣ����Բ��ܱ�����Ӱʧ�棬����ǰ��̳����Ѿ����۹��ˡ�
		/*
		float bias = 0.05;
		float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
		*/

		//������ShadowCalculation���ڱ����������
		/*
		float ShadowCalculation(vec3 fragPos)
		{
			// Get vector between fragment position and light position
			vec3 fragToLight = fragPos - lightPos;
			// Use the light to fragment vector to sample from the depth map
			float closestDepth = texture(depthMap, fragToLight).r;
			// It is currently in linear range between [0,1]. Re-transform back to original value
			closestDepth *= far_plane;
			// Now get current linear depth as the length between the fragment and light position
			float currentDepth = length(fragToLight);
			// Now test for shadows
			float bias = 0.05;
			float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

			return shadow;
		}
		*/

		//������Щ��ɫ���������Ѿ��ܵõ��ǳ��õ���ӰЧ���ˣ���δ�һ�����Դ������Χ�����϶�����Ӱ��
	}

	//��ʾ��������ͼ��Ȼ���
	void how()
	{
		//���������һ����һ�β�û�����ԣ���ô��Ҫ���е����Ŵ��������ͼ��ʾ�����Լ�����Ƿ���ȷ��
		//��Ϊ���ǲ�����2D�����ͼ���������ͼ����ʾ������ô�Զ��׼���

		//һ���򵥵İ���Ȼ�����ʾ�����ļ����ǣ���ShadowCalculation�����м����׼����closestDepth�������ѱ�����ʾΪ��
		// FragColor = vec4(vec3(closestDepth / far_plane), 1.0);

		//�����һ���Ҷȳ�����ÿ����ɫ�����ų������������ֵ

		//�����Ҳע�⵽�˴���Ӱ������ǽ�⡣����������������࣬���֪�������������ͼ���ɵ�û��
		//���������������ʲô��Ҳ����closestDepth��Ȼ����0��far_plane�ķ�Χ��
	}

	//PCF
	void how()
	{
		//����������Ӱ��ͼ���ڴ�ͳ��Ӱӳ���ԭ������Ҳ�̳����ɽ����Ȳ����ķ���ʵ�С������Ŵ�ͻῴ����ݱ��ˡ�
		//PCF���Percentage-closer filtering��������ͨ����fragmentλ����Χ���˶�����������Խ��ƽ������

		//��������ú�ǰ��̳�ͬ�����Ǹ��򵥵�PCF�������������������ά�ȣ����������ģ�
		/*
		float shadow = 0.0;
		float bias = 0.05;
		float samples = 4.0;
		float offset = 0.1;
		for(float x = -offset; x < offset; x += offset / (samples * 0.5))
		{
			for(float y = -offset; y < offset; y += offset / (samples * 0.5))
			{
				for(float z = -offset; z < offset; z += offset / (samples * 0.5))
				{
					float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r;
					closestDepth *= far_plane;   // Undo mapping [0;1]
					if(currentDepth - bias > closestDepth)
						shadow += 1.0;
				}
			}
		}
		shadow /= (samples * samples * samples);
		*/
		//��δ�������Ǵ�ͳ����Ӱӳ��û�ж��ٲ�ͬ��
		//�������Ǹ���������������̬����������ƫ��������������������������Σ���������������ƽ������

		//������Ӱ�������������ƽ���ˣ��ɴ˵õ�������ʵ��Ч��

		//Ȼ����samples����Ϊ4.0��ÿ��fragment���ǻ�õ��ܹ�64����������̫���ˣ�

		//�������Щ�������Ƕ���ģ�������ԭʼ�������������������������ڲ������������Ĵ�ֱ������в����������塣
		//���ǣ�û�У��򵥵ģ���ʽ�ܹ�ָ����һ���ӷ����Ƕ���ģ�������ˡ�
		//�и����ɿ���ʹ�ã���һ��ƫ�����������飬���ǲ�඼�Ƿֿ��ģ�ÿһ��ָ����ȫ��ͬ�ķ����޳��˴˽ӽ�����Щ�ӷ���
		//�������һ������20��ƫ�Ʒ�������飺
		/*
		vec3 sampleOffsetDirections[20] = vec3[]
		(
		   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
		   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
		   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
		   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
		   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
		);
		*/

		//Ȼ�����ǰ�PCF�㷨���sampleOffsetDirections�õ������������������䣬ʹ�����Ǵ���������ͼ�������
		//��ô���ĺô�����֮ǰ��PCF�㷨��ȣ�������Ҫ���������������ˡ�
		/*
		float shadow = 0.0;
		float bias = 0.15;
		int samples = 20;
		float viewDistance = length(viewPos - fragPos);
		float diskRadius = 0.05;
		for(int i = 0; i < samples; ++i)
		{
			float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
			closestDepth *= far_plane;   // Undo mapping [0;1]
			if(currentDepth - bias > closestDepth)
				shadow += 1.0;
		}
		shadow /= float(samples);
		*/
		//�������ǰ�һ��ƫ������ӵ�ָ����diskRadius�У�����fragToLight����������Χ����������ͼ�������

		//��һ�����������Ӧ�õ�����˼�ļ����ǣ����ǿ��Ի��ڹ۲�����һ��fragment�ľ������ı�diskRadius��
		//�������Ǿ��ܸ��ݹ۲��ߵľ���������ƫ�ư뾶�ˣ��������Զ��ʱ����Ӱ����ͣ������˾͸�������
		// float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;

		//PCF�㷨�Ľ�����û�б�ø��ã�Ҳ�Ƿǳ�����ġ�

		//��Ȼ�ˣ�������ӵ�ÿ��������bias��ƫ�ƣ��߶������������ģ�����Ҫ���ݳ�������΢���ġ�
		//������Щֵ����������Ӱ���˳����� ���������հ汾�Ķ����������ɫ����
	}

	//�һ�Ҫ����һ��ʹ�ü�����ɫ�������������ͼ����һ����ÿ������Ⱦ����6�θ��졣
	//ʹ�ü�����ɫ�������Լ������ܾ��ޣ��ڵ�һ���׶�ʹ�������ܻ�ø��õ����ܱ��֡�
	//��ȡ���ڻ��������ͣ��Լ��ض����Կ������ȵȣ����������ܹ������ܣ���Ҫȷ�������ַ����д����˽⣬Ȼ��ѡ����㳡����˵����Ч���Ǹ���
	//�Ҹ��˻���ϲ��ʹ�ü�����ɫ����������Ӱӳ�䣬ԭ��ܼ򵥣���Ϊ����ʹ���������򵥡�
}
