//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>

//���� Bloom
namespace how
{
	//����
	void how()
	{
		//�����Ĺ�Դ�����򾭳�������۲��߱���������Ϊ�����������ȷ�Χ�����޵ġ�
		//һ������������Դ�ķ�ʽ��ʹ�����ڼ������Ϸ�����â����Դ�Ĺ�â�����ܷ�ɢ��
		//�����۲��߾ͻ������Դ��������ȷ��ǿ������
		//����ע�����������������˵��Ϊ�˽�����������⣺
		//������һ���������µİ�ֽ����ֽ�ڼ���������ʾ���ǳ���ɫ����ǰ����̫��Ҳ�Ǵ���ɫ�ģ����Ի����ϰ�ֽ��̫������һ�����ˣ���̫����һ�����Σ�����̫���������ƺ��ͱȰ�ֽ�����ˣ�

		//����Ч������ʹ��һ��������Ч������ʵ�֡�
		//����ʹ�������������������Ч����
		//������һ��ʹ���˺�û��ʹ�ù��εĶԱȣ�ͼƬ������������棩��
		// https://learnopengl-cn.github.io/img/05/07/bloom_example.png

		//Bloom�������ܹ�ע�⵽һ������������������������ĸо���
		//������Լ������������еĹ���Ч�������ṩ�˼����Ч������������������һ��ֻ��һ��ı䡣

		//Bloom��HDR���ʹ��Ч���ܺá�
		//������һ�������HDR�ͷ�����һ���ģ��ܶ�����Ϊ���ּ����ǿ��Ի����ġ�
		//�������������ֲ�ͬ�ļ��������ڸ��Բ�ͬ��Ŀ���ϡ�
		//����ʹ��Ĭ�ϵ�8λ��ȷ�ȵ�֡���壬Ҳ�����ڲ�ʹ�÷���Ч����ʱ��ʹ��HDR��
		//ֻ����������HDR֮����ʵ�ַ���͸����ˡ�

		//Ϊʵ�ַ��⣬������ƽʱ������Ⱦһ�����ճ�����
		//��ȡ��������HDR��ɫ����ͽ���ʾ����������ĳ�������ͼ��
		//����ȡ������ͼ����ű�ģ���������������ӵ�ԭʼ��HDR�������档

		//������һ��һ���������������̡�
		//�����ڳ�������Ⱦһ������4����������ʽ��ͬ��ɫ�������Ĺ�Դ��
		//������ɫ�ķ����������������1.5��15.0֮�䡣
		//������ǽ�����Ⱦ��HDR��ɫ���壬�������������������ģ�
		// https://learnopengl-cn.github.io/img/05/07/bloom_scene.png

		//���ǵõ����HDR��ɫ����������ȡ���г���һ�����ȵ�fragment��
		//�������Ǿͻ���һ��ֻ��fragment������һ�����޵���ɫ����
		// https://learnopengl-cn.github.io/img/05/07/bloom_extracted.png

		//���ǽ��������һ���������޵��������ģ����
		//����Ч����ǿ�Ⱥܴ�̶������ɱ�ģ���������ķ�Χ��ǿ����������
		// https://learnopengl-cn.github.io/img/05/07/bloom_blurred.png

		//���յı�ģ�����������������������÷�������Ч���Ķ�����
		//�����ģ��������Ҫ��ӵ�ԭ����HDR��������֮�ϡ�
		//��Ϊģ����������Ӧ��ʹ�������򷢳����Σ��������������ڳ��Ϳ��϶�������չ��
		// https://learnopengl-cn.github.io/img/05/07/bloom_small.png

		//���Ȿ�����Ǹ����ӵļ����������ѻ����ȷ��Ч����
		//����Ʒ�ʺܴ�̶���ȡ�������õ�ģ�������������������͡�
		//�򵥵ظĸ�ģ���������ͻἫ��ĸı䷺��Ч����Ʒ�ʡ�

		//�����⼸�����Ƿ��������Ч�Ĺ��̣����ܽ���ʵ�ַ�������Ĳ��衣
		// https://learnopengl-cn.github.io/img/05/07/bloom_steps.png

		//����������Ҫ����һ����������ȡ������������ɫ����������������¡�
	}

	//��ȡ��ɫ
	void how()
	{
		//��һ������Ҫ����Ⱦ�����ĳ�������ȡ����ͼƬ��
		//���ǿ�����Ⱦ�������Σ�ÿ��ʹ��һ����ͬ����ɫ����Ⱦ����ͬ��֡�����У�
		//�����ǿ���ʹ��һ������MRT��Multiple Render Targets������ȾĿ�꣩��С���ɣ��������Ǿ���ָ�����������ɫ�������
		//���������ǻ��ܹ���һ��������Ⱦ��������ȡͷ����ͼƬ��
		//��������ɫ�������ǰ������ָ��һ������location��ʶ�����������Ǳ�ɿ���һ��������ɫ��д�뵽�ĸ���ɫ���壺
		/*
		layout (location = 0) out vec4 FragColor;
		layout (location = 1) out vec4 BrightColor;
		*/
		//��ֻ��������ľ��ж���ط���д��ʱ������ܹ�����
		//ʹ�ö��������ɫ������ı�Ҫ�����ǣ��ж����ɫ���帽�ӵ��˵�ǰ�󶨵�֡��������ϡ�
		//����ܴ�֡����̳���������𣬵���һ���������ӵ�֡�������ɫ������ʱ�����ǿ���ָ��һ����ɫ������
		//ֱ�����ڣ�����һֱʹ����GL_COLOR_ATTACHMENT0��
		//��ͨ��ʹ��GL_COLOR_ATTACHMENT1�����ǿ��Եõ�һ��������������ɫ�����֡�������
		/*
		// Set up floating point framebuffer to render scene to
		GLuint hdrFBO;
		glGenFramebuffers(1, &hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		GLuint colorBuffers[2];
		glGenTextures(2, colorBuffers);
		for (GLuint i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// attach texture to framebuffer
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
			);
		}
		*/

		//������Ҫ��ʽ��֪OpenGL��������ͨ��glDrawBuffers��Ⱦ�������ɫ���壬
		//����OpenGLֻ����Ⱦ��֡����ĵ�һ����ɫ���������������������ġ�
		//���ǿ���ͨ�����ݶ����ɫ������ö����������£�����������Ĳ���������Ⱦ��
		/*
		GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		*/

		//����Ⱦ�����֡�����ʱ��һ����ɫ��ʹ��һ������location���η���fragment�ͻ�д���Ӧ����ɫ���塣
		//��ܰ�����Ϊ����ʡȥ������Ϊ��ȡ��������Ķ�����Ⱦ���裬
		//��Ϊ�������ڿ���ֱ�Ӵӽ�����Ⱦ��fragment��ȡ�����ǣ�
		/*
		#version 330 core
		layout (location = 0) out vec4 FragColor;
		layout (location = 1) out vec4 BrightColor;

		[...]

		void main()
		{            
			[...] // first do normal lighting calculations and output results
			FragColor = vec4(lighting, 1.0f);
			// Check whether fragment output is higher than threshold, if so output as brightness color
			float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
			if(brightness > 1.0)
				BrightColor = vec4(FragColor.rgb, 1.0);
		}
		*/
		//��������������������գ����䴫�ݸ���һ��������ɫ�����������FragColor��
		//Ȼ������ʹ�õ�ǰ������FragColor�Ķ������������������Ƿ񳬹���һ�����ޡ�
		//����ͨ��ǡ���ؽ���תΪ�Ҷȵķ�ʽ����һ��fragment�����ȣ�
		//�����������һ�����ޣ����ǾͰ���ɫ������ڶ�����ɫ���壬���ﱣ��������������
		//��Ⱦ�����������Ҳ��һ���ġ�

		//��Ҳ˵����Ϊʲô������HDR�������ܹ����еúܺá�
		//��ΪHDR�У����ǿ��Խ���ɫֵָ������1.0���Ĭ�ϵķ�Χ�������ܹ��õ���һ��ͼ���е����ȵĸ��õĿ���Ȩ��
		//û��HDR���Ǳ��뽫��������ΪС��1.0��������Ȼ���У��������������ױ�úܶ࣬��͵��¹���Ч�����ء�

		//����������ɫ���壬���Ǿ�����һ������������ͼ���һ����ȡ����������ͼ����Щ����һ����Ⱦ��������ɡ�
		// https://learnopengl-cn.github.io/img/05/07/bloom_attachments.png

		//����һ����ȡ��������ͼ���������ھ�Ҫ�����ͼ�����ģ������
		//���ǿ���ʹ��֡����̳̺����ֵ��Ǹ��򵥵ĺ��ӹ�������
		//������������û���ʹ��һ�����߼��ĸ�Ư����ģ������������˹ģ��(Gaussian blur)��
	}

	//��˹ģ��
	void how()
	{
		//�ں���̳�������ǲ��õ�ģ����һ��ͼ����������Χ���صľ�ֵ��
		//����ȷΪ�����ṩ��һ������ʵ�ֵ�ģ��������Ч�������á�
		//��˹ģ�����ڸ�˹���ߣ���˹����ͨ��������Ϊһ���������ߣ��м��ֵ�ﵽ��󻯣����ž�������ӣ����ߵ�ֵ���ϼ��١�
		//��˹��������ѧ���в�ͬ����ʽ������ͨ������������״��https://learnopengl-cn.github.io/img/05/07/bloom_gaussian.png

		//��˹�����������м䴦��������ʹ������ֵ��ΪȨ��ʹ�ý���������ӵ����������Ȩ��
		//���磬������Ǵ�fragment��32��32���ķ���������������Ȩ�����ź�fragment�ľ������𽥼�С��
		//ͨ�����õ����ø���ʵ��ģ��Ч��������ģ��������˹ģ����

		//Ҫʵ�ָ�˹ģ������������Ҫһ����άȨ�غУ����ǿ��ԴӶ�ά��˹���߷����л�á�
		//Ȼ�������ַ����������������ܿ�����ļ�������ܡ�
		//��һ��32��32��ģ���ں�Ϊ�������Ǳ����ÿ��fragment��һ�������в���1024�Σ�

		//���˵��ǣ���˹�����и��ǳ�������ԣ����������ǰѶ�ά���̷ֽ�Ϊ������С�ķ��̣�
		//һ������ˮƽȨ�أ���һ��������ֱȨ�ء�
		//����������ˮƽȨ�������������Ͻ���ˮƽģ����Ȼ���ھ��ı�������Ͻ��д�ֱģ����
		//����������ԣ������һ���ģ����ǿ��Խ�ʡ�������ŵ����ܣ�
		//��Ϊ��������ֻ����32+32�β�����������32��32=1024���ˣ�
		//�����������˹ģ��two-pass Gaussian blur��
		// https://learnopengl-cn.github.io/img/05/07/bloom_gaussian_two_pass.png

		//��Ҳ��ζ�����������һ��ͼ�����ģ������������Ҫ���������ʹ��֡�������������¡�
		//���ǽ�ͨ��ƹ��֡������ʵ�ָ�˹ģ����
		//������˼�ǣ���һ�Զ�֡���壬
		//���ǰ���һ��֡�������ɫ�����Ž���ǰ��֡�������ɫ�����У�ʹ�ò�ͬ����ɫЧ����Ⱦָ���Ĵ�����
		//������Ͼ��ǲ��ϵ��л�֡���������ȥ���ơ�
		//�����������ڵ�һ�������жԳ����������ģ����Ȼ���ڰѸ�֡�������ɫ����Ž��ڶ���֡�������ģ����
		//���ţ��ٽ��ڶ���֡�������ɫ����Ž���һ��֡���壬ѭ��������

		//�������о�֡����֮ǰ�������۸�˹ģ����������ɫ����
		/*
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;

		uniform sampler2D image;

		uniform bool horizontal;

		uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

		void main()
		{             
			vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
			vec3 result = texture(image, TexCoords).rgb * weight[0]; // current fragment's contribution
			if(horizontal)
			{
				for(int i = 1; i < 5; ++i)
				{
					result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
					result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
				}
			}
			else
			{
				for(int i = 1; i < 5; ++i)
				{
					result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
					result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
				}
			}
			FragColor = vec4(result, 1.0);
		}
		*/
		//��������ʹ��һ���Ƚ�С�ĸ�˹Ȩ�������ӣ�ÿ������������ָ����ǰfragment��ˮƽ��ֱ�������ض�Ȩ�ء�
		//��ᷢ�����Ǹ���uniform����horizontal��ֵ��ģ���������ָ�Ϊһ��ˮƽ��һ����ֱ���֡�
		//ͨ����1.0��������Ĵ�С����textureSize�õ�һ��vec2���õ�һ���������ص�ʵ�ʴ�С���Դ���Ϊƫ�ƾ���ĸ��ݡ�

		//����Ϊͼ���ģ������������������֡���壬ÿ��ֻ��һ����ɫ��������
		/*
		GLuint pingpongFBO[2];
		GLuint pingpongBuffer[2];
		glGenFramebuffers(2, pingpongFBO);
		glGenTextures(2, pingpongBuffer);
		for (GLuint i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
			glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
			);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
			);
		}
		*/

		//�õ�һ��HDR�������������ȡ�����������������һ��֡���壬Ȼ�����ģ������10�Σ�5�δ�ֱ5��ˮƽ����
		/*
		GLboolean horizontal = true, first_iteration = true;
		GLuint amount = 10;
		shaderBlur.Use();
		for (GLuint i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]); 
			glUniform1i(glGetUniformLocation(shaderBlur.Program, "horizontal"), horizontal);
			glBindTexture(
				GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]
			); 
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		*/
		
		//ÿ��ѭ�����Ǹ������Ǵ�����Ⱦ����ˮƽ���Ǵ�ֱ����������������֮һ��������һ����Ϊ�������ģ����
		//��һ�ε�������Ϊ������ɫ���嶼�ǿյ��������������һ��ȥ����ģ������
		//�ظ��������10�Σ�����ͼ��ͽ���һ���ظ�5�εĸ�˹ģ���ˡ�
		//�������ǿ��Զ�����ͼ����������ģ������
		//��˹ģ��ѭ������Խ�࣬ģ����ǿ��Խ��

		//ͨ������ȡ�����������5��ģ�������Ǿ͵õ���һ����ȷ��ģ���ĳ�������ͼ��
		// https://learnopengl-cn.github.io/img/05/07/bloom_blurred_large.png
		
		//��������һ���ǰ�ģ�������ͼ��ͳ���ԭ����HDR������н�ϡ�
	}

	//������
	void how()
	{
		//���˳�����HDR�����ģ�������������������ֻ������ǽ����������ʵ�ַ����ƹ���Ч���ˡ�
		//���յ�������ɫ�����󲿷ֺ�HDR�̳��õĲ�ࣩҪ�����������ϣ�
		/*
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;

		uniform sampler2D scene;
		uniform sampler2D bloomBlur;
		uniform float exposure;

		void main()
		{             
			const float gamma = 2.2;
			vec3 hdrColor = texture(scene, TexCoords).rgb;      
			vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
			hdrColor += bloomColor; // additive blending
			// tone mapping
			vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
			// also gamma correct while we're at it       
			result = pow(result, vec3(1.0 / gamma));
			FragColor = vec4(result, 1.0f);
		}
		*/

		//Ҫע���������Ҫ��Ӧ��ɫ��ӳ��֮ǰ��ӷ���Ч����
		//������ӵ������ķ��⣬Ҳ�����ת��ΪLDR������Ч����Ի���á�

		//�������������Ժ󣬳������������˺��ʵĹ�����Ч��
		// https://learnopengl-cn.github.io/img/05/07/bloom.png

		//����ɫ�������忴�����·�����������ⷢ���â����ȷ��һ�����õ��Ӿ�Ч����
		//��������Ƚϼ򵥣����Է���Ч������ʮ��������Ŀ�����ڸ��õĳ����к�������֮��Ч�����о޴�Ĳ�ͬ��

		//����̳�����ֻ������һ����Լ򵥵ĸ�˹ģ��������������ÿ��������ֻ��5��������
		//ͨ�����Ÿ���İ뾶���ظ����������ģ�������в������ǾͿ�������ģ����Ч����
		//��Ϊģ���������뷺��Ч������������أ�����ģ��Ч�����ܹ���������Ч����
		//��Щ������ģ���������벻ͬ��С��ģ���ں˻���ö����˹������ѡ���Եؽ��Ȩ�ؽ������ʹ�á�
		//����Kalogirou��EpicGames�ĸ�����Դ���������ͨ��������˹ģ����������������Ч����
		// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
		// https://udn.epicgames.com/Three/Bloom.html
		// http://kalogirou.net/2006/05/20/how-to-do-good-bloom-for-hdr-rendering/

	}
}