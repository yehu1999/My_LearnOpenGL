//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>

#include <random> //���������

GLuint gPositionDepth, SCR_WIDTH, SCR_HEIGHT, gBuffer, gNormal, noiseTexture;
glm::mat4 projection;
GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
std::default_random_engine generator;
Shader shaderSSAO("","");
Shader shaderLightingPass("","");
void SendKernelSamplesToShader();
void RenderQuad();


//SSAO (Screen-Space Ambient Occlusion ��Ļ�ռ价�����ڱ�)
namespace how
{
	//����
	void what()
	{
		//�����Ѿ���ǰ��Ļ����̳��м򵥽��ܵ����ⲿ�����ݣ���������(Ambient Lighting)��
		//�������������Ǽ��볡����������е�һ���̶����ճ�������������ģ����ɢ��(Scattering)��
		//����ʵ�У����߻������ⷽ��ɢ�䣬����ǿ���ǻ�һֱ�ı�ģ����Լ�ӱ��յ����ǲ��ֳ���ҲӦ���б仯��ǿ�ȣ�������һ�ɲ���Ļ����⡣
		//����һ�ּ�ӹ��յ�ģ������������ڱ�(Ambient Occlusion)��
		//����ԭ����ͨ�������塢�׶��ͷǳ�������ǽ��䰵�ķ�������ģ�����ӹ��ա�
		//��Щ����ܴ�̶����Ǳ���Χ�ļ������ڱεģ����߻������ʧ��������Щ�ط������������һЩ��
		//վ������һ���㷿��Ĺսǻ��������壬�ǲ�����Щ�ط��ῴ������һ�㰵��

		//�������ͼչʾ����ʹ�úͲ�ʹ��SSAOʱ�����Ĳ�ͬ��
		//�ر�ע��Ա����岿�֣���ᷢ��(����)�ⱻ�ڱ�����ࣺ
		// https://learnopengl-cn.github.io/img/05/09/ssao_example.png

		//�����ⲻ��һ���ǳ����Ե�Ч��������SSAO��ͼ��ȷʵ�����Ǹ���ʵ�ĸо�����ЩС���ڱ�ϸ�ڸ��������������˸�ǿ����ȸС�

		//�������ڱ���һ����������ܴ�����ܿ�������Ϊ������Ҫ������Χ�ļ����塣
		//���ǿ��ԶԿռ���ÿһ�㷢�����������ȷ�����ڱ�������������ʵʱ�����л�ܿ��ɴ����⡣
		//��2007�꣬Crytek��˾������һ�������Ļ�ռ价�����ڱ�(Screen-Space Ambient Occlusion, SSAO)�ļ����������������ǵĿ������µ�Σ���ϡ�
		//��һ����ʹ������Ļ�ռ䳡������ȶ�������ʵ�ļ�����������ȷ���ڱ�����
		//��һ��������������Ļ������ڱβ����ٶȿ죬���һ��ܻ�úܺõ�Ч����ʹ������Ϊ����ʵʱ�������ڱεı�׼��

		//SSAO�����ԭ��ܼ򵥣�
		//���������ı���(Screen-filled Quad)�ϵ�ÿһ��Ƭ�Σ����Ƕ�������ܱ����ֵ����һ���ڱ�����(Occlusion Factor)��
		//����ڱ�����֮��ᱻ�������ٻ��ߵ���Ƭ�εĻ������շ�����
		//�ڱ�������ͨ���ɼ�Ƭ����Χ���ͺ���(Kernel)�Ķ��������������͵�ǰƬ�����ֵ�Աȶ��õ��ġ�
		//����Ƭ�����ֵ�����ĸ�������������Ҫ���ڱ����ӡ�
		// https://learnopengl-cn.github.io/img/05/09/ssao_crysis_circle.png

		//��ͼ���ڼ������ڻ�ɫ������������Ǹ���Ƭ�����ֵ�ģ����ǻ������ڱ����ӣ�
		//��������������������Խ�࣬Ƭ�λ�õĻ�������Ҳ��Խ�١�

		//�����ԣ���ȾЧ���������;��������ǲ���������������ֱ�ӹ�ϵ��
		//�����������̫�ͣ���Ⱦ�ľ��Ȼἱ����٣����ǻ�õ�һ�ֽ�������(Banding)��αӰ��
		//���������̫���ˣ��ֻ�Ӱ�����ܡ�Ϊ�˽���������ܵ�Ӱ�죬
		//���ǿ���ͨ����������Ե������ں�(Sample Kernel)�дӶ�������������Ŀ��
		//ͨ�������תÿ��Ƭ�εĲ����ںˣ����������������������еõ��������Ľ����
		//Ȼ������Ȼ����һ�����鷳����Ϊ�����������һ�������Ե�����ģʽ��
		//���Ǳ���ͨ��ģ��������޸���һ���⡣
		//�������ͼƬ(John Chapman�ķ���)չʾ�˲���ЧӦ��������ԶԽ����Ӱ�죺
		// https://learnopengl-cn.github.io/img/05/09/ssao_banding_noise.jpg

		//����Կ����������������ڵ�������������£�Ҳ�õ��˺����ԵĲ���Ч����
		//�����������֮����Щ����Ч������ȫ��ʧ�ˡ�

		//Crytek��˾������SSAO���������һ��������Ӿ����
		//��Ϊʹ�õĲ����ں���һ�����壬������ƽ����ǽ��Ҳ���Եû����ɵģ�
		//��Ϊ�ں���һ�������������ǽ����������ϡ�
		//�������ͼչʾ�˹µ�Σ����SSAO����������չʾ�����ֻ����ɵĸо���https://learnopengl-cn.github.io/img/05/09/ssao_crysis.jpg

		//�������ԭ�����ǽ�����ʹ������Ĳ����ںˣ���ʹ��һ�����ű��淨�����İ���������ںˡ�
		// https://learnopengl-cn.github.io/img/05/09/ssao_hemisphere.png

		//ͨ���ڷ��������(Normal-oriented Hemisphere)��Χ���������ǽ����ῼ�ǵ�Ƭ�εײ��ļ�����.
		//��������SSAO�����ɵĸо����Ӷ���������ʵ�Ľ����
		//���SSAO�̳̽�����ڷ�����򷨺�John Chapman��ɫ��SSAO�̡̳�
	}

	//��������
	void how()
	{
		//SSAO��Ҫ��ȡ���������Ϣ����Ϊ������ҪһЩ��ʽ��ȷ��һ��Ƭ�ε��ڱ����ӡ�
		//����ÿһ��Ƭ�Σ����ǽ���Ҫ��Щ���ݣ�
		/*
		��Ƭ��λ������
		��Ƭ�εķ�������
		��Ƭ�εķ�����ɫ
		�����ں�
		������ת�����ں˵������תʸ��
		*/

		//ͨ��ʹ��ÿ��Ƭ�ε���ͼ�ռ�λ�ã����ǿ��Խ�һ�����������ں˶�׼Ƭ�ε���ͼ�ռ���淨�ߣ�
		//��ʹ�ô˺˶Բ�ͬƫ�Ƶ�λ�û�����������в�����
		//����ÿһ��Ƭ�ε��ں����������ǽ��������λ�û������е���Ƚ��бȽϣ���ȷ���ڱ�����
		//Ȼ�����õ��ڱ����������������յĻ����������
		//ͨ��ÿ��Ƭ�ε���ת���壬���ǿ������ż���������Ҫ�ɼ�������������
		//https://learnopengl-cn.github.io/img/05/09/ssao_overview.png

		//����SSAO��һ����Ļ�ռ似�������Ǽ������Գ�����Ļ�� 2D �ı�����ÿ��Ƭ�ε�Ӱ�죻
		//Ҳ����˵����û�г����еļ�����Ϣ��
		//���ǿ��������ǽ�ÿ��Ƭ�εļ���������Ⱦ����Ļ�ռ�����
		//Ȼ�����ǽ��䷢�͵� SSAO ��ɫ�����Ա����ǿ��Է���ÿ��Ƭ�εļ��������ˡ�
		//����㿴��ǰ��һƪ�̳̣���ᷢ������ӳ���Ⱦ����G�����������ú����ơ�
		//��ˣ�SSAO�ǳ��ʺ����ӳ���Ⱦ���ʹ�ã���Ϊ�����Ѿ���G��������ӵ��λ�úͷ���������

		//������̳��У����ǽ�������һ�ڵ��ӳ���Ⱦ��(�ӳ���ɫ���̳���)�Ļ�����ʵ��SSAO

		//---------------------------------------------------����������Ӣ���ĵ��г���

		//���������Ѿ�������Ƭ��λ�úͷ�������(G������)��
		//����ֻ��Ҫ����һ�¼�����ɫ������������Ƭ�ε�������Ⱦ����ˡ�
		//������������Ȳ�����һ��ѧ����֪ʶ�����ǿ��Դ�gl_FragCoord.z����ȡ������ȣ�
		/*
		#version 330 core
		layout (location = 0) out vec4 gPositionDepth;
		layout (location = 1) out vec3 gNormal;
		layout (location = 2) out vec4 gAlbedoSpec;

		in vec2 TexCoords;
		in vec3 FragPos;
		in vec3 Normal;

		const float NEAR = 0.1; // ͶӰ����Ľ�ƽ��
		const float FAR = 50.0f; // ͶӰ�����Զƽ��
		float LinearizeDepth(float depth)
		{
			float z = depth * 2.0 - 1.0; // �ص�NDC
			return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
		}

		void main()
		{
			// ����Ƭ�ε�λ��ʸ������һ��G��������
			gPositionDepth.xyz = FragPos;
			// ����������ȵ�gPositionDepth��alpha����
			gPositionDepth.a = LinearizeDepth(gl_FragCoord.z);
			// ���淨����Ϣ��G����
			gNormal = normalize(Normal);
			// ����������ɫ
			gAlbedoSpec.rgb = vec3(0.95);
		}
		*/

		//��ȡ�������������������ͼ�ռ��еģ�����֮�������Ҳ������ͼ�ռ��С�
		//ȷ��G�����е�λ�úͷ��߶�����ͼ�ռ���(������ͼ����Ҳһ��)��
		//��ͼ�ռ��������ֵ֮��ᱻ������gPositionDepth��ɫ�����alpha�����У�ʡ������������һ���µ���ɫ��������

		//ͨ��һЩС������ͨ�����ֵ�ع�ʵ��λ��ֵ�ǿ��ܵģ�Matt Pettineo�����Ĳ������ᵽ����һ���ɡ�
		//��һ������Ҫ����ɫ�������һЩ���㣬����ʡ��������G�����д洢λ�����ݣ��Ӷ�ʡ�˺ܶ��ڴ档
		//Ϊ��ʾ���ļ򵥣����ǽ�����ʹ����Щ�Ż����ɣ����������̽����

		//gPositionDepth��ɫ�����������ó�������������
		glGenTextures(1, &gPositionDepth);
		glBindTexture(GL_TEXTURE_2D, gPositionDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//���������һ����������������ǿ�����������ÿһ������������ȡ���ֵ��
		//ע�����ǰ��������ֵ�洢Ϊ�˸������ݣ�
		//������0.1��50.0��Χ���ֵ�����ᱻ������[0.0, 1.0]֮���ˡ�
		//����㲻�ø���ֵ�洢��Щ������ݣ�ȷ�������Ƚ�ֵ����FAR����׼�����ǣ��ٴ洢��gPositionDepth�����У�
		//�����Ժ����ɫ���������Ƶķ����ؽ����ǡ�
		//ͬ����Ҫע�����GL_CLAMP_TO_EDGE��������ģʽ��
		//�Ᵽ֤�����ǲ��᲻С�Ĳ���������Ļ�ռ�������Ĭ����������֮������ֵ��

		//������������Ҫ�����İ���������ĺ�һЩ�����������ת����
	}

	//�������
	void how()
	{
		//������Ҫ����һЩ�ر��淨�߷����������
		//���������ڱ��¿�ͷ��Ҫ���۹��ģ�����ϣ�����ɵ������γ�һ������
		//����Ϊÿ�����淨�߷�������һ�������ں˷ǳ����ѣ�Ҳ����ʵ�ʣ�
		//������ǽ������߿ռ�����һ�������ںˣ���������ָ���� Z ����
		// https://learnopengl-cn.github.io/img/05/09/ssao_hemisphere.png

		//����������һ����λ�������ǿ��Եõ�һ�������64������ֵ�������ںˣ�������ʾ��
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // �������������Χ0.0 - 1.0
		std::default_random_engine generator;
		std::vector<glm::vec3> ssaoKernel;
		for (GLuint i = 0; i < 64; ++i)
		{
			glm::vec3 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator)
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			GLfloat scale = GLfloat(i) / 64.0;
			ssaoKernel.push_back(sample);

			//������-1.0 �� 1.0 ֮��ı����߿ռ�� x �� y ����
			//���� 0.0 �� 1.0 ֮��ı������� z ����
			//(�������Ҳ��-1.0 �� 1.0 ֮��ı� z ���򣬾ͻ�õ�һ�����������ں�)��
			//���ڲ������Ľ������ű��淨�߶������õ�����ʸ�������ڰ����

			//Ŀǰ������������������ֲ��������ں��У�
			//�����Ǹ�ϣ���ڿ���ʵ��Ƭ�ε��ڵ���������Ȩ�أ�
			//Ҳ���ǽ�������ں������ֲ��ڸ�����ԭ��ĵط���
			//���ǿ����ü��ٲ�ֵ����ʵ������

			//...[���Ϻ���]
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}
		//���ٲ�ֵ����lerp������Ϊ��
		/*
		GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
		{
			return a + f * (b - a);
		}
		*/
		//��͸�������һ���󲿷���������ԭ��ĺ��ķֲ���
		// https://learnopengl-cn.github.io/img/05/09/ssao_kernel_weight.png

		//ÿ�������ں˽��ᱻ����ƫ����ͼ�ռ�Ƭ��λ�ôӶ�������Χ�ļ����塣
		//Ϊ�˻�ñ���Ľ��������ȷʵ��Ҫ����ͼ�ռ��н��д�������������ܻ���������̫��Ӱ�졣
		//������ͨ������һ�������ת�����������У����ǿ��Ժܴ�̶��ϼ������������������
	}

	//����ں���ת
	void how()
	{
		//ͨ������һЩ����Ե����������ϣ����ǿ��Դ����ٻ�ò��������������������
		//���ǿ��ԶԳ�����ÿһ��Ƭ�δ���һ�������ת�����������ܿ콫�ڴ�ľ���
		//���ԣ����õķ����Ǵ���һ��С�������ת��������ƽ������Ļ�ϡ�

		//���Ǵ���һ��4x4�������߿ռ�ƽ�淨�ߵ������ת�������飺
		std::vector<glm::vec3> ssaoNoise;
		for (GLuint i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
			ssaoNoise.push_back(noise);
		}
		//���ڲ����ں���������z���������߿ռ�����ת�������趨z����Ϊ0.0���Ӷ�Χ��z����ת��

		//���ǽ���������һ�����������ת������4x4����
		//�ǵ��趨���Ļ��Ʒ�ʽΪGL_REPEAT���Ӷ���֤�����ʵ�ƽ������Ļ�ϡ�
		GLuint noiseTexture;
		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//���������������е�����������ݣ�������������Ҫʵ��SSAO��
	}

	//SSAO��ɫ��
	void how()
	{
		//SSAO��ɫ����2D�������ı��������У�
		//������ÿһ�����ɵ�Ƭ�μ����ڱ�ֵ(Ϊ�������յĹ�����ɫ����ʹ��)��
		//����������Ҫ�洢SSAO�׶εĽ�������ǻ���Ҫ�ڴ���һ��֡�������
		GLuint ssaoFBO;
		glGenFramebuffers(1, &ssaoFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		GLuint ssaoColorBuffer;

		glGenTextures(1, &ssaoColorBuffer);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

		//���ڻ����ڱεĽ����һ���Ҷ�ֵ�����ǽ�ֻ��Ҫ����ĺ�ɫ�������������ǽ���ɫ������ڲ���ʽ����ΪGL_RED��

		//��ȾSSAO�����Ĺ��̻���������
		// ���δ���׶�: ��Ⱦ��G������
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		//[...]
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ʹ��G������ȾSSAO���� 
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSSAO.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPositionDepth); 
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		SendKernelSamplesToShader();
		shaderSSAO.setMat4("projection", projection);
		RenderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ���մ���׶�: ��Ⱦ��������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderLightingPass.use();
		//[...]
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		//[...]
		RenderQuad();

		//shaderSSAO�����ɫ������ӦG��������(�����������)����������ͷ�������ں�������Ϊ���������
		/*
		#version 330 core
		out float FragColor;
		in vec2 TexCoords;

		uniform sampler2D gPositionDepth;
		uniform sampler2D gNormal;
		uniform sampler2D texNoise;

		uniform vec3 samples[64];
		uniform mat4 projection;

		// ��Ļ��ƽ����������������Ļ�ֱ��ʳ���������С��ֵ������
		const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); // ��Ļ = 800x600

		void main()
		{
			[...]
		}
		*/
		//ע������������һ��noiseScale�ı�����
		//������Ҫ����������ƽ��(Tile)����Ļ�ϣ���������TexCoords��ȡֵ��0.0��1.0֮�䣬texNoise��������ƽ�̡�
		//�������ǽ�ͨ����Ļ�ֱ��ʳ������������С�ķ�ʽ����TexCoords�����Ŵ�С��
		/*
		vec3 fragPos = texture(gPositionDepth, TexCoords).xyz;
		vec3 normal = texture(gNormal, TexCoords).rgb;
		vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
		*/
		//�������ǽ�texNoise��ƽ�̲�������ΪGL_REPEAT�������ֵ������ȫ�������ظ�������fragPog��normal������
		//���Ǿ����㹻������������һ��TBN���󣬽����������߿ռ�任���۲�ռ䡣
		/*
		vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
		vec3 bitangent = cross(normal, tangent);
		mat3 TBN = mat3(tangent, bitangent, normal);
		*/

		//ͨ��ʹ�ø���ķ-ʩ���ع���(Gramm-Schmidt Process)��
		//���Ǵ�����һ��������(Orthogonal Basis)��ÿһ������������������randomVec��ֵ��΢��б��
		//ע����Ϊ����ʹ����һ�������������������������
		//��� TBN ������Ҫ�뼸���������ȫ���룬Ҳ�Ͳ���Ҫÿ�����������������λ����������

		//�����������Ǳ���ÿ���ں������������������߿ռ�ת������ͼ�ռ䣬������ӵ���ǰƬ��λ�ã�
		//Ȼ��Ƚ�Ƭ��λ�õ���Ⱥ���ͼ�ռ�λ�û������д洢��������ȡ�
		//������һ������������
		/*
		float occlusion = 0.0;
		for(int i = 0; i < kernelSize; ++i)
		{
			// ��ȡ����λ��
			vec3 sample = TBN * samples[i]; // ����->�۲�ռ�
			sample = fragPos + sample * radius; 

			[...]
		}
		*/
		//�����kernelSize��radius������������������Ч����
		//���Ƿֱ𱣳����ǵ�Ĭ��ֵΪ64��1.0��
		//����ÿһ�ε���,�������Ƚ����Ե�����ת������ͼ�ռ䡣
		//Ȼ����ͼ�ռ��ں�ƫ��������ӵ���ͼ�ռ�Ƭ��λ�ã�
		//���ƫ���������԰뾶�������ӣ�����٣�SSAO ����Ч�����뾶��

		//������������Ҫ������ת������Ļ�ռ䣬
		//����������ֱ�ӽ���λ����Ⱦ����Ļ��һ������������λ��/���ֵ���в�����
		//����ʸ����ǰλ����ͼ�ռ䣬���ǽ�����ʹ��uniformͶӰ������ת�����ü��ռ䡣
		/*
		vec4 offset = vec4(sample, 1.0);
		offset = projection * offset; // �۲�->�ü��ռ�
		offset.xyz /= offset.w; // ͸�ӻ���
		offset.xyz = offset.xyz * 0.5 + 0.5; // �任��0.0 - 1.0��ֵ��
		*/
		//�ڱ������任���ü��ռ�֮��������xyz��������w��������͸�ӻ��֡�
		//������õı�׼���豸����֮��任��[0.0, 1.0]��Χ�Ա�����ʹ������ȥȡ���������
		// 
		// float sampleDepth = -texture(gPositionDepth, offset.xy).w;

		//����ʹ��offset������x��y�������������������,�Ӷ���ȡ�ӹ۲����ӽǿ���������λ�õ����ֵ(��һ�������ڱεĿɼ�Ƭ��)��
		//���ǽ�������������ĵ�ǰ���ֵ�Ƿ���ڴ洢�����ֵ������ǵģ�����ӵ����յĹ��������ϡ�
		// 
		// occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0);
		// 
		//��ע�⣬������ԭʼƬ�ε����ֵ������������Ϊ 0.025���������һ��Сƫ�á�
		//ƫ�ò������Ǳ�Ҫ�ģ��������������Ӿ��ϵ��� SSAO Ч����
		//������򳡾������Զ����ܳ��ֵ��ЧӦ��

		//���˲�û����ȫ��������Ϊ��Ȼ����һ��С������Ҫ���ǡ�
		//�����һ�����������Ե��Ƭ��ʱ�������ῼ�ǲ��Ա���֮�µı�������ֵ��
		//��Щֵ����(����ȷ��)Ӱ���ڱ����ӡ�
		//���ǿ���ͨ������һ����Χ���Ӷ����������⣬������ͼ��ʾ(John Chapman�ķ���)��
		// https://learnopengl-cn.github.io/img/05/09/ssao_range_check.png

		//��������һ����Χ���ԴӶ���֤����ֻ���������ֵ��ȡ���뾶��ʱӰ���ڱ����ӡ�
		//���������һ�л��ɣ�
		/*
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;    
		*/

		//��������ʹ����GLSL��smoothstep������
		//�ú����ĵ����������ڵ�һ���͵ڶ��������ķ�Χ֮��ƽ����ֵ��
		//���С�ڻ���ڵ�һ���������򷵻� 0.0��
		//������ڻ���ڵڶ����������򷵻� 1.0��
		//�����Ȳ�ֵ�����ڰ뾶֮�䣬��ֵ��ͨ������������ 0.0 �� 1.0 ֮��ƽ����ֵ��
		// ��������ʹ����GLSL��smoothstep���������ǳ��⻬���ڵ�һ�͵ڶ���������Χ�ڲ�ֵ�˵����������������Ȳ��������ȡֵ��radius֮�䣬���ǵ�ֵ����⻬�ظ�������������߲�ֵ��0.0��1.0֮�䣺

		//�������ʹ��һ�������ֵ��radius֮���ͻȻ�Ƴ��ڱι��׵�Ӳ���޷�Χ���(Hard Cut-off Range Check)��
		//���ǽ����ڷ�Χ���Ӧ�õĵط�����һ�����Ե�(���ѿ���)��Ե��
		
		//���һ����������Ҫ���ڱι��׸��ݺ��ĵĴ�С��׼��������������
		//ע��������1.0��ȥ���ڱ����ӣ��Ա�ֱ��ʹ���ڱ�����ȥ���Ż������շ�����
		/*
		occlusion = 1.0 - (occlusion / kernelSize);
		FragColor = occlusion;
		*/

		//�������ͼչʾ��������ϲ��������װģ�����ڴ���ĳ����������ڱ���ɫ�����������µ�����
		// https://learnopengl-cn.github.io/img/05/09/ssao_without_blur.png
		
		//�ɼ��������ڱβ����˷ǳ�ǿ�ҵ���ȸС�
		//����ͨ�������ڱ��������Ǿ��Ѿ��������ؿ���ģ��һ�������ڵذ��϶����Ǹ��ڿ��С�

		//����������Ȼ������������Ϊ���������ظ�ͼ�������ɼ���
		//Ϊ�˴���ƽ���Ļ������ڱ�Ч����������Ҫ�Ի������ڱ��������ģ������
	}

	//�����ڱ�ģ��
	void how()
	{
		//��SSAO�׶κ͹��ս׶�֮�䣬������Ҫ����ģ��SSAO����Ĵ���
		//���������ִ�����һ��֡�������������ģ�������
		GLuint ssaoBlurFBO, ssaoColorBufferBlur;
		glGenFramebuffers(1, &ssaoBlurFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glGenTextures(1, &ssaoColorBufferBlur);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

		//����ƽ�̵����������������һ�µ�����ԣ�
		//���ǿ���ʹ����һ����������һ���򵥵�ģ����ɫ����
		/*
		#version 330 core
		in vec2 TexCoords;
		out float fragColor;

		uniform sampler2D ssaoInput;

		void main() 
		{
			vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
			float result = 0.0;
			for (int x = -2; x < 2; ++x) 
			{
				for (int y = -2; y < 2; ++y) 
				{
					vec2 offset = vec2(float(x), float(y)) * texelSize;
					result += texture(ssaoInput, TexCoords + offset).r;
				}
			}
			fragColor = result / (4.0 * 4.0);
		}
		*/

		//�������Ǳ�������Χ-2.0��2.0֮���SSAO����Ԫ(Texel)��
		//�� SSAO ������в���������������������ĳߴ���ͬ��
		//����ʹ�� textureSize ��ÿ����������ƫ�Ƶ���ɫԪ�ľ�ȷ�ߴ磬�����ظ�������ߴ�� vec2��
		//���ǽ��õ��Ľ����ƽ��ֵ���õ�һ���򵥵���Ч��ģ��Ч����
		// https://learnopengl-cn.github.io/img/05/09/ssao.png

		//���������Ǿ͵õ���һ������ÿ��Ƭ�λ����ڱ����ݵ����������ڹ��մ�����ʹ���ˡ�
	}

	//Ӧ�û����ڱ�
	void how()
	{
		//Ӧ���ڱ����ӵ����շ����м���򵥣�
		//����ֻ�轫ÿ��Ƭ�εĻ����ڱ����ӳ��������Ļ����������ɡ�
		//�������ʹ���ϸ��̳��е�Blinn-Phong�ӳٹ�����ɫ��������һ���޸ģ�
		//���ܵõ������Ƭ����ɫ����
		/*
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;

		uniform sampler2D gPositionDepth;
		uniform sampler2D gNormal;
		uniform sampler2D gAlbedo;
		uniform sampler2D ssao;

		struct Light 
		{
			vec3 Position;
			vec3 Color;

			float Linear;
			float Quadratic;
			float Radius;
		};
		uniform Light light;

		void main()
		{             
			// ��G��������ȡ����
			vec3 FragPos = texture(gPositionDepth, TexCoords).rgb;
			vec3 Normal = texture(gNormal, TexCoords).rgb;
			vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
			float AmbientOcclusion = texture(ssao, TexCoords).r;

			// Blinn-Phong (�۲�ռ���)
			vec3 ambient = vec3(0.3 * AmbientOcclusion); // �������Ǽ����ڱ�����
			vec3 lighting  = ambient; 
			vec3 viewDir  = normalize(-FragPos); // Viewpos Ϊ (0.0.0)���ڹ۲�ռ���
			// ������
			vec3 lightDir = normalize(light.Position - FragPos);
			vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;
			// ����
			vec3 halfwayDir = normalize(lightDir + viewDir);  
			float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
			vec3 specular = light.Color * spec;
			// ˥��
			float dist = length(light.Position - FragPos);
			float attenuation = 1.0 / (1.0 + light.Linear * dist + light.Quadratic * dist * dist);
			diffuse  *= attenuation;
			specular *= attenuation;
			lighting += diffuse + specular;

			FragColor = vec4(lighting, 1.0);
		}
		*/

		//������ͼ�ռ�ĸı�֮�⣬���������ı���ǳ����Ļ����������� AmbientOcclusion��
		//ͨ���ڳ����м���һ������ɫ�ĵ��Դ�����ǽ���õ�������������
		// https://learnopengl-cn.github.io/img/05/09/ssao_final.png

		//��Ļ�ռ价���ڱ���һ���ɸ߶��Զ����Ч��������Ч���ܴ�̶������������Ǹ��ݳ������͵���������
		//û���ʺ�ÿ�ֳ���������������ϡ�
		//��Щ����ֻ��ʹ�ý�С�İ뾶������Щ��������Ҫ�ϴ�İ뾶�ͽ϶�����������ܿ��������档
		//��ǰ��ʾ����64�����������ڱȽ϶���ˣ�����Ե�����С�ĺ��Ĵ�С�Ӷ���ø��õĽ����

		//һЩ����Ե���(����˵ͨ��uniform)�Ĳ��������Ĵ�С���뾶��/���������ĵĴ�С��
		//��Ҳ�����������յ��ڱ�ֵ��һ���û�������ݴӶ���������ǿ�ȣ�
		/*
		occlusion = 1.0 - (occlusion / kernelSize);       
		FragColor = pow(occlusion, power);
		*/

		//�����Բ�ͬ�ĳ����Ͳ�ͬ�Ĳ���������� SSAO �Ŀɶ����ԡ�

		//���� SSAO ��һ�ֲ�̫���Ե�΢��Ч����
		//������Ϊ�ʵ������ĳ������������ʵ�У�
		//�����������߰��в��ɻ�ȱ�ļ�����
	}
}