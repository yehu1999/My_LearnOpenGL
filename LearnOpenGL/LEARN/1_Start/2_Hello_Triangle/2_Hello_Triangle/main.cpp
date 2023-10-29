#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	float vertices_p[] =
	{
		-0.1f, -0.1f, 0.6f,
		0.5f, -0.1f, 0.0f,
		0.6f, 0.1f, 0.1f
	};
	//��������
	unsigned int indices[] = {
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±��������ϳɾ���

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	
	//����VAO��VBO
	unsigned int VAO,VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��VAO���ȣ�
	glBindVertexArray(VAO);
	//��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������ͨ��VBO����Կ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//������0��ʼ��ÿ�����㺬3��floatֵ
	glEnableVertexAttribArray(0);
	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//�Ƚ��VAO
	glBindVertexArray(0);//�ٽ��VBO

	//����VAOp��VBOp
	unsigned int VAOp, VBOp;
	glGenVertexArrays(1, &VAOp);
	glGenBuffers(1, &VBOp);
	//��VAOp���ȣ�
	glBindVertexArray(VAOp);
	//��VBOp����
	glBindBuffer(GL_ARRAY_BUFFER, VBOp);
	//����������ͨ��VBOp����Կ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_p), vertices_p, GL_STATIC_DRAW);
	//����EBO
	unsigned int EBOp;
	glGenBuffers(1, &EBOp);
	//��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOp);
	//ͨ��EBO���������ݸ��Ƶ�����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//������0��ʼ��ÿ�����㺬3��floatֵ
	glEnableVertexAttribArray(0);
	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//������
		processInput(window);
		//����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//����������
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOp);//Ҳ���Ի���VAOp����
		//glDrawArrays(GL_TRIANGLES, 0, 3);//����������0��ʼ��һ����3������
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�����߿�ģʽ
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//���EBO��������ͼ
		//˫����
		glfwSwapBuffers(window);
		//�¼����
		glfwPollEvents();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//�ر��߿�ģʽ
	//�������ͷ���Դ(VAO,VBO,EBO,��ɫ������)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAOp);
	glDeleteBuffers(1, &VBOp);
	glDeleteBuffers(1, &EBOp);
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

