//运行本文件时请将其他含main文件从生成排除
#include<mythings/shader.h>
#include"stb_image.h"
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\7\\vShader.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\7\\fShader.fs";

//摄像机全局变量
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);   //相机的世界坐标
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//相机视线方向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);    //相机顶部指向

float fov = 45.0f;      // 视场角
float pitch = 0.0f;     // 俯仰角
float yaw = -90.0f;     // 偏航角
bool firstMouse = true; // 鼠标是否为第一次移动
float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2; //上一帧鼠标位置

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
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glad加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//使用着色器函数创建着色器程序
	Shader shader(vsPath, fsPath);

	//顶点数据(一个立方体)
	float vertices[] =
	{   //位置                //纹理
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
	//10个立方体的世界位置数据
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
	//索引数据
	unsigned int indices[] =
	{
		// 注意索引从0开始!
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	*/
	//创建VAO，VBO，EBO
	unsigned int VAO, VBO;// EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	//绑定VAO（先）
	glBindVertexArray(VAO);
	//绑定VBO（后）
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据通过VBO存进显卡
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	////绑定EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	////通过EBO将索引数据复制到缓冲
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性指针
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//创建纹理对象
	unsigned int texture[2];

	//使用stb_image.h加载图片
	int width[2], height[2], nrChannels[2];
	unsigned char* data[2];
	stbi_set_flip_vertically_on_load(true);//设置y轴反转
	data[0] = stbi_load("E:\\Learn\\Deps\\images_in\\container.jpg", &width[0], &height[0], &nrChannels[0], 0);
	data[1] = stbi_load("E:\\Learn\\Deps\\images_in\\awesomefaceC.png", &width[1], &height[1], &nrChannels[1], 0);
	//这个awesomefaceC.png是实验6次的产物，最后笑脸的周围终于是透明而非黑边了
	//具体操作是:PS打开图片，新建alpha通道，设置透明度为100%，选取透明区域，导出为png，勾选透明度和较小文件

	for (int i = 0; i <= 1; i++)
	{
		glGenTextures(1, &texture[i]);

		//绑定纹理对象至纹理目标
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		//为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//检测是否正常读取到图像信息
		if (data[i])
		{
			//生成纹理
			if (i == 0)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
			if (i == 1)//(png文件有透明值并且通过alpha channel，应使用RGBA)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width[i], height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
			//生成MipMap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture[" << i << "]" << std::endl;
		//释放图像内存
		stbi_image_free(data[i]);
	}

	//设置uniform采样器与纹理单元的对应关系
	shader.use(); //不要忘记在设置uniform变量之前激活着色器程序！
	shader.setTex("texture1", 0);
	shader.setTex("texture2", 1);

	//解绑VBO和VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//先解绑VAO
	glBindVertexArray(0);//再解绑VBO

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//输入检测
		processInput(window);
		//清屏
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一次的颜色和深度缓存
		//激活材质单元并将对应材质绑定上去
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//激活着色器
		shader.use();
		//---------------------------------
		//模型矩阵(在绘图for循环中)
		//观察矩阵
		glm::mat4 view;//看向(相机坐标，目标坐标，世界竖直向上的坐标)
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		//controller移动部分
		//投影矩阵
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		//----------------------------------
		shader.setMat4fv("view", view, 1, GL_FALSE);
		shader.setMat4fv("projection", projection, 1, GL_FALSE);
		//启用深度测试
		glEnable(GL_DEPTH_TEST);
		//绘制三角形
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//添加EBO后索引绘图
		for (unsigned int i = 1; i < 11; i++)//绘制10个模型
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i - 1]);
			float angle = 20.0f * i;//模型旋转角度
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4fv("model", model, 1, GL_FALSE);;

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	shader.del();

	//终止，清除所有先前分配的 GLFW 资源。
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
	//摄像机：WASD
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	float cameraSpeed = 2.5f * deltaTime; //将帧率与相机速度结合，保证所有设备移动速度相同

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	//摄像机：mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏光标
	glfwSetCursorPosCallback(window, mouse_callback);//注册鼠标回调函数(鼠标一移动注册的函数就会被调用)
	//当鼠标移动触发回调函数，将更新相机视线方向

	//摄像机：mouse_scroll
	glfwSetScrollCallback(window, scroll_callback);//注册滚轮回调函数
	//当鼠标滚轮滚动时，将更新视场角，达到缩放目的


}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)//防止首次进入时跳屏
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//计算当前帧和上一帧鼠标位置的偏移量：
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;                 // 我再解释一下为什么是相反的，记住视线方向是View_z轴负方向
	lastY = ypos;                 // 如果不取反，Screen_y偏移量增加，pitch增加，上抬的是原本是z轴方向的向量，其反方向才是视线，也就是视线实际在下压
	//设置灵敏度
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//把偏移量加到全局变量pitch和yaw上：
	yaw += xoffset;
	pitch += yoffset;
	//限制俯仰角极限
	if (pitch > 89.9f)
		pitch = 89.9f;
	if (pitch < -89.9f)
		pitch = -89.9f;

	//通过俯仰角和偏航角来计算以得到真正的方向向量：
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//通过改变视场角来产生缩放的效果
	//视野(Field of View)或fov定义了我们可以看到场景中多大的范围。
	//当视野变小时，场景投影出来的空间就会减小，产生放大(Zoom In)了的感觉
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

