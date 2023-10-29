//part2��������
//����part2ǰ�뽫part1���������ų�������part2��������
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//���Ǹ�����ɫ��Դ�룬����������ʹ���ַ�������Դ���룬
//��shader��part3�У����ǽ�ʹ�÷�װ�����ɫ��
// 
//������ɫ����Դ����
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"  // λ�ñ���������λ��ֵΪ 0 
"layout (location = 1) in vec3 aColor;\n"// ��ɫ����������λ��ֵΪ 1
"out vec3 ourColor;\n"                   // ��Ƭ����ɫ�����һ����ɫ
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"               // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
"}\0";
//Ƭ����ɫ����Դ����
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

int main()
{
	//��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//��������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//���붥����ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����Դ��
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
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//����Դ��
	glCompileShader(fragmentShader);//����
	//���������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//�������ʧ�ܣ����ȡʧ����Ϣ����ӡ
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//������ɫ������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//������ɫ��
	glAttachShader(shaderProgram, vertexShader);//������ɫ������
	glAttachShader(shaderProgram, fragmentShader);//Ƭ����ɫ������
	glLinkProgram(shaderProgram);//����
	//���������ɫ���������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��������
	float vertices[] = {
		// λ��              // ��ɫ
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
	};
	//����VAO��VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��VAO���ȣ�
	glBindVertexArray(VAO);
	//��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������ͨ��VBO����Կ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//��Ϊ�����������һ���������ԣ����Ҹ�����VBO���ڴ棬
	//���Ǿͱ����������ö�������ָ�롣	
	//���ö�������ָ��
	    //λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	    //��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//���º��VBO�ڴ��е��������ڿ�������������
	/*
	VERTEX1     VERTEX2    VERTEX3
	XYZ RGB     XYZ RGB    XYZ RGB
BYTE0      24          48         72
	����Ҳ���Կ�������ɫ��Դ���е�layout (location = 0)��ɶ��˼��
	location��ÿ������ĵ�n-1�����Ե�����
	���������һ��������vec3��XYZֵ��location=0��
	        �ڶ���������vec3��RGBֵ��location=1��
	����glVertexAttribPointer�����ĵ�һ�������Ƕ�Ӧ��

	���������������������������ԣ����ǲ��ò����¼��㲽��ֵ��
	Ϊ������ݶ�������һ������ֵ������λ���������¸�x������
	���Ǳ��������ƶ�6��float������3����λ��ֵ������3������ɫֵ��
	��ʹ���ǵĲ���ֵΪ6����float���ֽ�����=24�ֽڣ���
	ͬ����������Ǳ���ָ��һ��ƫ������
	����ÿ��������˵��λ�ö���������ǰ����������ƫ������0��
	��ɫ���Խ���λ������֮������ƫ��������3 * sizeof(float)�����ֽ����������12�ֽڡ�

	���ܽ�һ�£�
	glVertexAttribPointer
	(���������������������ݳ��ȣ��������ͣ��Ƿ��׼�������������ܲ�����ƫ����)
	*/

	//��ֵ
	/*
	���������к�����������ɫ�����������������������м佥���ͼ��
	������ɫ����������ÿ����������ֵ�Ľ������ֵ�㷨�Ѿ���games101�Ŀγ��н����ˡ�

	����openGL�н���Ƭ�β�ֵ��
	����Ⱦһ��������ʱ����դ��(Rasterization)�׶�ͨ������ɱ�ԭָ����������Ƭ�Ρ�
	��դ�����ÿ��Ƭ������������״���������λ�þ�����ЩƬ�ε�λ�á�
	
	������Щλ�ã������ֵ(Interpolate)����Ƭ����ɫ�������������
	����˵��������һ���߶Σ�����Ķ˵�����ɫ�ģ�����Ķ˵�����ɫ�ġ�
	���һ��Ƭ����ɫ�����߶ε�70%��λ�����У�
	������ɫ�������Ծͻ���һ����ɫ����ɫ�����Խ�ϣ�
	����ȷ��˵����30%�� + 70%�̡�

	Ƭ�β�ֵ�ᱻӦ�õ�Ƭ����ɫ�����������������ϡ�
	*/





	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//�Ƚ��VAO
	glBindVertexArray(0);//�ٽ��VBO


	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window);
		// ��Ⱦ
		// �����ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// �ǵü�����ɫ��
		glUseProgram(shaderProgram);
		// ����uniform��ɫ
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		// ����������
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// �������岢��ѯIO�¼�
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//�������ͷ���Դ(VAO,VBO,��ɫ������)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//��ֹ�����������ǰ����� GLFW ��Դ��
	glfwTerminate();

	return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

