//���б��ļ�ʱ�뽫������main�ļ��������ų�
#include<mythings/shader.h>
#include"stb_image.h"
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\7\\vShader.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\7\\fShader.fs";

//�����ȫ�ֱ���
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);   //�������������
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//������߷���
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);    //�������ָ��

float fov = 45.0f;      // �ӳ���
float pitch = 0.0f;     // ������
float yaw = -90.0f;     // ƫ����
bool firstMouse = true; // ����Ƿ�Ϊ��һ���ƶ�
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2; //��һ֡���λ��

//ʱ��
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void controller(GLFWwindow* window);
void FeedBack();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

	//��������(һ��������)
	float vertices[] =
	{   //λ��                //����
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	//10�������������λ������
	glm::vec3 cubePositions[] =
	{
	  glm::vec3(0.0f,  0.0f,  0.0f),
	  glm::vec3(2.0f,  5.0f, -15.0f),
	  glm::vec3(-1.5f, -2.2f, -2.5f),
	  glm::vec3(-3.8f, -2.0f, -12.3f),
	  glm::vec3(2.4f, -0.4f, -3.5f),
	  glm::vec3(-1.7f,  3.0f, -7.5f),
	  glm::vec3(1.3f, -2.0f, -2.5f),
	  glm::vec3(1.5f,  2.0f, -2.5f),
	  glm::vec3(1.5f,  0.2f, -1.5f),
	  glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	/*
	//��������
	unsigned int indices[] =
	{
		// ע��������0��ʼ!
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±��������ϳɾ���
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	*/
	//����VAO��VBO��EBO
	unsigned int VAO, VBO;// EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	//��VAO���ȣ�
	glBindVertexArray(VAO);
	//��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������ͨ��VBO����Կ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////��EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	////ͨ��EBO���������ݸ��Ƶ�����
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���ö�������ָ��
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//��������
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//�����������
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

		//���������������Ŀ��
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		//Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
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

	//����uniform������������Ԫ�Ķ�Ӧ��ϵ
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
		processInput(window);
		//����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����һ�ε���ɫ����Ȼ���
		//������ʵ�Ԫ������Ӧ���ʰ���ȥ
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//������ɫ��
		shader.use();
		//---------------------------------
		//ģ�;���(�ڻ�ͼforѭ����)
		//�۲����
		glm::mat4 view;//����(������꣬Ŀ�����꣬������ֱ���ϵ�����)
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//controller�ƶ�����
		//ͶӰ����
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		//----------------------------------
		shader.setMat4fv("view", view, 1, GL_FALSE);
		shader.setMat4fv("projection", projection, 1, GL_FALSE);
		//������Ȳ���
		glEnable(GL_DEPTH_TEST);
		//����������
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//���EBO��������ͼ
		for (unsigned int i = 1; i < 11; i++)//����10��ģ��
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i - 1]);
			float angle = 20.0f * i;//ģ����ת�Ƕ�
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4fv("model", model, 1, GL_FALSE);;

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//˫����
		glfwSwapBuffers(window);
		//�¼����
		glfwPollEvents();
		//���з���
		FeedBack();
	}
	std::cout << "MISSION COMPLETE!" << std::endl;
	//�������ͷ���Դ(VAO,VBO,��ɫ������)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	shader.del();

	//��ֹ�����������ǰ����� GLFW ��Դ��
	glfwTerminate();

	return 0;
}

void FeedBack()
{
	std::cout << "OK\b\b";
}

void controller(GLFWwindow* window)
{
	//ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//�������WASD
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 2.5f * deltaTime; //��֡��������ٶȽ�ϣ���֤�����豸�ƶ��ٶ���ͬ

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//�������mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//���ع��
	glfwSetCursorPosCallback(window, mouse_callback);//ע�����ص�����(���һ�ƶ�ע��ĺ����ͻᱻ����)
	//������ƶ������ص�������������������߷���

	//�������mouse_scroll
	glfwSetScrollCallback(window, scroll_callback);//ע����ֻص�����
	//�������ֹ���ʱ���������ӳ��ǣ��ﵽ����Ŀ��


}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)//��ֹ�״ν���ʱ����
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//���㵱ǰ֡����һ֡���λ�õ�ƫ������
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
	lastX = xpos;                 // ���ٽ���һ��Ϊʲô���෴�ģ���ס���߷�����View_z�Ḻ����
	lastY = ypos;                 // �����ȡ����Screen_yƫ�������ӣ�pitch���ӣ���̧����ԭ����z�᷽����������䷴����������ߣ�Ҳ��������ʵ������ѹ
	//����������
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//��ƫ�����ӵ�ȫ�ֱ���pitch��yaw�ϣ�
	yaw += xoffset;
	pitch += yoffset;
	//���Ƹ����Ǽ���
	if (pitch > 89.9f)
		pitch = 89.9f;
	if (pitch < -89.9f)
		pitch = -89.9f;

	//ͨ�������Ǻ�ƫ�����������Եõ������ķ���������
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//ͨ���ı��ӳ������������ŵ�Ч��
	//��Ұ(Field of View)��fov���������ǿ��Կ��������ж��ķ�Χ��
	//����Ұ��Сʱ������ͶӰ�����Ŀռ�ͻ��С�������Ŵ�(Zoom In)�˵ĸо�
	fov -= (float)yoffset;

	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	//controller
	controller(window);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

