//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>

GLuint tex, textures, vbo;
float* data;
void glCheckError();
void glewInit();

//����
namespace how
{
	//����
	/*
	ͼ�α�̿��Դ����ܶ����Ȥ��Ȼ�����ʲô������Ⱦ���󣬻�������������û����Ⱦ����ͬ�����Ը�����������ľ�ɥ�У�
	�������Ǵ󲿷�ʱ�䶼�������ش򽻵��������ִ����ʱ��Ѱ�Ҵ����Դͷ���ܻ�ǳ����ѡ�
	����(Debug)�������Ӿ�������������Ϥ��CPU���Բ�ͬ��
	����û��һ��������������ı��Ŀ���̨����GLSL������Ҳ�������öϵ㣬��û�з������GPU������״̬��

	����ƪ�̳��У����ǽ�����ʶ��������OpenGL����ļ��ɡ�
	OpenGL�еĵ��Բ����Ǻ��ѣ�����ס��Щ���ɶ�֮���ѧϰ���зǳ���İ�����
	*/

	//glGetError()  ���Ѽ���debug.h��
	void how()
	{
		//���㲻��ȷʹ��OpenGL��ʱ�򣨱���˵�ڰ�֮ǰ����һ�����壩��
		//�����⵽������Ļ������һ�������û������ǡ�
		//���ǿ���ʹ��һ������glGetError�ĺ�����ѯ��Щ�����ǣ�
		//����������Ǽ���������OpenGLȷʵ�����ʱ�򷵻�һ������ֵ��
		GLenum glGetError();

		//��glGetError������ʱ����Ҫô�᷵�ش�����֮һ��Ҫô�����޴���
		//glGetError�᷵�صĴ���ֵ���£�
		/*

		���	                          ����  ����
		GL_NO_ERROR	                       0    ���ϴε���glGetError����û�д���
		GL_INVALID_ENUM	                  1280	ö�ٲ������Ϸ�
		GL_INVALID_VALUE	              1281	ֵ�������Ϸ�
		GL_INVALID_OPERATION	          1282	һ��ָ���״̬��ָ��Ĳ������Ϸ�
		GL_STACK_OVERFLOW	              1283	ѹջ�������ջ����(Overflow)
		GL_STACK_UNDERFLOW	              1284	��ջ����ʱջ����͵㣨��ע����ջ����(Underflow)��
		GL_OUT_OF_MEMORY	              1285	�ڴ���ò����޷����ã��㹻�ģ��ڴ�
		GL_INVALID_FRAMEBUFFER_OPERATION  1286	��ȡ��д��һ����������֡����
		*/

		//��OpenGL�ĺ����ĵ���������ҵ������ڴ���ʱ���ɵĴ�����롣
		//����˵������㿴һ��glBindTexture�������ĵ���
		//�� Errors ����������Կ������������ɵ������û�������롣

		//��һ�������Ǳ����ص�ʱ�򣬽����ᱨ�������Ĵ����ǡ�
		//���仰˵����glGetError�����õ�ʱ������������еĴ����ǣ��ڷֲ�ʽϵͳ��ֻ�����һ�����������ע�ͣ���
		//��Ҳ����˵�������ÿһ֮֡�����glGetErrorһ�Σ�������һ������
		//���㲻��ȷ�������Ψһ�Ĵ��󣬲��Ҵ������Դ��������һ֡������ط���

		//ע�⵱OpenGL�Ƿֲ�ʽ(Distributely)���е�ʱ��
		//����X11ϵͳ�ϣ��������û����������Ȼ���Ա����ɣ�ֻҪ�������Ų�ͬ�Ĵ�����롣
		//����glGetErrorֻ������һ����������ǣ����������С�
		//������һ�㣬����ͨ���Ὠ����һ��ѭ���е���glGetError��

		glBindTexture(GL_TEXTURE_2D, tex);
		std::cout << glGetError() << std::endl; // ���� 0 (�޴���)

		glTexImage2D(GL_TEXTURE_3D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		std::cout << glGetError() << std::endl; // ���� 1280 (�Ƿ�ö��)

		glGenTextures(-5, &textures);
		std::cout << glGetError() << std::endl; // ���� 1281 (�Ƿ�ֵ)

		std::cout << glGetError() << std::endl; // ���� 0 (�޴���)

		//glGetError�ǳ�����һ��������ܹ��ǳ��򵥵ض�λ������ܵ���Դ��������֤OpenGLʹ�õ���ȷ�ԡ�
		//����˵������һ�������Ľ�����ǲ�֪��ʲô���������
		//�ǲ���֡�������ô����ǲ��������ǰ������ˣ�
		//ͨ���ڴ����и�������glGetError������Էǳ����ٵز���OpenGL����ʼ���ֵ�λ�ã�
		//��Ҳ����ζ����ε���֮ǰ�Ĵ�������������ˡ�

		//Ĭ�������glGetErrorֻ���ӡ�������֣�����㲻ȥ����Ļ���ǳ�������⡣
		//ͨ�����ǻ�дһ�����ֺ��������ش�ӡ�������ַ����Լ������⺯�����õ�λ�á�
		/*
		GLenum glCheckError_(const char *file, int line)
		{
			GLenum errorCode;
			while ((errorCode = glGetError()) != GL_NO_ERROR)
			{
				std::string error;
				switch (errorCode)
				{
					case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
					case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
					case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
					case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
					case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
					case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
					case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
				}
				std::cout << error << " | " << file << " (" << line << ")" << std::endl;
			}
			return errorCode;
		}
		#define glCheckError() glCheckError_(__FILE__, __LINE__)
		*/

		//��ֹ�㲻֪��__FILE__��__LINE__������Ԥ����ָ��(Preprocessor Directive)��ʲô��
		//���ǻ��ڱ����ʱ���滻�ɱ���ʱ��Ӧ���ļ����кš�
		//������Ǽ���ڴ�����ʹ�ô���glGetError�ĵ��ã���ͻ������Ǹ���׼ȷ��֪���ĸ�glGetError���÷����˴���
		//����ע���ǵ�glGetError��ʾ�Ĵ���ᷢ���ڸôε������ϴε���֮�䣬������̫��Ļ���Ҫ���ĵط���̫���ˣ���

		glBindBuffer(GL_VERTEX_ARRAY, vbo);
		glCheckError();
		//�����������µ������https://learnopengl-cn.github.io/img/06/01/debugging_glgeterror.png

		//glGetError�����ܰ�����ܶ࣬��Ϊ�����ص���Ϣ�ǳ��򵥣�
		//�����ɷ����������ܰ������������߿��ٶ�λ������Դ��
		//�ܶ���֮����һ���ǳ��򵥵���Ч�Ĺ��ߡ�
	}

	//�������      ���Ѽ���debug.h��
	void how()
	{
		//��Ȼû��glGetError�ձ飬��һ�������������(Debug Output)��OpenGL��չ��һ���ǳ����õĹ��ߣ�
		//����4.3�汾֮���Ϊ�˺���OpenGL��һ���֡�
		//ͨ��ʹ�õ��������չ��OpenGL�����ֱ�ӷ���һ������glGetError��Ϊ���ƵĴ���򾯸���Ϣ���û���
		//�������ṩ�˸������Ϣ��Ҳ�ܹ�������ʹ�õ�����(Debugger)��׽����Դͷ��

		//���������4.3�汾��Ϊ����OpenGL��һ���֣�
		//��Ҳ����˵���������������OpenGL 4.3�����ϰ汾�Ļ������ҵ���һ���ܡ�
		//���OpenGL������һ�汾������Կ��Բ�ѯARB_debug_output����AMD_debug_output��չ����ȡ���Ĺ��ܡ�
		//ע��OS X����֧�ֵ���������ܣ������Ͽ����ģ�����ʱ��û�в��ԡ�����Ҵ����������һ�£�

		//Ҫ�뿪ʼʹ�õ��������������Ҫ�ڳ�ʼ�������д�OpenGL����һ��������������ġ�
		//������̸�����Ĵ���ϵͳ��������ͬ����������ֻ��������GLFW�����ã�
		//��������ڽ̳����ĸ�����Դ���ҵ�����ϵͳ��������ϡ�

		//GLFW�еĵ������------------------------------------------------------------------------------------

		//��GLFW������һ����������ǳ��򵥣�����ֻ��Ҫ����һ�����ѵ�GLFW�У�
		//������������Ҫһ��������������ļ��ɡ�
		//������Ҫ�ڵ���glfwCreateWindow֮ǰ�����һ����
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		//һ��GLFW��ʼ����ɣ��������ʹ�õ�OpenGL �汾Ϊ4.3�����ϵĻ����Ǿ���һ�������������ˣ�
		//������ǵĻ�����ϵͳ��Ȼ�ܹ�����һ�����������İɡ�
		//������ǲ��еĻ����Ǳ���ʹ������OpenGL��չ��������������

		//ע�⣬�ڵ�����������ʹ��OpenGL�����Ը�����һ�㣬���Ե������Ż����߷�������֮ǰ�뽫��һGLFW���������ע�͵���

		//Ҫ��������Ƿ�ɹ��س�ʼ���˵��������ģ����ǿ��Զ�OpenGL���в�ѯ��
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			// ��ʼ��������� 
		}

		//������������ķ�ʽ�������ģ��������Ƚ�һ�������¼�����Ļص���������GLFW����Ļص������ݸ�OpenGL��
		//������ص����������ǿ������ɵش���OpenGL�������ݣ����������ǽ����һЩ���õĴ������ݵ�����̨�С�
		//�������������OpenGL�Ե���������ڴ��Ļص�������ԭ�ͣ�
		void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
			GLsizei length, const GLchar * message, const void* userParam);

		//������һ��ѵ����ݣ����ǿ��Դ���һ���ǳ����õĴ����ӡ���ߣ�
		/*
		void APIENTRY glDebugOutput(GLenum source,
									GLenum type,
									GLuint id,
									GLenum severity,
									GLsizei length,
									const GLchar *message,
									const void *userParam)
		{
			// ����һЩ����Ҫ�Ĵ���/�������
			if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

			std::cout << "---------------" << std::endl;
			std::cout << "Debug message (" << id << "): " <<  message << std::endl;

			switch (source)
			{
				case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
				case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
				case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
				case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
				case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
				case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
			} std::cout << std::endl;

			switch (type)
			{
				case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
				case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
				case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
				case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
				case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
				case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
				case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
				case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
				case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
			} std::cout << std::endl;

			switch (severity)
			{
				case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
				case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
				case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
				case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
			} std::cout << std::endl;
			std::cout << std::endl;
		}
		*/

		//�����������⵽��һ��OpenGL���������������ص����������ǽ����Դ�ӡ���ǳ����OpenGL������Ϣ��
		//ע�����Ǻ��Ե���һЩ������룬��Щ�������һ�㲻�ܸ������κ����õ���Ϣ
		//������NVidia�����е�131185���������ǻ���ɹ������ˣ���

		//�����������˻ص���������ʱ���ʼ����������ˣ�
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);  //ע��DebugOutput�ص�����
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		//��������Ǹ��� OpenGL ���õ��������
		//glEnable(GL_DEBUG_SYNCRHONOUS) ���ø��� OpenGL �ڷ�������ʱֱ�ӵ��ûص�������

		//���˵������----------------------------------------------------------------------------------------

		//����glDebugMessageControl�������Ǳ�ڵع��˳�����Ҫ�Ĵ������͡�
		//���������ǲ���������κ���Դ�����ͻ������صȼ���
		//������ǽ�ϣ����ʾOpenGL API�ĸ����صȼ�������Ϣ�����������Ϊ����������
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);

		//�������ǵ����ã�������������֧�ֵ�������Ļ���ÿ������ȷ��OpenGLָ����ӡ��һ��ѵ��������ݡ�
		// https://learnopengl-cn.github.io/img/06/01/debugging_debug_output.png

		//���ݵ��Դ���Դ--------------------------------------------------------------------------------------

		//ʹ�õ��������һ���ܰ��ļ��ɾ�������Ժ������ҳ���������׼ȷ�кŻ��ߵ��á�
		//ͨ����DebugOutput���ض��Ĵ��������ϣ������ں����Ķ�����
		//����㲻�������͵Ļ�������һ���ϵ㣬���������Ჶ׽���׳��Ĵ���
		//��������ϲ��ҵ���ջֱ���ҵ���Ϣ������Դͷ��
		// https://learnopengl-cn.github.io/img/06/01/debugging_debug_output_breakpoint.png

		//����ҪһЩ�ֶ�����������������֪������Ѱ��ʲô�����ǳ����ã��ܹ���������ٶ�λ����

		//�Զ���������------------------------------------------------------------------------------------

		//���˽������Ķ���Ϣ������Ҳ����ʹ��glDebugMessageInsert����Ϣ���͵��������ϵͳ��
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "error message here");

		//�����������������ʹ�õ�����������ĵĳ����OpenGL������п��������ǳ����á�
		//�����Ŀ������ܿ����˽����Զ���OpenGL�������κα��������Bug��

		//�ܶ���֮������������������ʹ������������ٲ�׽�����Ƿǳ����õģ���ȫֵ���㻨һ��ʱ�������ã����ܹ�ʡ����ǳ���Ŀ���ʱ�䡣
	}

	//������ɫ�����
	void how()
	{
		//����GLSL��˵�����ǲ��ܷ�������glGetError�����ĺ�����Ҳ����ͨ�������ķ�ʽ������ɫ�����롣
		//�����õ�һ������������ȫ������Ӿ�Ч����ͨ����Ҫ֪����ɫ�������Ƿ��д����ǳ����ѡ�
		//�ǵģ��������б�����󱨸��������﷨���󣬵��ǲ�׽�����������һ�����⡣

		//һ������ʹ�õļ��ɾ��ǽ���ɫ��������������صı���ֱ�ӷ��͵�Ƭ����ɫ�������ͨ�������������ǡ�
		//ͨ��ֱ�������ɫ�������������ɫͨ��������ͨ������ͨ���۲��Ӿ��������ȡ���õ���Ϣ��
		//����˵�����������Ҫ���һ��ģ�͵ķ������Ƿ���ȷ��
		//���ǿ��԰����ǣ������Ǳ任����Ҳ������û�б任���ģ��Ӷ�����ɫ�����ݵ�Ƭ����ɫ���У�
		//��Ƭ����ɫ�������ǻ����������ַ�ʽ�����������
		/*
		#version 330 core
		out vec4 FragColor;
		in vec3 Normal;
		[...]
  
		void main()
		{
			[...]
			FragColor.rgb = Normal;
			FragColor.a = 1.0f;
		}
		*/

		//ͨ�����һ��������ɫ�������������ӵ������ɫͨ���У����ǿ��Կ����������Ƿ���ʾ����ȷ��ֵ��
		//������˵����������Ӿ�Ч����ȫ�Ǻ�ɫ�ģ�����������������û����ȷ�ش�������ɫ���С�
		//�����Ƕ���ʾ������ʱ�򣬼�����ǣ���ţ���ȷ���ͻ��÷ǳ��򵥡�
		// https://learnopengl-cn.github.io/img/06/01/debugging_glsl_output.png
		//���Ӿ�Ч�����������ǿ��Կ���������Ӧ������ȷ�ģ���Ϊ����װ���Ҳ�󲿷ֶ��Ǻ�ɫ�ģ���������ߴ�ţ���ȷ�أ�ָ����x�ᣩ��
		//�������Ƶ�����װ��ǰ���󲿷ֶ�Ϊ��ɫ������z�᷽��

		//��һ�������Ժ�������չ������Ҫ���Ե��κα�����
		//һ���㿨ס�˻��߻��������ɫ�������⣬������ʾ���������/���м����������Ĳ����㷨ʲô��û���ϻ����д���
	}

	//OpenGL GLSL�ο�������
	void how()
	{
		//ÿһ�������������Լ��Ĺ�񱡣����˵NVIDIA�����������һ�㣬ͨ�������һЩ���ƻ��߹淶����ATI/AMD������ͨ�����ϸ�ִ��OpenGL�淶�����ҿ��������һ�㣩��
		//��������һ̨�����ϵ���ɫ������һ̨�����Ͽ��ܾ������������첻�����������ˡ�

		//ͨ������ľ�����������ܹ�֪����ͬGPU��Ӧ��֮���ϸ΢���
		//���������Ҫ��֤�����ɫ�����������еĻ����϶������У�
		//�����ֱ�Ӷ��Źٷ��ı�׼ʹ��OpenGL��GLSL�ο���������Reference Compiler������顣
		//����Դ�����������ν��GLSL����У����(GLSL Lang Validator)�Ŀ�ִ�а汾��https://www.khronos.org/opengles/sdk/tools/Reference-Compiler/
		//���ߴ������ҵ�������Դ�롣https://github.com/KhronosGroup/glslang

		//�������GLSL����У����������Ժܷ���ļ�������ɫ�����룬ֻ��Ҫ����ɫ���ļ���Ϊ����ĵ�һ���������ɡ�
		//ע��GLSL����У������ͨ�����й̶��ĺ�׺����������ɫ�������͵ģ�
		/*
		.vert��������ɫ��(Vertex Shader)
		.frag��Ƭ����ɫ��(Fragment Shader)
		.geom��������ɫ��(Geometry Shader)
		.tesc��ϸ�ֿ�����ɫ��(Tessellation Control Shader)
		.tese��ϸ�ּ�����ɫ��(Tessellation Evaluation Shader)
		.comp��������ɫ��(Compute Shader)
		*/

		//����GLSL�ο��������ǳ��򵥣�
		// �������� glsllangvalidator shaderFile.vert
		//ע�����û�м�⵽����Ļ���û�������
		//��һ������ȷ�Ķ�����ɫ��ʹ��GLSL�ο����������в��Ի�������½����
		// https://learnopengl-cn.github.io/img/06/01/debugging_glsl_reference_compiler.png

		//��������ʾAMD��NVidia���Լ�Intel��GLSL������֮���ϸ΢���Ҳ���ܱ�֤�����ɫ����ȫû��Bug��
		//���������ܹ��������ֱ�ӵ�GLSL�淶���м�顣
	}

	//֡�������    ���Ѽ���debug.h��
	void how()
	{
		//��ĵ��Թ�����������һ�����ɾ�����OpenGL������һ���ض�������ʾ֡��������ݡ�
		//����ܻ�Ƚ�Ƶ����ʹ��֡���壬������֡�����ħ��ͨ����Ļ����У���ʱ����Ҫ֪����ʲô�����ǳ����ѡ�
		//����ĳ�������ʾ֡�����������һ�������õļ��ɣ���������ټ�����

		//ע�⣬�������۵�֡������ʾ���ݣ���������������������ʹ�ã�������Ӧ������Ⱦ�������

		//ͨ��ʹ��һ���ǳ��򵥣�ֻ��ʾ�������ɫ����
		//���ǿ���дһ�����ֺ�����������Ļ���Ͻ���ʾ�κ�����

		/*
		// ������ɫ��
		#version 330 core
		layout (location = 0) in vec2 position;
		layout (location = 1) in vec2 texCoords;

		out vec2 TexCoords;

		void main()
		{
			gl_Position = vec4(position, 0.0f, 1.0f);
			TexCoords = texCoords;
		}

		// Ƭ����ɫ��
		#version 330 core
		out vec4 FragColor;
		in  vec2 TexCoords;

		uniform sampler2D fboAttachment;

		void main()
		{
			FragColor = texture(fboAttachment, TexCoords);
		}
		*/
		/*
		void DisplayFramebufferTexture(GLuint textureID)
		{
			if(!notInitialized)
			{
				// ����Ļ���Ͻǣ�ʹ��NDC���������ʼ����ɫ����VAO
				[...]
			}

			glActiveTexture(GL_TEXTURE0);   
			glUseProgram(shaderDisplayFBOOutput);
				glBindTexture(GL_TEXTURE_2D, textureID);
				glBindVertexArray(vaoDebugTexturedRect);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			glUseProgram(0);
		}

		int main()
		{
			[...]
			while (!glfwWindowShouldClose(window))
			{
				[...]
				DisplayFramebufferTexture(fboAttachment0);

				glfwSwapBuffers(window);
			}
		}
		*/

		//�⽫����Ļ���ϽǸ���һ��С���ڣ���������֡����������
		//��������Ҫ����ӳ���Ⱦ���ļ�����Ⱦ�׶��еķ������Ƿ���ȷ��ʹ�������ǳ����㣺
		// https://learnopengl-cn.github.io/img/06/01/debugging_fbo_output.png
		//����ͼ���������� 5/8_Deferred_Shading��ʵ��)

		//�㵱Ȼ������չ���������֧����Ⱦ���������
		//��������ܹ��ǳ����ٵ������֡�����������ų����ķ�����
	}

	//�ⲿ�������
	void what()
	{
		//���������н��ܵ��ļ��ɶ�����ʹ�õ�ʱ�������Կ���ʹ��һ���������Ĺ������������ǵ��ԡ�
		//������Ӧ��ͨ���������Լ�ע�뵽OpenGL�����У������ܹ����ظ���OpenGL���ã�����������õ����ݡ�
		//��Щ���߿����ںܶ෽��������㣺��OpenGL����ʹ�ý������ܲ��ԣ�Ѱ��ƿ������黺���ڴ棬��ʾ�����֡���帽����
		//���������д�����ģ����Ʒ���룬����Ĺ����ڿ����������Ƿǳ����õġ�

		//���������г���һЩ���еĵ��Թ��ߣ�ѡ��������һ�£������ĸ����ʺ��㡣s

		//gDebugger------------------------------------------------------------

		//gDebugger��һ���ǳ����õĿ�ƽ̨OpenGL������Թ��ߡ�
		//gDebugger���������е�OpenGL������ϣ��ṩOpenGL״̬����ϸ�ſ���
		//�������ʱ��ͣ��������鵱ǰ״̬�����������Լ�����ʹ�á�
		//���������������gDebugger��http://www.gremedy.com/

		//����gDebuggerֻ��Ҫ�򿪳��򣬴���һ�����̣�������OpenGL�����λ���ڹ���Ŀ¼���ɡ�
		// https://learnopengl-cn.github.io/img/06/01/debugging_external_gdebugger.png

		//RenderDoc------------------------------------------------------------

		//RenderDoc������һ���ܰ��ģ���ȫ��Դ�ģ��������Թ��ߡ�
		//��gDebugger���ƣ���ֻ��Ҫ���ò�׽�ĳ����Լ�����Ŀ¼�����ˡ�
		//��ĳ�����������У�������Ҫ���һ���ض���֡��ʱ��
		//��ֻ��Ҫ��RenderDoc�ڳ���ǰ״̬�²�׽һ������֡���ɡ�
		//�ڲ�׽��֡���У�����Թ۲����״̬������OpenGLָ����崢�棬�Լ�ʹ�õ�����
		// https://learnopengl-cn.github.io/img/06/01/debugging_external_renderdoc.png

		//CodeXL----------------------------------------------------------------

		//CodeXL����AMD������һ��GPU���Թ��ߣ����ж����汾Ҳ��Visual Studio����汾��
		//CodeXL���Ը���ǳ������Ϣ������ͼ�γ�������ܲ���Ҳ�ǳ����á�
		//CodeXL��NVidia��Intel���Կ���Ҳ�����У������᲻֧��OpenCL���ԡ�
		// https://learnopengl-cn.github.io/img/06/01/debugging_external_codexl.png

		//NVIDIA Nsight---------------------------------------------------------

		//NVIDIA���е�Nsight GPU���Թ��߲�����һ���������򣬶���һ��Visual Studio IDE����Eclipse IDE�Ĳ����
		//Nsight�����ͼ�ο�������˵�ǳ�����ʹ�ã���Ϊ��������GPU��������֡GPU״̬��������ʱ��ͳ�����ݡ�

		//������Visual Studio����Eclipse��ʹ��Nsight�ĵ��Ի������ܲ���ָ�����������ʱ��
		//Nsight�����ڳ������������С�
		//Nsight�ǳ�����һ�����������ĳ�������Ⱦ��һ��GUIϵͳ�������ʹ������ȡ�������ָ������õ���Ϣ��
		//����������ʱҲ��������֡������

		// https://learnopengl-cn.github.io/img/06/01/debugging_external_nsight.png

		//Nsight��һ��ǳ����õĹ��ߣ����ҿ����������������߶��и��õı��֣�
		//��������һ���ǳ���Ҫ��ȱ�㣬��ֻ����NVIDIA���Կ��Ϲ�����
		//���������ʹ��һ��NVIDIA���Կ�������ʹ��Visual Studio����Nsight�Ƿǳ�ֵ��һ�Եġ�
	}

	//������Դ
	/*
	http://retokoradi.com/2014/04/21/opengl-why-is-your-code-producing-a-black-window/
	Reto Koradi�о��������Ļû�в����κ�����Ŀ���ԭ��
	
	http://vallentinsource.com/opengl/debug-output
	Vallentin Sourceд��һ�ݷǳ���ϸ�ĵ�������̳̣��������ڶ������ϵͳ�����õ��������ĵ���ϸ��Ϣ��
	*/
}