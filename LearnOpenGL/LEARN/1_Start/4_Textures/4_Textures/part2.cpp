//�������б��ļ���ȷ��how.cpp��part1.cpp�Ѿ����������ų�
//�����±���
#include<mythings/shader.h>
#include"stb_image.h" //���벻ͬ��ʽ������

void controller(GLFWwindow* window, Shader& shader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\4\\vShader_.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\4\\fShader_.fs";

//��������
float w = 0, a = 0, s = 0, d = 0;
float up = 0, down = 0;
float play_add = 0.5, play_sub = 0;

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
	Shader shader(vsPath, fsPath);

	//��������
	float vertices[] =
	{
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	//��������
	unsigned int indices[] =
	{
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±����������ϳɾ���
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	//����VAO��VBO��EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//��VAO���ȣ�
	glBindVertexArray(VAO);
	//��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������ͨ��VBO����Կ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//��EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//ͨ��EBO���������ݸ��Ƶ�����
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ö�������ָ��
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//��������
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//������������
	unsigned int texture[2];

	//ʹ��stb_image.h����ͼƬ
	int width[2], height[2], nrChannels[2];
	unsigned char* data[2];
	stbi_set_flip_vertically_on_load(true);//����y�ᷴת
	data[0] = stbi_load("E:\\Learn\\Deps\\images_in\\container.jpg", &width[0], &height[0], &nrChannels[0], 0);
	data[1] = stbi_load("E:\\Learn\\Deps\\images_in\\awesomefaceC.png", &width[1], &height[1], &nrChannels[1], 0);
	//���awesomefaceC.png��ʵ��6�εĲ�����Ц������Χ������͸�����Ǻڱ���
	//���������:PS��ͼƬ���½�alphaͨ��������͸����Ϊ100%��ѡȡ͸�����򣬵���Ϊpng����ѡ͸���Ⱥͽ�С�ļ�

	for (int i = 0; i <= 1; i++)
	{
		glGenTextures(1, &texture[i]);

		//����������������Ŀ��
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		//Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//����Ƿ�������ȡ��ͼ����Ϣ
		if (data[i])
		{
			//��������
			if (i == 0)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
			if (i == 1)//(png�ļ���͸��ֵ����ͨ��alpha channel��Ӧʹ��RGBA)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[i], height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
			//����MipMap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture[" << i << "]" << std::endl;
		//�ͷ�ͼ���ڴ�
		stbi_image_free(data[i]);
	}

	//����uniform��������������Ԫ�Ķ�Ӧ��ϵ
	shader.use(); //��Ҫ����������uniform����֮ǰ������ɫ������
	shader.setTex("texture1", 0);
	shader.setTex("texture2", 1);

	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//�Ƚ��VAO
	glBindVertexArray(0);//�ٽ��VBO

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//������
		processInput(window, shader);
		//����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//������ʵ�Ԫ������Ӧ���ʰ���ȥ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//������ɫ��
		shader.use();
		//����������
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//����EBO��������ͼ
		//˫����
		glfwSwapBuffers(window);
		//�¼����
		glfwPollEvents();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//�ر��߿�ģʽ
	//�������ͷ���Դ(VAO,VBO,��ɫ������)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shader.del();

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
	//play
	float play = play_add + play_sub;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && (1 - play) >= 0.000001)
		play_add += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && play >= 0.0001)
		play_sub -= 0.0001;

	float eye_x = a + d;
	float eye_z = w + s;
	float eye_y = up + down;

	shader.setVec3f("move", eye_x, eye_y, eye_z);
	shader.setFloat("play", play);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window, Shader& shader)
{
	//controller
	controller(window, shader);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

