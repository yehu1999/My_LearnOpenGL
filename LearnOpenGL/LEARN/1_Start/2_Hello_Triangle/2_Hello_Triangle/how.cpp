#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
//�����ص㣺
//VAO,VBO,EBO
//������Ⱦ
//ͼԪ���ƻ�������




//������ɫ����Դ����
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Ƭ����ɫ����Դ����
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

/*ͼ����Ⱦ����
����--(������ɫ��)--<��׼���豸����>--(ͼԪװ��)-->
ͼԪ--(������ɫ��)--<��Ļ�ռ�����>--(��դ��)-->
Ƭ��--(Ƭ����ɫ��)--(��������)

OpenGLû��Ĭ�ϵĶ�����ɫ����Ƭ����ɫ������Ҫ�û��Լ��ṩ
*/

int how()
{
	//��������
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	/*���
	*
	����OpenGL����3D�ռ��й����ģ���������Ⱦ����һ��2D�����Σ�
	���ǽ��������z��������Ϊ0.0�������ӵĻ�������ÿһ������(Depth)����һ���ģ�
	�Ӷ�ʹ������ȥ����2D�ġ�

	ͨ����ȿ�������Ϊz���꣬������һ�������ڿռ��к���ľ��룬
	�������Զ�Ϳ��ܱ���������ڵ�����Ϳ��������ˣ�
	���ᱻ�������Խ�ʡ��Դ��
	*/

	/*��׼���豸����(Normalized Device Coordinates, NDC)
	*
	OpenGL���Ǽ򵥵ذ����е�3D����任Ϊ��Ļ�ϵ�2D���أ�
	OpenGL����3D������3���ᣨx��y��z����-1.0��1.0�ķ�Χ��ʱ�Ŵ�������
	�����������Χ�ڵ����������׼���豸����(Normalized Device Coordinates)��
	�˷�Χ�ڵ�����������ʾ����Ļ�ϣ��������Χ����������򲻻���ʾ����

	һ����Ķ��������Ѿ��ڶ�����ɫ���д����������Ǿ�Ӧ���Ǳ�׼���豸�����ˣ�
	��׼���豸������һ��x��y��zֵ��-1.0��1.0��һС�οռ䡣
	�κ����ڷ�Χ������궼�ᱻ����/�ü���������ʾ�������Ļ�ϡ�

	��ͨ������Ļ���겻ͬ��y��������Ϊ���ϣ�
	(0, 0)���������ͼ������ģ����������Ͻǡ�

	�ڽ��������ӿڱ任����׼���豸���꽫��ת��Ϊ��Ļ�ռ�����(Screen-space Coordinates)��
	֮����Ļ�ռ������ֱ��任ΪƬ�����뵽Ƭ����ɫ����
	*/

	//�������㻺�����VBO
	unsigned int VBO;     //����ID
	glGenBuffers(1, &VBO);//����VBO�������
	/*
	����ͨ�����㻺�����(Vertex Buffer Objects, VBO)��������ڴ棬
	������GPU�ڴ棨ͨ������Ϊ�Դ棩�д���������㡣
	ʹ����Щ�������ĺô������ǿ���һ���Եķ���һ�������ݵ��Կ��ϣ�������ÿ�����㷢��һ�Ρ�
	��CPU�����ݷ��͵��Կ���Խ���������ֻҪ�������Ƕ�Ҫ���Ծ���һ���Է��;����ܶ�����ݡ�
	�����ݷ������Կ����ڴ��к󣬶�����ɫ���������������ʶ��㣬���Ǹ��ǳ���Ĺ��̡�
	*/

	//��VBO�������󶨵�GL_ARRAY_BUFFER(VBO�Ļ�������)Ŀ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//���ֻ��Ҫ�ٴε��øú�������������(GL_ARRAY_BUFFER, 0)
	//�ڰ󶨻�������(Ŀ��)֮��֮�����ʹ�øû�������(Ŀ��)�����û����ø�VBO����

	//���û����ݸ��Ƶ������ڴ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*glBufferData����
	*
	glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ�����
	��һ��������Ŀ�껺������ͣ����㻺�����ǰ�󶨵�GL_ARRAY_BUFFERĿ���ϡ�
	�ڶ�������ָ���������ݵĴ�С(���ֽ�Ϊ��λ)����һ���򵥵�sizeof������������ݴ�С���С�
	����������������ϣ�����͵�ʵ�����ݡ�
	���ĸ�����ָ��������ϣ���Կ���ι������������ݡ�
	����������ʽ��

		GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
		GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
		GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣

	�����ε�λ�����ݲ���ı䣬ÿ����Ⱦ����ʱ������ԭ������������ʹ�����������GL_STATIC_DRAW��
	���������˵һ�������е����ݽ�Ƶ�����ı䣬��ôʹ�õ����;���GL_DYNAMIC_DRAW��GL_STREAM_DRAW��
	��������ȷ���Կ������ݷ����ܹ�����д����ڴ沿�֡�
	*/

	//���������Ѿ��Ѷ������ݴ������Կ����ڴ��У���VBO������㻺����������
	//�������ǻᴴ��һ��������ɫ����Ƭ����ɫ��������������Щ���ݡ��������ǿ�ʼ���ִ������ǰɡ�

	//������ɫ��
	//��ɫ������GLSL(OpenGL Shading Language)
	/*
	������һ�ηǳ�������GLSL������ɫ����Դ���룺
	#version 330 core
	layout (location = 0) in vec3 aPos;

	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}

	GLSL����������C���ԡ�ÿ����ɫ������ʼ��һ���汾������
	OpenGL 3.3�Լ��͸��߰汾�У�GLSL�汾�ź�OpenGL�İ汾��ƥ���
	������˵GLSL 420�汾��Ӧ��OpenGL 4.2����
	����ͬ����ȷ��ʾ���ǻ�ʹ�ú���ģʽ��

	��һ����ʹ��in�ؼ��ִ�������������ڶ�����ɫ�����������е����붥������(Input Vertex Attribute)��
	��������ֻ����λ��(Position)���ݣ���������ֻ��Ҫһ���������ԡ�

	GLSL��һ��������������vec��������1��4��float�������������������Դ����ĺ�׺���ֿ�������
	��GLSL��һ�����������4��������ÿ������ֵ�������ռ��е�һ�����꣬
	���ǿ���ͨ��vec.x��vec.y��vec.z��vec.w����ȡ��
	����ÿ�����㶼��һ��3D���꣬���Ǿʹ���һ��vec3�������aPos��

	����ͬ��Ҳͨ��layout (location = 0)�趨�����������λ��ֵ(Location)

	Ϊ�����ö�����ɫ������������Ǳ����λ�����ݸ�ֵ��Ԥ�����gl_Position����������Ļ����vec4���͵ġ�
	��main������������ǽ�gl_Position���õ�ֵ���Ϊ�ö�����ɫ���������

	��ǰ���������ɫ���������������뵽����򵥵Ķ�����ɫ���ˣ�
	��Ϊ���Ƕ���������ʲô��û�д����Ͱ���������ɫ��������ˡ�
	����ʵ�ĳ�������������ͨ�������Ǳ�׼���豸���꣬
	�����������ȱ����Ȱ�����ת����OpenGL�Ŀ��������ڣ����������û�����֡�
	*/

	//������ɫ��
	//������ʱ��������ɫ����Դ����Ӳ�����ڴ����ļ�������C����ַ�����
	//Ϊ���ܹ���OpenGLʹ���������Ǳ���������ʱ��̬��������Դ���롣
	//��������Ҫ�����Ǵ���һ����ɫ������ע�⻹����ID�����õġ�
	//�������Ǵ������������ɫ��Ϊunsigned int��Ȼ����glCreateShader���������ɫ����
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//���ǰ���Ҫ��������ɫ�������Բ�����ʽ�ṩ��glCreateShader��
	//�����������ڴ���һ��������ɫ�������ݵĲ�����GL_VERTEX_SHADER��
	//��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ���������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����Դ��
	//glShaderSource������Ҫ�������ɫ��������Ϊ��һ��������
	//�ڶ�����ָ���˴��ݵ�Դ���ַ�������������ֻ��һ����
	//�����������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL��
	glCompileShader(vertexShader);//����

	//���������
	int  success;//��ʾ�Ƿ�ɹ�����
	char infoLog[512];//���������Ϣ������еĻ���������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//�������ʧ�ܣ����ȡʧ����Ϣ����ӡ
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//����Ƭ����ɫ��
	/*
	//Ƭ����ɫ���������Ǽ�������������ɫ�����
	//Ϊ����������򵥣����ǵ�Ƭ����ɫ������һֱ����ٻ�ɫ��
	�ڼ����ͼ������ɫ����ʾΪ��4��Ԫ�ص����飺
	��ɫ����ɫ����ɫ��alpha(͸����)������ͨ����дΪRGBA��
	����OpenGL��GLSL�ж���һ����ɫ��ʱ�����ǰ���ɫÿ��������ǿ��������0.0��1.0֮�䡣
	����˵�������ú�Ϊ1.0f����Ϊ1.0f�����ǻ�õ�������ɫ�Ļ��ɫ������ɫ��
	��������ɫ�����Ĳ�ͬ����������ɳ���1600���ֲ�ͬ����ɫ��

	������һ�ηǳ�������GLSLƬ����ɫ����Դ���룺
	#version 330 core
	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}

	Ƭ����ɫ��ֻ��Ҫһ��������������������һ��4����������
	����ʾ�������յ������ɫ������Ӧ���Լ�������������
	���������������ʹ��out�ؼ��֣�������������ΪFragColor��
	���棬���ǽ�һ��AlphaֵΪ1.0(1.0������ȫ��͸��)���ٻ�ɫ��vec4��ֵ����ɫ�����
	*/
	//����Ƭ����ɫ���Ĺ����붥����ɫ�����ƣ�ֻ��������ʹ��GL_FRAGMENT_SHADER������Ϊ��ɫ�����ͣ�
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//����Դ��
	glCompileShader(fragmentShader);//����


	//������ɫ�����ڶ������ˣ�ʣ�µ������ǰ�������ɫ���������ӵ�һ��������Ⱦ����ɫ������(Shader Program)�С�
	/*��ɫ���������(Shader Program Object)
	* 
	��ɫ���������(Shader Program Object)�Ƕ����ɫ���ϲ�֮������������ɵİ汾��
	���Ҫʹ�øղű������ɫ�����Ǳ������������(Link)Ϊһ����ɫ���������
	Ȼ������Ⱦ�����ʱ�򼤻������ɫ������
	�Ѽ�����ɫ���������ɫ���������Ƿ�����Ⱦ���õ�ʱ��ʹ�á�
	
	��������ɫ����һ�������ʱ�������ÿ����ɫ����������ӵ��¸���ɫ�������롣
	����������벻ƥ���ʱ�����õ�һ�����Ӵ���
	*/
	//������ɫ������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//glCreateProgram��������һ�����򣬲������´�����������ID���á�
	//����������Ҫ��֮ǰ�������ɫ�����ӵ���������ϣ�Ȼ����glLinkProgram�������ǣ�
	glAttachShader(shaderProgram, vertexShader);//������ɫ������
	glAttachShader(shaderProgram, fragmentShader);//Ƭ����ɫ������
	glLinkProgram(shaderProgram);//������ɫ��

	//���������ɫ���������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//������ɫ����
	glUseProgram(shaderProgram);
	//��glUseProgram��������֮��
	//ÿ����ɫ�����ú���Ⱦ���ö���ʹ�������������ˡ�

	//ɾ����ɫ������
	//�ڰ���ɫ���������ӵ���������Ժ󣬼ǵ�ɾ����ɫ���������ǲ�����Ҫ������(sinimasai)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//���ö�������ָ��
	//���ڣ������Ѿ������붥�����ݷ��͸���GPU��
	//��ָʾ��GPU����ڶ����Ƭ����ɫ���д�������
	//����û������OpenGL����֪��������ν����ڴ��еĶ������ݣ�
	//�Լ�������ν������������ӵ�������ɫ���������ϡ�
	//���Ǳ�������Ⱦǰָ��OpenGL����ν��Ͷ������ݡ�
	/*
	���ǵĶ��㻺�����ݻᱻ����Ϊ���������ӣ�
	| vector1 | vector2 | vector3 | ...
	| x  y  z | x  y  z | x  y  z | ...
	0         12        24        36     BYTE
	*λ�����ݱ�����Ϊ32λ��4�ֽڣ�����ֵ��
	*ÿ��λ�ð���3��������ֵ��
	*����3��ֵ֮��û�п�϶��������ֵ�����⼸��ֵ�������н�������(Tightly Packed)��
	*�����е�һ��ֵ�ڻ��忪ʼ��λ�á�
	
	������Щ��Ϣ���ǾͿ���ʹ��glVertexAttribPointer����
	����OpenGL����ν����������ݣ�Ӧ�õ�������������ϣ��ˣ�
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glVertexAttribPointer�����Ĳ����ǳ��࣬�����һ���һ�������ǣ�
	/*
	1.��һ������ָ������Ҫ���õĶ������ԡ�
	  �붥����ɫ����ʹ��layout(location = 0)������position�������Ե�λ��ֵ(Location)��Ӧ��
	  �����԰Ѷ������Ե�λ��ֵ����Ϊ0����Ϊ����ϣ�������ݴ��ݵ���һ�����������У������������Ǵ���0��

	2.�ڶ�������ָ���������ԵĴ�С��
	  ����������һ��vec3������3��ֵ��ɣ����Դ�С��3��

	3.����������ָ�����ݵ����ͣ�������GL_FLOAT(GLSL��vec*�����ɸ�����ֵ��ɵ�)��

	4.�¸��������������Ƿ�ϣ�����ݱ���׼��(Normalize)��
	  �����������ΪGL_TRUE���������ݶ��ᱻӳ�䵽0�������з�����signed������-1����1֮�䡣
	  ���ǰ�������ΪGL_FALSE��

	5.�����������������(Stride)�������������������Ķ���������֮��ļ����
	  �����¸���λ��������3��float֮�����ǰѲ�������Ϊ3 * sizeof(float)��
	  Ҫע�������������֪����������ǽ������еģ���������������֮��û�п�϶��
	  ����Ҳ��������Ϊ0����OpenGL�������岽���Ƕ��٣�ֻ�е���ֵ�ǽ�������ʱ�ſ��ã���
	  һ�������и���Ķ������ԣ����Ǿͱ����С�ĵض���ÿ����������֮��ļ���������ں���ῴ�����������
	 ����ע: �����������˼��˵���Ǵ�������Եڶ��γ��ֵĵط�����������0λ��֮���ж����ֽڣ���

	6.���һ��������������void*��������Ҫ���ǽ��������ֵ�ǿ������ת����
	  ����ʾλ�������ڻ�������ʼλ�õ�ƫ����(Offset)��
	  ����λ������������Ŀ�ͷ������������0�����ǻ��ں�����ϸ�������������
	*/
	glEnableVertexAttribArray(0);
	//���������Ѿ�������OpenGL����ν��Ͷ������ݣ���������Ӧ��ʹ��glEnableVertexAttribArray��
	//�Զ�������λ��ֵ��Ϊ���������ö�������0����������Ĭ���ǽ��õġ�
	/*
	ÿ���������Դ�һ��VBO�������ڴ��л���������ݣ�
	�������Ǵ��ĸ�VBO�������п����ж��VBO����ȡ
	����ͨ���ڵ���glVertexAttribPointerʱ�󶨵�GL_ARRAY_BUFFER��VBO�����ġ������ڵ���glVertexAttribPointer֮ǰ�󶨵�����ǰ�����VBO���󣬶�������0���ڻ����ӵ����Ķ������ݡ�
	*/

	//�����������VAO
	//VAO������VBOһ�����󶨣��κ����Ķ������Ե��ö��ᴢ�������VAO�С�
	//�����ĺô����ǣ������ö�������ָ��ʱ����ֻ��Ҫ����Щ����ִ��һ�Σ�
	//֮���ٻ��������ʱ��ֻ��Ҫ����Ӧ��VAO�����ˡ�
	//��ʹ�ã��ڲ�ͬ�������ݺ���������֮���л���÷ǳ��򵥣�
	//ֻ��Ҫ�󶨲�ͬVAO�ͺã������õ�����״̬����������VAO��
	/*
	OpenGL�ĺ���ģʽҪ������ʹ��VAO��������֪������δ������ǵĶ������롣
	������ǰ�VAOʧ�ܣ�OpenGL��ܾ������κζ�����

	һ�������������ᴢ��������Щ���ݣ�
	glEnableVertexAttribArray��glDisableVertexAttribArray�ĵ��á�
	ͨ��glVertexAttribPointer���õĶ����������á�
	ͨ��glVertexAttribPointer�����붥�����Թ����Ķ��㻺�����
	*/

	//����VAO����(�봴��VBO�ķ�ʽ������)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//��VAO
	glBindVertexArray(VAO);//���ֻ��Ҫ�ٴε��øú�������������0
	//�Ӱ�֮��������Ӧ�ð󶨺����ö�Ӧ��VBO������ָ�룬֮����VAO��֮��ʹ�á�
	//�����Ǵ������һ�������ʱ������ֻҪ�ڻ�������ǰ�򵥵ذ�VAO�󶨵�ϣ��ʹ�õ��趨�Ͼ����ˡ�

	//����֮�󣬱���Ի���ͼԪ��
	glDrawArrays(GL_TRIANGLES, 0, 3);
	/*
	glDrawArrays������һ�����������Ǵ�����Ƶ�OpenGLͼԪ�����͡�
	�ڶ�������ָ���˶����������ʼ����������������0��
	���һ������ָ�����Ǵ�����ƶ��ٸ����㣬������3
	������ֻ�����ǵ���������Ⱦһ�������Σ���ֻ��3�����㳤��
	*/
	
	//Ԫ�ػ������(Element Buffer Object��EBO)
	//Ҳ�������������IBO(Index Buffer Object��IBO)
	/*
	����Ⱦ������һ���������ǻ������һ����Ҫ���۵Ķ�����
	EBO��IBO
	
	 EBO��һ��������������һ�����㻺��������һ��VBO��
	 ���洢 OpenGL ��������Ҫ������Щ�����������
	 �Ӷ������������ơ�

	 ʲô���������ƣ�����ʲô�ã������Ի���һ������Ϊ����
	 ��openGl��ͨ��û�о��ζ��壬�����Ҫ���ǻ�����������������ɾ��Ρ�
	 ���Է��֣�����ͨ��VBO�У�����������������Ҫ6�����㣬
	 �����������������ظ��ģ�����˷�����Դ��
	 ��ʱ�����ǿ���ֻ������ε�4�����㣬��ͨ������4�������ţ�
	 ���ñ��(������)���ظ�����������Ҫ���ʹ�õĵ㡣
	*/
	float vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, -0.5f, 0.0f, // ���½�
	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};

	unsigned int indices[] = {
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±����������ϳɾ���
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
//����EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//ͨ��EBO���������ݸ��Ƶ�����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	

	//��ʹ����������ʱ����Ӧ�Ļ��ƺ���ҲҪ�ı�
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	/*
	��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ����
	�ڶ������������Ǵ�����ƶ���ĸ�����������6��Ҳ����˵����һ����Ҫ����6�����㡣
	���������������������ͣ�������GL_UNSIGNED_INT��
	���һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ�򣩣�������д0
	*/

	/*
	���ˣ����´������ˣ�
	VBO�ṩ���㻺�壬
	VAO�ṩ�������ã�
	EBO�ṩ����������
	
	���ǹ���glBindBuffer()������
	���ͬʱʹ��GL_ELEMENT_ARRAY_BUFFERΪ��Ŀ�꣬
	�������ڰ󶨽��ʱ�໥Ӱ�죬ͨ��������¼�����Ƿ��������󶨣�
	����ڰ󶨻���ʱҪ���Ǹ�������İ������
	
	�󶨵��Ⱥ�˳���ǣ�VAO VBO EBO 
	������Ⱥ�˳���ǣ�VBO EBO
	�����Ⱥ�˳���ǣ�VAO VBO EBO
	*/

	//��ᷢ���޷�ֱ���������cpp�ļ����뽫���ļ�����Ŀ���ų�
	//������Ϊ����һ�����ս�ѧ���̱�д�Ľ�ѧ����
	//��ѧ˳��û����ȫ���ճ�����Ҫ������
	//���������ܽ�������̣�������ִ�д��뽫��main.cpp��


	//
	/*
		// ..:: ��ʼ������ :: ..
		// 1. �󶨶����������
		glBindVertexArray(VAO);
		// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 4. �趨��������ָ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		[...]

		// ..:: ���ƴ��루��Ⱦѭ���У� :: ..
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	*/






	return 0;
}