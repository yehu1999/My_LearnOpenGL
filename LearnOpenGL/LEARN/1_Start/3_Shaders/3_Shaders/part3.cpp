#include "shader.h";

void controller(GLFWwindow* window, Shader& shader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\3\\test.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\3\\test.fs";

//��������
float w = 0, a = 0, s = 0, d = 0;
float up = 0, down = 0; 

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
	//glad����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//ʹ����ɫ������������ɫ������
	Shader Shader_0(vsPath, fsPath);

	//��������
	float vertices[] = 
	{
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

	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//�Ƚ��VAO
	glBindVertexArray(0);//�ٽ��VBO


	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ����
		processInput(window, Shader_0);
		// ��Ⱦ
		// �����ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// �ǵü�����ɫ��
		Shader_0.use();
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
	Shader_0.del();

	//��ֹ�����������ǰ����� GLFW ��Դ��
	glfwTerminate();

	return 0;
}

void controller(GLFWwindow* window, Shader& shader)
{
	//�˳�
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//ˮƽ�ƶ�
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		w += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		a -= 0.0001;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		s -= 0.0001;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		d += 0.0001;
	//��ֱ�ƶ�
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		up += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		down -= 0.0001;

	float eye_x = a + d;
	float eye_z = w + s;
	float eye_y = up + down;
	shader.setVec3f("move", eye_x, eye_y, eye_z);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Shader& shader)
{
	//controller
	controller(window, shader);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


