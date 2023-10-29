#include<mythings/shader.h>
#include<mythings/camera.h>


//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\2_Lighting\\1\\vShader.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\2_Lighting\\1\\fShader.fs";
const char* L_vsPath = "E:\\Learn\\Deps\\GLSL\\2_Lighting\\1\\vLightShader.vs";
const char* L_fsPath = "E:\\Learn\\Deps\\GLSL\\2_Lighting\\1\\fLightShader.fs";

//摄像机
Camera camera(glm::vec3(0, 0, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//时间
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void controller(GLFWwindow* window);
void FeedBack();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	//基础设置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//窗口设置
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//追踪并隐藏光标
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//注册窗口大小调整回调函数
	glfwSetCursorPosCallback(window, mouse_callback);//注册鼠标回调函数(鼠标一移动注册的函数就会被调用)
	glfwSetScrollCallback(window, scroll_callback);//注册滚轮回调函数

	//glad加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//使用着色器函数创建着色器程序
	Shader shader(vsPath, fsPath);
	Shader LightShader(L_vsPath, L_fsPath);

	//顶点数据(立方体)
	float vertices[] =
	{ 
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	//BOX
	//创建VAO，VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//绑定VAO
	glBindVertexArray(VAO);
	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据通过VBO存进显卡
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//设置顶点属性指针
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Light
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//启用深度测试
	glEnable(GL_DEPTH_TEST);

	//设置物体颜色与光照颜色
	shader.use();
	shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);//珊瑚红
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);  //白色

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//输入检测
		processInput(window);
		//清屏
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一次的颜色和深度缓存
		
		//BOX
		//激活着色器
		shader.use();
		//设置物体颜色与光照颜色
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);//珊瑚红
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);  //白色
		//---------------------------------
		//模型矩阵
		glm::vec3 boxPos(0, 0, 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, boxPos);
		//观察矩阵
		glm::mat4 view;//看向(相机坐标，目标坐标，世界竖直向上的坐标)
		view = camera.GetViewMatrix();
		//投影矩阵
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//----------------------------------
		shader.setMat4fv("view", view, 1, GL_FALSE);
		shader.setMat4fv("projection", projection, 1, GL_FALSE);
		shader.setMat4fv("model", model, 1, GL_FALSE);
		//绘制
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Light
		//激活着色
		LightShader.use();
		//---------------------------------
		//模型矩阵
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);   //移动
		model = glm::scale(model, glm::vec3(0.2f));//缩小
		//观察矩阵（与BOX共用）
		//投影矩阵（与BOX共用）
		//----------------------------------
		LightShader.setMat4fv("view", view, 1, GL_FALSE);
		LightShader.setMat4fv("projection", projection, 1, GL_FALSE);
		LightShader.setMat4fv("model", model, 1, GL_FALSE);
		//绘制
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//双缓冲
		glfwSwapBuffers(window);
		//事件检测
		glfwPollEvents();
		//运行反馈
		FeedBack();
	}
	std::cout << "MISSION COMPLETE!" << std::endl;
	//结束，释放资源(VAO,VBO,着色器程序)
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	shader.del();
	LightShader.del();

	//终止，清除所有先前分配的 GLFW 资源。
	glfwTerminate();

	return 0;
}

void controller(GLFWwindow* window)
{
	//ESC
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//摄像机：WASD
	Camera_Movement direction;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	// 注意这里y偏移量是相反的，因为y坐标是从底部往顶部依次增大的
	// 我再解释一下为什么是相反的，记住视线方向是View_z轴负方向
	// 如果不取反，Screen_y偏移量增加，pitch增加，上抬的是原本是z轴方向的向量，其反方向才是视线，也就是视线实际在下压

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
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

void FeedBack()
{
	std::cout << "OK\b\b";
}