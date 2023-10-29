//haowanhaowanhhhhhhhhhhhh
#include<mythings/shader.h>
#include"stb_image.h"

void controller(GLFWwindow* window, Shader& shader);
void FeedBack();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\6\\vShader.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\6\\fShader.fs";

//�������ݣ�ע��:û�����÷����������̫�ã�
float ctrl_w = 0, ctrl_a = 0, ctrl_s = 0, ctrl_d = 0, ctrl_up = 0, ctrl_down = 0;
float ctrl_cw_x = 0, ctrl_ccw_x = 0, ctrl_cw_y = 0, ctrl_ccw_y = 0, ctrl_cw_z = 0, ctrl_ccw_z = 0;
float ctrl_play_add = 0.5, ctrl_play_sub = 0;
float ctrl_big = 0, ctrl_small = 0;

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
		processInput(window, shader);
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
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		//�۲����
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//ͶӰ����
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//----------------------------------
		//shader.setMat4fv("model", model, 1, GL_FALSE);
		shader.setMat4fv("view", view, 1, GL_FALSE);
		shader.setMat4fv("projection", projection, 1, GL_FALSE);
		//������Ȳ���
		glEnable(GL_DEPTH_TEST);
		//����������
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//���EBO��������ͼ
		for (unsigned int i = 1; i < 11; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i - 1]);
			float angle = 20.0f * i;
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

void controller(GLFWwindow* window, Shader& shader)
{
	//ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//Reset
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		ctrl_w = 0, ctrl_a = 0, ctrl_s = 0, ctrl_d = 0, ctrl_up = 0, ctrl_down = 0;
		ctrl_cw_x = 0, ctrl_ccw_x = 0, ctrl_cw_y = 0, ctrl_ccw_y = 0, ctrl_cw_z = 0, ctrl_ccw_z = 0;
		ctrl_play_add = 0.5, ctrl_play_sub = 0;
		ctrl_big = 0, ctrl_small = 0;
	}
	//Translate��WASD/QE��
	float eye_x = ctrl_a + ctrl_d;
	float eye_z = ctrl_w + ctrl_s;
	float eye_y = ctrl_up + ctrl_down;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ctrl_w += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ctrl_a -= 0.0001;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ctrl_s -= 0.0001;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ctrl_d += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		ctrl_up += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		ctrl_down -= 0.0001;
	//Rotate��RT��
	float rotate_z = ctrl_cw_z + ctrl_ccw_z;
	float rotate_x = ctrl_cw_x + ctrl_ccw_x;
	float rotate_y = ctrl_cw_y + ctrl_ccw_y;
	if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS))
		ctrl_cw_x += 0.1;
	if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS))
		ctrl_ccw_x -= 0.1;
	if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS))
		ctrl_cw_y += 0.1;
	if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS))
		ctrl_ccw_y -= 0.1;
	if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS))
		ctrl_cw_z += 0.1;
	if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS))
		ctrl_ccw_z -= 0.1;
	//Scale��UI��
	float scale_xyz = 1 + ctrl_big + ctrl_small;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		ctrl_big += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		ctrl_small -= 0.0001;
	//TransformOutcome
	glm::mat4 trans(1.0f);
	trans = glm::translate(trans, glm::vec3(eye_x, eye_y, eye_z));
	trans = glm::rotate(trans, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));
	trans = glm::rotate(trans, glm::radians(rotate_z), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(scale_xyz, scale_xyz, scale_xyz));
	//Play��JK��
	float play = ctrl_play_add + ctrl_play_sub;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && (1 - play) >= 0.000001)
		ctrl_play_add += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && play >= 0.0001)
		ctrl_play_sub -= 0.0001;

	shader.setFloat("play", play);
	shader.setMat4fv("transform", trans, 1, GL_FALSE);
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

