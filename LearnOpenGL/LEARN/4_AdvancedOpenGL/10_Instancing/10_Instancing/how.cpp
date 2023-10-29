//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include <mythings/model.h>

int amount_of_models_to_draw, amount_of_vertices = 0;
void DoSomePreparations();
Shader shader("", "");
Shader instanceShader("", "");
unsigned int quadVAO;
Model planet, rock;


//ʵ����
namespace how
{
	//part1
	void how()
	{
		//��������һ�������˺ܶ�ģ�͵ĳ��������󲿷ֵ�ģ�Ͱ�������ͬһ�鶥�����ݣ�ֻ�������е��ǲ�ͬ������ռ�任��
		//����һ�������ݵĳ�����ÿ���ݶ���һ���������������ε�Сģ�͡�
		//����ܻ���Ҫ���ƺܶ���ݣ�������ÿ֡������ܻ���Ҫ��Ⱦ��ǧ����������ݡ�
		//��Ϊÿһ���ݽ������ɼ��������ι��ɣ���Ⱦ������˲����ɵģ�����ǧ����Ⱦ��������ȴ�Ἣ���Ӱ�����ܡ�

		//���������Ҫ��Ⱦ��������ʱ�����뿴��������������
		for (unsigned int i = 0; i < amount_of_models_to_draw; i++)
		{
			DoSomePreparations(); // ��VAO������������uniform��
			glDrawArrays(GL_TRIANGLES, 0, amount_of_vertices);
		}

		//�������������ģ�͵Ĵ���ʵ��(Instance)����ܿ�ͻ���Ϊ���Ƶ��ù�����ﵽ����ƿ����
		//����ƶ��㱾����ȣ�ʹ��glDrawArrays��glDrawElements��������GPUȥ������Ķ������ݻ����ĸ�������ܣ�
		//��ΪOpenGL�ڻ��ƶ�������֮ǰ��Ҫ���ܶ�׼���������������GPU�ô��ĸ������ȡ���ݣ�
		//����Ѱ�Ҷ������ԣ�������Щ��������Ի�����CPU��GPU����(CPU to GPU Bus)�Ͻ��еģ���
		//���ԣ�������Ⱦ����ǳ��죬����GPUȥ��Ⱦȴδ�ء�

		//��������ܹ�������һ���Է��͸�GPU��Ȼ��ʹ��һ�����ƺ�����OpenGL������Щ���ݻ��ƶ�����壬�ͻ�������ˡ������ʵ����(Instancing)��

		//ʵ����������ܹ�������ʹ��һ����Ⱦ���������ƶ�����壬����ʡÿ�λ�������ʱCPU -> GPU��ͨ�ţ�
		//��ֻ��Ҫһ�μ��ɡ������ʹ��ʵ������Ⱦ������ֻ��Ҫ��glDrawArrays��glDrawElements����Ⱦ���÷ֱ��ΪglDrawArraysInstanced��glDrawElementsInstanced�Ϳ����ˡ�
		//��Щ��Ⱦ������ʵ�����汾��Ҫһ������Ĳ���������ʵ������(Instance Count)�����ܹ�����������Ҫ��Ⱦ��ʵ��������
		//��������ֻ��Ҫ����������ݷ��͵�GPUһ�Σ�Ȼ��ʹ��һ�κ������ø���GPU��Ӧ����λ�����Щʵ����
		//GPU����ֱ����Ⱦ��Щʵ���������ò��ϵ���CPU����ͨ�š�

		//�����������û��ʲô�á���Ⱦͬһ������һǧ�ζ����ǲ�û��ʲô�ô���ÿ�����嶼����ȫ��ͬ�ģ����һ���ͬһ��λ�á�
		//����ֻ�ܿ���һ�����壡�������ԭ��GLSL�ڶ�����ɫ����Ƕ������һ���ڽ�������gl_InstanceID��

		//��ʹ��ʵ������Ⱦ����ʱ��gl_InstanceID���0��ʼ����ÿ��ʵ������Ⱦʱ����1��
		//����˵������������Ⱦ��43��ʵ������ô������ɫ��������gl_InstanceID������42��
		//��Ϊÿ��ʵ������Ψһ��ID�����ǿ��Խ���һ�����飬��ID��λ��ֵ��Ӧ��������ÿ��ʵ������������Ĳ�ͬλ�á�

		//Ϊ������һ��ʵ�������ƣ����ǽ����ڱ�׼���豸����ϵ��ʹ��һ����Ⱦ���ã�����100��2D�ı��Ρ�
		//���ǻ�����һ������100��ƫ��������uniform���飬��ƫ��ֵ�ӵ�ÿ��ʵ�������ı����ϡ�
		//���յĽ����һ������������ı�������
		//ÿ���ı�����2������������ɣ�һ����6�����㡣ÿ���������һ��2D�ı�׼���豸����λ��������һ����ɫ������
		//��������������ʹ�õĶ������ݣ�Ϊ�˴��������Ļ��ÿ�������ζ���С��
		float quadVertices[] =
		{
			// λ��          // ��ɫ
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};

		//Ƭ����ɫ����Ӷ�����ɫ��������ɫ����������������Ϊ������ɫ�������ʵ���ı��ε���ɫ��
		/*
		#version 330 core
		out vec4 FragColor;

		in vec3 fColor;

		void main()
		{
			FragColor = vec4(fColor, 1.0);
		}
		*/

		//�����ڶ�û��ʲô�����ݣ����Ӷ�����ɫ����ʼ�ͱ�ú���Ȥ�ˣ�
		/*
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec3 aColor;

		out vec3 fColor;

		uniform vec2 offsets[100];

		void main()
		{
			vec2 offset = offsets[gl_InstanceID];
			gl_Position = vec4(aPos + offset, 0.0, 1.0);
			fColor = aColor;
		}
		*/
		//�������Ƕ�����һ������offsets�����飬������100��ƫ��������
		//�ڶ�����ɫ���У����ǻ�ʹ��gl_InstanceID������offsets���飬��ȡÿ��ʵ����ƫ��������
		//�������Ҫʵ��������100���ı��Σ���ʹ�����������ɫ�����Ǿ��ܵõ�100��λ�ڲ�ͬλ�õ��ı��Ρ�

		//��ǰ��������Ҫ������Щƫ��λ�ã����ǻ��ڽ�����Ⱦѭ��֮ǰʹ��һ��Ƕ��forѭ�����㣺
		glm::vec2 translations[100];
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		}
		//������Ǵ���100��λ����������ʾ10x10�����ϵ�����λ�á�
		//��������translations����֮�⣬���ǻ���Ҫ������ת�Ƶ�������ɫ����uniform�����У�
		shader.use();
		for (unsigned int i = 0; i < 100; i++)
		{
			std::stringstream ss;
			std::string index;
			ss << i;
			index = ss.str();
			shader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
		}
		//����һ�δ����У����ǽ�forѭ���ļ�����iת��Ϊһ��string��
		//���ǿ�����������̬����λ��ֵ���ַ���������uniformλ��ֵ��������
		//�����������ǻ��offsets uniform�����е�ÿһ�����ö�Ӧ��λ��������

		//�������е�׼�������������ˣ����ǿ��Կ�ʼ��Ⱦ�ı����ˡ�
		//����ʵ������Ⱦ������ʹ��glDrawArraysInstanced��glDrawElementsInstanced��
		//��Ϊ����ʹ�õĲ����������壬���ǻ����glDrawArrays�汾�ĺ�����
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

		//glDrawArraysInstanced�Ĳ�����glDrawArrays��ȫһ�������������˸���������������Ҫ���Ƶ�ʵ��������
		//��Ϊ������Ҫ��10x10��������ʾ100���ı��Σ����ǽ�������Ϊ100.���д���֮����Ӧ���ܵõ���Ϥ��100����ʵ��ı��Ρ�

		//ʵ��������------------------------------------------------------------
		//��Ȼ֮ǰ��ʵ����Ŀǰ��������ܹ����������������������Ҫ��ȾԶ����100��ʵ����ʱ������ʵ�ǳ��ձ飩��
		//�������ջᳬ������ܹ���������ɫ����uniform���ݴ�С���ޡ�
		//����һ�����淽����ʵ��������(Instanced Array)����������Ϊһ���������ԣ��ܹ������Ǵ����������ݣ���
		//���ڶ�����ɫ����Ⱦһ���µ�ʵ��ʱ�Ż���¡�

		//ʹ�ö�������ʱ��������ɫ����ÿ�����ж�����GLSL��ȡ��һ�������ڵ�ǰ��������ԡ�
		//�������ǽ��������Զ���Ϊһ��ʵ��������ʱ��
		//������ɫ����ֻ��Ҫ��ÿ��ʵ����������ÿ�����㣬���¶������Ե������ˡ�
		//���������Ƕ��𶥵������ʹ����ͨ�Ķ������ԣ�������ʵ��������ʹ��ʵ�������顣

		//Ϊ�˸���һ��ʵ������������ӣ����ǽ�ʹ��֮ǰ�����ӣ�
		//����ƫ����uniform��������Ϊһ��ʵ�������顣������Ҫ�ڶ�����ɫ���������һ���������ԣ�
		/*
		#version 330 core
		layout (location = 0) in vec2 aPos;
		layout (location = 1) in vec3 aColor;
		layout (location = 2) in vec2 aOffset;

		out vec3 fColor;

		void main()
		{
			gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
			fColor = aColor;
		}
		*/

		//���ǲ���ʹ��gl_InstanceID�����ڲ���Ҫ����һ��uniform������ܹ�ֱ��ʹ��offset�����ˡ�

		//��Ϊʵ���������position��color����һ�������Ƕ������ԣ�
		//���ǻ���Ҫ���������ݴ��ڶ��㻺������У�����������������ָ�롣
		//�������Ƚ�����һ���ֵģ�translations����浽һ���µĻ�������У�
		unsigned int instanceVBO;
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//֮�����ǻ���Ҫ�������Ķ�������ָ�룬�����ö������ԣ�
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);
		//��δ��������˼�ĵط��������һ�У����ǵ�����glVertexAttribDivisor��
		//�������������OpenGL��ʲôʱ����¶������Ե���������һ�����ݡ�
		//���ĵ�һ����������Ҫ�Ķ������ԣ��ڶ������������Գ���(Attribute Divisor)��
		//Ĭ������£����Գ�����0������OpenGL������Ҫ�ڶ�����ɫ����ÿ�ε���ʱ���¶������ԡ�
		//��������Ϊ1ʱ�����Ǹ���OpenGL����ϣ������Ⱦһ����ʵ����ʱ����¶������ԡ�
		//������Ϊ2ʱ������ϣ��ÿ2��ʵ������һ�����ԣ��Դ����ơ�
		//���ǽ����Գ�������Ϊ1�����ڸ���OpenGL������λ��ֵ2�Ķ���������һ��ʵ�������顣

		//���֮ǰ����������ȫһ���ģ��������ʹ��ʵ��������ʵ�ֵģ����������ܹ����ݸ�������ݵ�������ɫ����ֻҪ�ڴ�����������ʵ�������ơ�

		//Ϊ�˸���Ȥһ�㣬����Ҳ����ʹ��gl_InstanceID�������ϵ���������С�ı��Σ�

		//����㻹�ǲ�ȷ��ʵ������Ⱦ����ι����ģ������뿴�����д����������������ģ�������������ҵ������Դ���롣

		//��Ȼ����Ȥ��������Щ���Ӳ�����ʵ�����ĺ����ӡ�
		//�ǵģ����ǵ�ȷ����֪��ʵ��������ô�����ģ��������ǻ�û�Ӵ����������õ�һ�㣺���ƾ޴��������������塣
		//�������ԭ�����ǽ�������һ���ֽ���̫��̽�գ���ʶʵ������Ⱦ������������
	}

	//part2
	void how()
	{
		//��������һ������������������һ��������ǣ���λ��С���Ǵ������롣
		//������С���Ǵ����ܰ�����ǧ������ҿ飬�ںܲ�����Կ���Ҳ���������������Ⱦ��
		//ʵ������Ⱦ���������������ĳ�������Ϊ���е�С���Ƕ�����ʹ��һ��ģ������ʾ��
		//ÿ��С���ǿ�����ʹ�ò�ͬ�ı任��������������ı仯��

		//Ϊ��չʾʵ������Ⱦ�����ã��������Ȼ᲻ʹ��ʵ������Ⱦ������ȾС�����������Ƿ��еĳ�����
		//��������������һ���������ģ�ͣ��Լ��ܶ໷�������ǵ�С���ǡ�

		//�ڴ��������У����ǽ�ʹ����ģ�ͼ���С���ж����ģ�ͼ�����������ģ�͡�

		//Ϊ�˵õ���Ҫ��Ч�������ǽ���Ϊÿ��С��������һ���任�����������ǵ�ģ�;���
		//�任�������Ƚ�С����λ�Ƶ�С���Ǵ��е�ĳ�������ǻ����һ��С�����ƫ��ֵ�����ƫ�����ϣ������Բ������������Ȼһ�㡣
		//������������Ӧ��һ����������ţ�������һ����ת����Ϊ�����һ���������ת��
		//���յı任���󲻽��ܽ�С���Ǳ任�����ǵ���Χ�����һ���������������Ȼ��������С���ǲ�ͬ��
		//���յĽ����һ������С���ǵ�Բ��������ÿһ��С���Ƕ����ڲ�ͬ��
		unsigned int amount = 1000;
		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[amount];
		srand(glfwGetTime()); // ��ʼ���������    
		float radius = 50.0;
		float offset = 2.5f;
		for (unsigned int i = 0; i < amount; i++)
		{
			glm::mat4 model;
			// 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
			float angle = (float)i / (float)amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. ���ţ��� 0.05 �� 0.25f ֮������
			float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. ��ӵ������������
			modelMatrices[i] = model;
		}

		//��δ��뿴���������е����ˣ�������ֻ�ǽ�С���ǵ�x��zλ�ñ任����һ���뾶Ϊradius��Բ���ϣ������ڰ뾶�Ļ�����ƫ����-offset��offset��
		//������yƫ�Ƶ�Ӱ���Сһ�㣬��С���Ǵ�����ƽһ�㡣
		//������������Ӧ�������ź���ת�任���������յı任���󴢴���modelMatrices�У��������Ĵ�С��amount��
		//�������һ������1000��ģ�;���ÿ��С����һ����

		//�ڼ��������Ǻ���ʯģ�ͣ�����������ɫ��֮����Ⱦ�Ĵ��뿴�����������ģ�
		//��ʵ������������
		shader.use();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		planet.Draw(shader);

		//��ʵ��������С����
		for (unsigned int i = 0; i < amount; i++)
		{
			shader.setMat4("model", modelMatrices[i]);
			rock.Draw(shader);
		}
		//�������Ȼ��������ǵ�ģ�ͣ�����������λ�ƺ����ţ�����Ӧ������
		//�����������ǻ���amount��������ʯģ�͡�
		//�ڻ���ÿ����ʯ֮ǰ������������Ҫ����ɫ�������ö�Ӧ��ģ�ͱ任����
		//���յĽ����һ������������̫�յĳ��������������ǵ��ǿ���������Ȼ��С���Ǵ���

		//�������ÿ֡����1001����Ⱦ���ã�����1000������ʯģ�͡���
		//�����ǿ�ʼ����������ֵ�ʱ����ܿ�ͻᷢ�ֳ��������ܹ����������ˣ�
		//֡��Ҳ�½��������������ǽ�amount����Ϊ2000��ʱ�򣬳������Ѿ������ƶ��������ѵĳ̶��ˡ�

		//ʵ��������
		//���ڣ�����������ʹ��ʵ������Ⱦ����Ⱦ��ͬ�ĳ������������ȶԶ�����ɫ������һ���޸ģ�
		/*
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 2) in vec2 aTexCoords;
		layout (location = 3) in mat4 instanceMatrix;

		out vec2 TexCoords;

		uniform mat4 projection;
		uniform mat4 view;

		void main()
		{
			gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
			TexCoords = aTexCoords;
		}
		*/

		//���ǲ���ʹ��ģ��uniform��������Ϊһ��mat4�Ķ������ԣ��������ܹ��洢һ��ʵ��������ı任����
		//Ȼ���������Ƕ������Ե����ʹ���vec4ʱ����Ҫ�����һ�������ˡ�
		//�������������������ݴ�С����һ��vec4����Ϊһ��mat4��������4��vec4��������ҪΪ�������Ԥ��4���������ԡ�
		//��Ϊ���ǽ�����λ��ֵ����Ϊ3������ÿһ�еĶ�������λ��ֵ����3��4��5��6��

		//��������������ҪΪ��4������������������ָ�룬������������Ϊʵ�������飺
		//���㻺�����
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
		for (unsigned int i = 0; i < rock.meshes.size(); i++)
		{
			unsigned int VAO = rock.meshes[i].VAO;
			glBindVertexArray(VAO);
			// ��������
			GLsizei vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
		//ע���������ǽ�Mesh��VAO��˽�б�����Ϊ�˹��б������������ܹ��������Ķ����������
		//�Ⲣ������õĽ��������ֻ��Ϊ����ϱ�С�ڵ�һ���򵥵ĸĶ���
		//����֮������Ӧ�ú�����ˡ����Ǹ�����OpenGLӦ����ν���ÿ�����嶥�����ԵĻ��壬���Ҹ�������Щ����������ʵ�������顣

		//�������������ٴ�ʹ�������VAO����һ��ʹ��glDrawElementsInstanced���л��ƣ�
		//����С����
		instanceShader.use();
		for (unsigned int i = 0; i < rock.meshes.size(); i++)
		{
			glBindVertexArray(rock.meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
			);
		}

		//������ǻ�����֮ǰ��ͬ����amount��С���ǣ�����ʹ�õ���ʵ����Ⱦ��
		//���Ӧ���Ƿǳ����Ƶģ�������㿪ʼ����amount����������ܿ���ʵ������Ⱦ��Ч���ˡ�
		//û��ʵ������Ⱦ��ʱ������ֻ��������Ⱦ1000��1500��С���ǡ�
		//��ʹ����ʵ������Ⱦ֮�����ǿ��Խ����ֵ����Ϊ100000��ÿ����ʯģ����576�����㣬
		//ÿ֡���������Ҫ����5700������㣬������ȴû���ܵ��κ�Ӱ�죡��10w���ҵ�3060laptop�Ѿ���֡�ˡ���������ʵʵ��1w����
		
		//�ο����뾶Ϊ50.0f��ƫ��������5.0f

		//���Կ������ں��ʵĻ����£�ʵ������Ⱦ�ܹ���������Կ�����Ⱦ������
		//���ǳ������ԭ��ʵ������Ⱦͨ����������Ⱦ�ݡ�ֲ�������ӣ��Լ����������ĳ�����
		//������ֻҪ�������кܶ��ظ�����״�����ܹ�ʹ��ʵ������Ⱦ��������ܡ�
	}

}