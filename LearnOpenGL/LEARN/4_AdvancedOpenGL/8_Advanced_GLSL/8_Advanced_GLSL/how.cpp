//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<stb_image.h>

Shader shaderA("", "");
Shader shaderRed("", "");
Shader shaderGreen("", "");
Shader shaderBlue("", "");
Shader shaderYellow("", "");
float width, height;
Camera camera;
unsigned int cubeVAO;

//�߼�GLSL
namespace how
{
	//��һС�ڲ���������չʾ�ǳ��Ƚ��ǳ���������ԣ�Ҳ����Գ������Ӿ���������������ߡ�
	//���ǣ���һ�ڻ�������漰GLSL��һЩ��Ȥ�ĵط��Լ�һЩ�ܰ��ļ��ɣ����ǿ����ڽ���������㡣
	//����˵�����Ǿ��������ʹ��OpenGL��GLSL��������ʱ��һЩ���Ҫ֪���Ķ�������һЩ����������������ɵ����ԡ�

	//���ǽ�������һЩ��Ȥ���ڽ�����(Built-in Variable)��
	//������ɫ�������������·�ʽ�Լ�һ������Uniform�������(Uniform Buffer Object)�����ù��ߡ�

	//GLSL���ڽ�����
	/*
	��ɫ��������򻯵ģ������Ҫ��ǰ��ɫ������ط������ݵĻ������Ǳ���Ҫ�����ݴ�������
	�����Ѿ�ѧ��ʹ�ö������ԡ�uniform�Ͳ������������һ�����ˡ�
	Ȼ��������֮�⣬GLSL�����������⼸����gl_Ϊǰ׺�ı�����
	�������ṩ�����Ǹ���ķ�ʽ����ȡ/д�����ݡ�
	�����Ѿ���ǰ��̳��нӴ������е������ˣ�
	������ɫ�����������gl_Position����Ƭ����ɫ����gl_FragCoord��

	���ǽ������ۼ�����Ȥ��GLSL�ڽ���������������������������ܹ����������㡣
	ע�⣬���ǽ���������GLSL�д��ڵ������ڽ��������������֪�����е��ڽ������Ļ�����鿴OpenGL��wiki��
	*/

	//������ɫ������--------------------------------------------------------------
	/*
	//gl_Position
	�����Ѿ�����gl_Position�ˣ����Ƕ�����ɫ���Ĳü��ռ����λ��������
	�����������Ļ����ʾ�κζ������ڶ�����ɫ��������gl_Position�Ǳ���Ĳ��衣���Ѿ�������ȫ�������ˡ�

	//gl_PointSize
	�����ܹ�ѡ�õ�����һ��ͼԪ��GL_POINTS�����ʹ�����Ļ���ÿһ�����㶼��һ��ͼԪ�����ᱻ��ȾΪһ���㡣
	���ǿ���ͨ��OpenGL��glPointSize������������Ⱦ�����ĵ�Ĵ�С��������Ҳ�����ڶ�����ɫ�����޸����ֵ��

	GLSL������һ������gl_PointSize�������������һ��float�����������ʹ���������õ�Ŀ�ߣ����أ���
	�ڶ�����ɫ�����޸ĵ�Ĵ�С�Ļ�������ܶ�ÿ���������ò�ͬ��ֵ�ˡ�

	�ڶ�����ɫ�����޸ĵ��С�Ĺ���Ĭ���ǽ��õģ�
	�������Ҫ�������Ļ�������Ҫ����OpenGL��GL_PROGRAM_POINT_SIZE��
	glEnable(GL_PROGRAM_POINT_SIZE);

	һ���򵥵����Ӿ��ǽ���Ĵ�С����Ϊ�ü��ռ�λ�õ�zֵ��Ҳ���Ƕ����۲��ߵľ��롣
	��Ĵ�С�����Ź۲��߾ඥ������Զ������
	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);
		gl_PointSize = gl_Position.z;
	}
	������ǣ�������Զ����Щ���ʱ�����ǻ��ø���
	������뵽����ÿ������ʹ�ò�ͬ�ĵ��С��������������֮��ļ����к�����˼��

	//gl_VertexID
	gl_Position��gl_PointSize���������������Ϊ���ǵ�ֵ����Ϊ������ɫ�����������ȡ�ġ�
	���ǿ��Զ����ǽ���д�룬���ı�����������ɫ����Ϊ�����ṩ��һ����Ȥ�����������
	����ֻ�ܶ������ж�ȡ��������gl_VertexID��

	���ͱ���gl_VertexID���������ڻ��ƶ���ĵ�ǰID��
	����ʹ��glDrawElements������������Ⱦ��ʱ�����������洢���ڻ��ƶ���ĵ�ǰ������
	����ʹ��glDrawArrays����ʹ���������л��Ƶ�ʱ����������ᴢ�����Ⱦ���ÿ�ʼ���Ѵ�����������

	��Ȼ������û��ʲô�������;����֪�������ܹ����������Ϣ���Ǻõġ�
	*/

	//Ƭ����ɫ������--------------------------------------------------------------
	/*
	��Ƭ����ɫ���У�����Ҳ�ܷ��ʵ�һЩ��Ȥ�ı�����
	GLSL�ṩ������������Ȥ�����������gl_FragCoord��gl_FrontFacing��

	//gl_FragCoord
	��������Ȳ��Ե�ʱ�������Ѿ�����gl_FragCoord�ܶ���ˣ�
	��Ϊgl_FragCoord��z�������ڶ�ӦƬ�ε����ֵ��Ȼ��������Ҳ��ʹ������x��y������ʵ��һЩ��Ȥ��Ч����

	gl_FragCoord��x��y������Ƭ�εĴ��ڿռ�(Window-space)���꣬��ԭ��Ϊ���ڵ����½ǡ�
	�����Ѿ�ʹ��glViewport�趨��һ��800x600�Ĵ����ˣ�����Ƭ�δ��ڿռ������x��������0��800֮�䣬y������0��600֮�䡣

	ͨ������Ƭ����ɫ�������ǿ��Ը���Ƭ�εĴ������꣬�������ͬ����ɫ��
	gl_FragCoord��һ�������ô������ڶԱȲ�ͬƬ�μ�����Ӿ����Ч����
	���ڼ�����ʾ�п��Ծ�������������˵�������ܹ�����Ļ�ֳ������֣��ڴ��ڵ������Ⱦһ��������ڴ��ڵ��Ҳ���Ⱦ��һ�������
	�����������Ƭ����ɫ������ݴ������������ͬ����ɫ��
	void main()
	{
		if(gl_FragCoord.x < 400)
			FragColor = vec4(1.0, 0.0, 0.0, 1.0);
		else
			FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
	��Ϊ���ڵĿ����800����һ�����ص�x����С��400ʱ����һ���ڴ��ڵ���࣬�������Ǹ���һ����ͬ����ɫ��

	�������ڻ�����������ȫ��ͬ��Ƭ����ɫ�����������������ʾ�ڴ��ڵ����ࡣ
	������˵������Խ������ڲ��Բ�ͬ�Ĺ��ռ��ɡ�

	//gl_FrontFacing
	Ƭ����ɫ������һ��������˼�����������gl_FrontFacing��
	�����޳��̳��У������ᵽOpenGL�ܹ����ݶ���Ļ���˳��������һ�����������Ǳ����档
	������ǲ�������GL_FACE_CULL����ʹ�����޳�����ôgl_FrontFacing����������ǵ�ǰƬ���������������һ���ֻ��Ǳ������һ���֡�
	������˵�������ܹ���������������ͬ����ɫ��

	gl_FrontFacing������һ��bool�������ǰƬ�����������һ������ô����true���������false��
	����˵�����ǿ��������Ӵ���һ�������壬���ڲ����ⲿʹ�ò�ͬ������
	#version 330 core
	out vec4 FragColor;

	in vec2 TexCoords;

	uniform sampler2D frontTexture;
	uniform sampler2D backTexture;

	void main()
	{
		if(gl_FrontFacing)
			FragColor = texture(frontTexture, TexCoords);
		else
			FragColor = texture(backTexture, TexCoords);
	}
	����������������濴�����ܿ���ʹ�õ��ǲ�ͬ������
	ע�⣬����㿪�������޳�����Ϳ����������ڲ������ˣ�����������ʹ��gl_FrontFacing��û�������ˡ�

	//gl_FragDepth
	�������gl_FragCoord�������Ƕ�ȡ��ǰƬ�εĴ��ڿռ����꣬����ȡ�������ֵ����������һ��ֻ��(Read-only)������
	���ǲ����޸�Ƭ�εĴ��ڿռ����꣬��ʵ�����޸�Ƭ�ε����ֵ���ǿ��ܵġ�
	GLSL�ṩ������һ������gl_FragDepth��������������ǿ���ʹ����������ɫ��������Ƭ�ε����ֵ��

	Ҫ���������ֵ������ֱ��д��һ��0.0��1.0֮���floatֵ����������Ϳ����ˣ�
	gl_FragDepth = 0.0; // ���Ƭ�����ڵ����ֵΪ 0.0

	�����ɫ��û��д��ֵ��gl_FragDepth�������Զ�ȡ��gl_FragCoord.z��ֵ��

	Ȼ�����������Լ��������ֵ��һ���ܴ��ȱ�㣬
	ֻҪ������Ƭ����ɫ���ж�gl_FragDepth����д�룬OpenGL�ͻᣨ����Ȳ���С�������۵��������������е���ǰ��Ȳ���(Early Depth Testing)��
	�������õ�ԭ���ǣ�OpenGL�޷���Ƭ����ɫ������֮ǰ��֪Ƭ�ν�ӵ�е����ֵ����ΪƬ����ɫ�����ܻ���ȫ�޸�������ֵ��

	��д��gl_FragDepthʱ�������Ҫ���ǵ���������������Ӱ�졣
	Ȼ������OpenGL 4.2�������Կ��Զ����߽���һ���ĵ��ͣ���Ƭ����ɫ���Ķ���ʹ���������(Depth Condition)��������gl_FragDepth������
	layout (depth_<condition>) out float gl_FragDepth;
	condition����Ϊ�����ֵ��
		����	     ����
		any	         Ĭ��ֵ����ǰ��Ȳ����ǽ��õģ������ʧ�ܶ�����
		greater	     ��ֻ�������ֵ��gl_FragCoord.z����
		less	     ��ֻ�������ֵ��gl_FragCoord.z��С
		unchanged	 �����Ҫд��gl_FragDepth���㽫ֻ��д��gl_FragCoord.z��ֵ

	ͨ���������������Ϊgreater����less��OpenGL���ܼ�����ֻ��д��ȵ�ǰƬ�����ֵ������߸�С��ֵ�ˡ�
	�����ӵĻ��������ֵ��Ƭ�ε����ֵҪС��ʱ��OpenGL�����ܹ�������ǰ��Ȳ��Եġ�

	������������У����Ƕ�Ƭ�ε����ֵ�����˵���������ȻҲ������һЩ��ǰ��Ȳ��ԣ�
	#version 420 core // ע��GLSL�İ汾��
	out vec4 FragColor;
	layout (depth_greater) out float gl_FragDepth;

	void main()
	{
		FragColor = vec4(1.0);
		gl_FragDepth = gl_FragCoord.z + 0.1;
	}
	ע���������ֻ��OpenGL 4.2�汾�����ϲ��ṩ��
	*/

	//�ӿڿ�----------------------------------------------------------------------
	/*
	��ĿǰΪֹ��ÿ������ϣ���Ӷ�����ɫ����Ƭ����ɫ����������ʱ�����Ƕ������˼�����Ӧ������/���������
	������һ��һ����������ɫ���䷢��������򵥵ķ�ʽ�ˣ����������ø���ʱ����ϣ�����͵Ŀ��ܾͲ�ֻ�Ǽ��������ˣ��������ܰ�������ͽṹ�塣

	Ϊ�˰������ǹ�����Щ������GLSLΪ�����ṩ��һ�������ӿڿ�(Interface Block)�Ķ��������������������Щ������
	�ӿڿ��������struct�������е����񣬲�ͬ���ǣ����ڸ�������һ�����뻹�������(Block)��ʹ��in��out�ؼ���������ġ�
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoords;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	out VS_OUT
	{
		vec2 TexCoords;
	} vs_out;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);
		vs_out.TexCoords = aTexCoords;
	}
	�������������һ������vs_out�Ľӿڿ飬�����������ϣ�����͵���һ����ɫ���е��������������
	��ֻ��һ���ܼ򵥵����ӣ������������һ�£����ܹ������������ɫ��������������
	������ϣ������ɫ���������������Ϊ����ʱ����Ҳ��ǳ����ã����ǽ�����һ�����ۼ�����ɫ��(Geometry Shader)ʱ������

	֮�����ǻ���Ҫ����һ����ɫ������Ƭ����ɫ�����ж���һ������ӿڿ顣
	����(Block Name)Ӧ���Ǻ���ɫ����һ���ģ�VS_OUT������ʵ����(Instance Name)��������ɫ�����õ���vs_out������������ģ�
	��Ҫ����ʹ�����Ե����ƣ������ʵ���ϰ�����������Ľӿڿ�����Ϊvs_out��
	#version 330 core
	out vec4 FragColor;

	in VS_OUT
	{
		vec2 TexCoords;
	} fs_in;

	uniform sampler2D texture;

	void main()
	{
		FragColor = texture(texture, fs_in.TexCoords);
	}
	ֻҪ�����ӿڿ������һ�������Ƕ�Ӧ��������������ƥ��������
	���ǰ��������������һ���������ԣ����ڼ�����ɫ�����������ض���ɫ���׶εĳ����»�����á�
	*/

	//Uniform�������-------------------------------------------------------------
	/*
	�����Ѿ�ʹ��OpenGL�ܳ�ʱ���ˣ�ѧ����һЩ�ܿ�ļ��ɣ���Ҳ������һЩ���鷳�ĵط���
	����˵����ʹ�ö���һ������ɫ��ʱ�����ܴ󲿷ֵ�uniform����������ͬ�ģ����ǻ�����Ҫ���ϵ��������ǣ�
	����ΪʲôҪ��ô�鷳���ظ����������أ�

	OpenGLΪ�����ṩ��һ������Uniform�������(Uniform Buffer Object)�Ĺ��ߣ�
	���������Ƕ���һϵ���ڶ����ɫ������ͬ��ȫ��Uniform������
	��ʹ��Uniform��������ʱ������ֻ��Ҫ������ص�uniformһ�Ρ�
	��Ȼ����������Ҫ�ֶ�����ÿ����ɫ���в�ͬ��uniform�����Ҵ���������Uniform����������һ�㷱����

	��ΪUniform�����������һ�����壬���ǿ���ʹ��glGenBuffers����������
	�����󶨵�GL_UNIFORM_BUFFER����Ŀ�꣬����������ص�uniform���ݴ��뻺�塣
	��Uniform��������д�����������һЩ����ģ����ǽ�����֮����������

	���ȣ����ǽ�ʹ��һ���򵥵Ķ�����ɫ������projection��view����洢����ν��Uniform��(Uniform Block)�У�
	#version 330 core
	layout (location = 0) in vec3 aPos;

	layout (std140) uniform Matrices
	{
		mat4 projection;
		mat4 view;
	};

	uniform mat4 model;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}

	�����Ǵ�����������У����Ƕ�����ÿ����Ⱦ�����У���ÿ����ɫ������projection��view Uniform����
	��������Uniform��������һ���ǳ����������ӣ���Ϊ��������ֻ��Ҫ�洢��Щ����һ�ξͿ����ˡ�

	�������������һ������Matrices��Uniform�飬������������4x4����
	Uniform���еı�������ֱ�ӷ��ʣ�����Ҫ�ӿ�����Ϊǰ׺��
	��������������OpenGL�����н���Щ����ֵ���뻺���У�ÿ�����������Uniform�����ɫ�����ܹ�������Щ����

	�����ڿ��ܻ�����layout (std140)��������ʲô��˼��������˼��˵����ǰ�����Uniform�����������ʹ��һ���ض����ڴ沼�֡�
	������������Uniform�鲼��(Uniform Block Layout)��
	*/

	//Uniform�鲼��---------------------------------------------------------------
	/*
	Uniform��������Ǵ�����һ����������еģ���ʵ����ֻ��һ��Ԥ���ڴ档
	��Ϊ����ڴ沢���ᱣ�������屣�����ʲô���͵����ݣ�
	���ǻ���Ҫ����OpenGL�ڴ����һ���ֶ�Ӧ����ɫ���е���һ��uniform������

	������ɫ���������µ����Uniform�飺
	layout (std140) uniform ExampleBlock
	{
		float value;
		vec3  vector;
		mat4  matrix;
		float values[3];
		bool  boolean;
		int   integer;
	};

	������Ҫ֪������ÿ�������Ĵ�С���ֽڣ��ͣ��ӿ���ʼλ�õģ�ƫ���������������ܹ���˳�����ǷŽ������С�
	ÿ��Ԫ�صĴ�С������OpenGL��������������ģ�����ֱ�Ӷ�ӦC++�������ͣ�
	���������;����Ǵ��float���顣OpenGLû������������Щ������ļ��(Spacing)��
	������Ӳ���ܹ�������Ϊ���ʵ�λ�÷��ñ���������˵��һЩӲ�����ܻὫһ��vec3������float���ϡ�
	�������е�Ӳ�����������������ܻ��ڸ������float֮ǰ���Ƚ�vec3���(Pad)Ϊһ��4��float�����顣
	������Ա���ܰ������ǻ����������鷳��

	Ĭ������£�GLSL��ʹ��һ����������(Shared)���ֵ�Uniform�ڴ沼�֣�
	��������Ϊһ��Ӳ��������ƫ�����������ڶ���������ǹ���һ�µġ�
	ʹ�ù�����ʱ��GLSL�ǿ���Ϊ���Ż�����uniform������λ�ý��б䶯�ģ�ֻҪ������˳�򱣳ֲ��䡣
	��Ϊ�����޷�֪��ÿ��uniform������ƫ����������Ҳ�Ͳ�֪�����׼ȷ��������ǵ�Uniform�����ˡ�
	�����ܹ�ʹ������glGetUniformIndices�����ĺ�������ѯ�����Ϣ�����ⳬ�����ڵķ�Χ�ˡ�

	��Ȼ�����ָ������Ǻܶ��ʡ�ռ���Ż�������������Ҫ��ѯÿ��uniform������ƫ�������������ǳ���Ĺ�������
	ͨ���������ǣ���ʹ�ù����֣�����ʹ��std140���֡�
	std140����������ÿ��������ƫ����������һϵ�й����������ģ�����ʽ��������ÿ���������͵��ڴ沼�֡�
	����������ʽ�ἰ�ģ����ǿ����ֶ������ÿ��������ƫ������

	ÿ����������һ����׼������(Base Alignment)��������һ��������Uniform������ռ�ݵĿռ䣨���������(Padding)����
	�����׼��������ʹ��std140���ֵĹ����������ġ�
	����������ÿ�������������ټ������Ķ���ƫ����(Aligned Offset)������һ�������ӿ���ʼλ�õ��ֽ�ƫ������
	һ�������Ķ����ֽ�ƫ����������ڻ�׼�������ı�����

	���ֹ����ԭ�Ŀ�����OpenGL��Uniform����淶�����ҵ���https://registry.khronos.org/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt
	�����ǽ����������г�����Ĺ���
	GLSL�е�ÿ������������˵int��float��bool����������Ϊ4�ֽ�����ÿ4���ֽڽ�����һ��N����ʾ��

	����	             ���ֹ���
	����������int��bool	 ÿ�������Ļ�׼������ΪN��
	����	             2N����4N������ζ��vec3�Ļ�׼������Ϊ4N��
	����������������	 ÿ��Ԫ�صĻ�׼��������vec4����ͬ��
	����	             ����Ϊ�����������飬ÿ�������Ļ�׼��������vec4����ͬ��
	�ṹ��	             ��������Ԫ�ظ��ݹ�������Ĵ�С��������䵽vec4��С�ı�����

	��OpenGL������Ĺ淶һ����ʹ�����Ӿ��ܸ����׵���⡣
	���ǻ�ʹ��֮ǰ������Ǹ�����ExampleBlock��Uniform�飬��ʹ��std140���ּ����ÿ����Ա�Ķ���ƫ������
	layout (std140) uniform ExampleBlock
	{
						 // ��׼������      // ����ƫ����(��׼���������ۻ�)
		float value;     // 4               // 0
		vec3 vector;     // 16              // 16  (������16�ı��������� 4->16)
		mat4 matrix;     // 16              // 32  (�� 0)
						 // 16              // 48  (�� 1)
						 // 16              // 64  (�� 2)
						 // 16              // 80  (�� 3)
		float values[3]; // 16              // 96  (values[0])
						 // 16              // 112 (values[1])
						 // 16              // 128 (values[2])
		bool boolean;    // 4               // 144
		int integer;     // 4               // 148
	};

	��Ϊ��ϰ������ȥ�Լ�����һ��ƫ���������ͱ����жԱȡ�
	ʹ�ü�����ƫ����ֵ������std140���ֵĹ������Ǿ���ʹ������glBufferSubData�ĺ������������ݰ���ƫ���������������ˡ�
	��Ȼstd140���ֲ������Ч�Ĳ��֣�������֤���ڴ沼����ÿ�����������Uniform��ĳ�������һ�µġ�

	ͨ����Uniform�鶨��֮ǰ���layout (std140)��䣬���Ǹ���OpenGL���Uniform��ʹ�õ���std140���֡�
	����֮�⻹����ѡ���������֣������Ƕ���Ҫ��������仺��֮ǰ�Ȳ�ѯÿ��ƫ������
	�����Ѿ�����shared�����ˣ�ʣ�µ�һ��������packed��
	��ʹ�ý���(Packed)����ʱ���ǲ��ܱ�֤���������ÿ�������б��ֲ���ģ����ǹ�����
	��Ϊ�����������ȥ��uniform������Uniform�����Ż���������ÿ����ɫ���ж������ǲ�ͬ�ġ�
	*/

	//ʹ��Uniform����-------------------------------------------------------------
	void how()
	{

		//�����Ѿ��������������ɫ���ж���Uniform�飬���趨���ǵ��ڴ沼���ˣ������ǻ�û�����۸����ʹ�����ǡ�

		//���ȣ�������Ҫ����glGenBuffers������һ��Uniform�������
		//һ����������һ���������������Ҫ�����󶨵�GL_UNIFORM_BUFFERĿ�꣬
		//������glBufferData�������㹻���ڴ档
		unsigned int uboExampleBlock;
		glGenBuffers(1, &uboExampleBlock);
		glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
		glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // ����152�ֽڵ��ڴ�
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//���ڣ�ÿ��������Ҫ�Ի�����»��߲������ݣ����Ƕ���󶨵�uboExampleBlock����ʹ��glBufferSubData�����������ڴ档
		//����ֻ��Ҫ�������Uniform����һ�Σ�����ʹ������������ɫ���Ͷ�ʹ�õ��Ǹ��º�������ˡ�
		//���ǣ���β�����OpenGL֪���ĸ�Uniform�����Ӧ�����ĸ�Uniform���أ�

		//��OpenGL�������У�������һЩ�󶨵�(Binding Point)�����ǿ��Խ�һ��Uniform��������������
		//�ڴ���Uniform����֮�����ǽ����󶨵�����һ���󶨵��ϣ�������ɫ���е�Uniform��󶨵���ͬ�İ󶨵㣬���������ӵ�һ��
		//��������ͼʾչʾ�������https://learnopengl-cn.github.io/img/04/08/advanced_glsl_binding_points.png

		//����Կ��������ǿ��԰󶨶��Uniform���嵽��ͬ�İ󶨵��ϡ�
		//��Ϊ��ɫ��A����ɫ��B����һ�����ӵ��󶨵�0��Uniform�飬���ǵ�Uniform�齫�Ṳ����ͬ��uniform���ݣ�uboMatrices��
		//ǰ��������������ɫ������������ͬ��Matrices Uniform�顣

		//Ϊ�˽�Uniform��󶨵�һ���ض��İ󶨵��У�������Ҫ����glUniformBlockBinding������
		//���ĵ�һ��������һ���������֮����һ��Uniform�����������ӵ��İ󶨵㡣
		//Uniform������(Uniform Block Index)����ɫ�����Ѷ���Uniform���λ��ֵ������
		//�����ͨ������glGetUniformBlockIndex����ȡ��������һ����������Uniform������ơ�
		//���ǿ��������·�ʽ��ͼʾ�е�Lights Uniform�����ӵ��󶨵�2��
		unsigned int lights_index = glGetUniformBlockIndex(shaderA.ID, "Lights");
		glUniformBlockBinding(shaderA.ID, lights_index, 2);
		//ע��������Ҫ��ÿ����ɫ���ظ���һ���衣

		//��OpenGL 4.2�汾����Ҳ�������һ�����ֱ�ʶ����
		//��ʽ�ؽ�Uniform��İ󶨵㴢������ɫ���У������Ͳ����ٵ���glGetUniformBlockIndex��glUniformBlockBinding�ˡ�
		//����Ĵ�����ʽ��������Lights Uniform��İ󶨵㡣
		//layout(std140, binding = 2) uniform Lights { ... };

		//�����������ǻ���Ҫ��Uniform���������ͬ�İ󶨵��ϣ�
		//�����ʹ��glBindBufferBase��glBindBufferRange����ɡ�
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock);
		//��
		glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);

		//glBindbufferBase��Ҫһ��Ŀ�꣬һ���󶨵�������һ��Uniform���������Ϊ���Ĳ�����
		//���������uboExampleBlock���ӵ��󶨵�2�ϣ��Դˣ��󶨵�����˶��������ˡ�
		//��Ҳ����ʹ��glBindBufferRange����������Ҫһ�����ӵ�ƫ�����ʹ�С������
		//����������԰�Uniform������ض�һ���ֵ��󶨵��С�
		//ͨ��ʹ��glBindBufferRange������������ö����ͬ��Uniform��󶨵�ͬһ��Uniform��������ϡ�

		//���ڣ����еĶ�������������ˣ����ǿ��Կ�ʼ��Uniform��������������ˡ�
		//ֻҪ������Ҫ���Ϳ���ʹ��glBufferSubData��������һ���ֽ�����������е����ݣ����߸��»����һ���֡�
		//Ҫ�����uniform����boolean�����ǿ��������·�ʽ����Uniform�������
		glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
		int b = true; // GLSL�е�bool��4�ֽڵģ��������ǽ�����Ϊһ��integer
		glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//ͬ���Ĳ���Ҳ��Ӧ�õ�Uniform����������uniform�����ϣ�����Ҫʹ�ò�ͬ�ķ�Χ������
	}

	//һ���򵥵�����-----------------------------------------------------------
	void how()
	{
		//���ԣ�������չʾһ������ʹ��Uniform�����������ӡ�
		//������ǻ�ͷ����֮ǰ���еĴ������ӣ����ǲ��ϵ���ʹ��3������ͶӰ���۲��ģ�;���
		//�����е���Щ�����У�ֻ��ģ�;����Ƶ���䶯��
		//��������ж����ɫ��ʹ������ͬһ�������ôʹ��Uniform���������ܻ���á�

		//���ǻὫͶӰ����ͼ����洢��һ������Matrices��Uniform���С�
		//���ǲ��Ὣģ�;�����������Ϊģ�;����ڲ�ͬ����ɫ���л᲻�ϸı䣬����ʹ��Uniform������󲢲������ʲô�ô���
		/*
		#version 330 core
		layout (location = 0) in vec3 aPos;

		layout (std140) uniform Matrices
		{
			mat4 projection;
			mat4 view;
		};
		uniform mat4 model;

		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
		*/
		//����ûʲô�ر�ģ�������������ʹ�õ���һ��std140���ֵ�Uniform�顣
		//���ǽ������ӳ����У���ʾ4�������壬ÿ�������嶼��ʹ�ò�ͬ����ɫ��������Ⱦ�ġ�
		//��4����ɫ������ʹ����ͬ�Ķ�����ɫ������ʹ�õ��ǲ�ͬ��Ƭ����ɫ����ÿ����ɫ���������ͬ����ɫ��

		//���ȣ����ǽ�������ɫ����Uniform������Ϊ�󶨵�0��
		//ע��������Ҫ��ÿ����ɫ��������һ�顣
		unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
		unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
		unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
		unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");
		glUniformBlockBinding(shaderRed.ID, uniformBlockIndexRed, 0);
		glUniformBlockBinding(shaderGreen.ID, uniformBlockIndexGreen, 0);
		glUniformBlockBinding(shaderBlue.ID, uniformBlockIndexBlue, 0);
		glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);

		//�����������Ǵ���Uniform���������������󶨵��󶨵�0��
		unsigned int uboMatrices;
		glGenBuffers(1, &uboMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
		//��������Ϊ����������㹻���ڴ棬������glm::mat4��С��������
		//GLM�������͵Ĵ�Сֱ�Ӷ�Ӧ��GLSL�е�mat4��
		//�����������ǽ������е��ض���Χ�����������������壩���ӵ��󶨵�0��

		//ʣ��ľ��������������ˡ�������ǽ�ͶӰ�������Ұ(Field of View)ֵ���ֲ��䣨�����������û�������ˣ���
		//����ֻ��Ҫ�����ڳ����ж���һ�Ρ�����Ҳ��ζ������ֻ��Ҫ�������뵽������һ�Ρ�
		//��Ϊ�����Ѿ�Ϊ�������������㹻���ڴ棬���ǿ���ʹ��glBufferSubData�ڽ�����Ⱦѭ��֮ǰ�洢ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//�������ǽ�ͶӰ���󴢴���Uniform�����ǰ�벿�֡�
		//��ÿ����Ⱦ�����л�������֮ǰ�����ǻὫ�۲������µ�����ĺ�벿�֣�
		glm::mat4 view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//Uniform�������Ĳ��־ͽ����ˡ�ÿ��������Matrices���Uniform��Ķ�����ɫ���������������uboMatrices�е����ݡ�
		//���ԣ������������Ҫ��4����ͬ����ɫ������4�������壬���ǵ�ͶӰ�͹۲���󶼻���һ���ġ�
		glBindVertexArray(cubeVAO);
		shaderRed.use();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));  // �ƶ������Ͻ�
		shaderRed.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// ... ������ɫ������
		// ... ������ɫ������
		// ... ���ƻ�ɫ������ 

		//Ψһ��Ҫ���õ�uniformֻʣmodel uniform�ˡ�
		//���������ĳ�����ʹ��Uniform����������������ÿ����ɫ���ж�ʣ��һЩuniform���á�

		//��Ϊ�޸���ģ�;���ÿ�������嶼�ƶ����˴��ڵ�һ�ߣ�
		//��������ʹ���˲�ͬ��Ƭ����ɫ�������ǵ���ɫҲ��ͬ��
		//��ֻ��һ���ܼ򵥵��龰�����ǿ��ܻ���Ҫʹ��Uniform�������
		//���κδ��͵���Ⱦ���򶼿���ͬʱ�������ϰٸ���ɫ��������ʱ��Uniform�����������ƾͻ�ܴ�����ֳ����ˡ�
	}
	/*
	Uniform���������������uniform�кܶ�ô���
	��һ��һ�����úܶ�uniform���һ��һ�����ö��uniformҪ��ܶࡣ
	�ڶ��������ڶ����ɫ�����޸�ͬ����uniform����Uniform�������޸�һ�λ������һЩ��
	���һ���ô����ܲ����������֣����ʹ��Uniform�������Ļ������������ɫ����ʹ�ø����uniform��
	OpenGL���������ܹ������uniform�����������ͨ��GL_MAX_VERTEX_UNIFORM_COMPONENTS����ѯ��
	��ʹ��Uniform�������ʱ��������������ߡ�
	���ԣ�����ﵽ��uniform���������ʱ������������������(Skeletal Animation)��ʱ�򣩣������ǿ���ѡ��ʹ��Uniform�������
	*/


}