//运行此文件前，请将其他非依赖文件从生成中排除
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>
#include<mythings/debug.h>
#include<mythings/render.h>

#include<random>

//设置参数
const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;

//着色器文件路径
const char* PBR_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vPbrShader.vs";
const char* PBR_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fPbrShader.fs";

//摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));//相机初始位置
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

//控制
GLboolean keys[1024];
GLboolean keysPressed[1024];
GLboolean firstMouse = true;

//时间
GLfloat deltaTime = 0.0f; // 当前帧与上一帧的时间差
GLfloat lastFrame = 0.0f; // 上一帧的时间

//全局变量
glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.8, 0.8, 1.0);

//选项
GLboolean hdr = true; // Change with 'Space'
GLboolean hdrKeyPressed = false;
GLfloat exposure = 0.154f; // 曝光度 Change with Q and E
GLboolean blinn = false;
GLboolean blinnKeyPressed = false;
GLboolean gammaEnabled = false;
GLboolean gammaKeyPressed = false;
GLboolean bloom = true;
GLboolean bloomKeyPressed = false;
GLboolean shadows = true;

//函数
void controller(GLFWwindow* window);
void processInput(GLFWwindow* window);
void FeedBack();
GLuint loadTexture(char const* path, bool gammaCorrection = false);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//主函数
int main()
{
	//基础设置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  //开启DebugOutput，会影响opengl运行速度，在生成正式程序时请将其注释掉
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
	glfwSetKeyCallback(window, key_callback);//注册键盘回调函数

	//glad加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//初始化调试输出上下文
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);//启用调试输出
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//告诉 OpenGL 在发生错误时直接调用回调函数
		glDebugMessageCallback(glDebugOutput, nullptr);  //注册DebugOutput回调函数
		//过滤输出的错误类型
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);      //关闭所有消息
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE); //仅开启ERROR类型消息
		//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
		//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);    //开启所有消息
	}

	//向调试系统中插入自定义错误消息
	//glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "error message here");

	//开启y轴反转(已经在model.h记载后期处理过了)
	//stbi_set_flip_vertically_on_load(true);

	//设置openGL状态
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);   //深度测试函数设置为小于等于(用于绘制天空盒，因为天空盒的深度被设置为最大值1.0)	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  //在立方体贴图的面之间进行正确过滤的选项

	//使用着色器函数创建着色器程序 
	//**
	//debugShader
	const char* vertPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.vs";
	const char* fargPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.fs";
	Shader debugShader(vertPath, fargPath);  //在程序主函数的glad初始化之后创建shader

	//加载纹理
	//**

	//加载模型
	//**

	// DATAs
	//**

	//帧缓冲-------------------------------------------------------------------------
	//**

	//设置着色器纹理单元
	//**

	//开启线框模式
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//per-frame time logic
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//事件检测
		glfwPollEvents();
		//输入检测
		processInput(window);

		//清屏
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除上一次的颜色和深度缓存

		//render
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		//**

		//双缓冲
		glfwSwapBuffers(window);
		//运行反馈
		FeedBack();
	}
	std::cout << "MISSION COMPLETE!" << std::endl;

	//终止，清除所有先前分配的 GLFW 资源。
	glfwTerminate();

	return 0;
}

//每帧输入
void processInput(GLFWwindow* window)
{
	//controller
	controller(window);
}
void controller(GLFWwindow* window)
{
	//ESC
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, true);
	//摄像机：WASD
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	//B
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
	{
		blinn = !blinn;
		blinnKeyPressed = true;//B被按住时，不重复改变blinn的值
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		blinnKeyPressed = false;//B松开时，让blinn值可以改变
	}
	//SPACE
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
	{
		gammaEnabled = !gammaEnabled;
		gammaKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		gammaKeyPressed = false;
	}
	if (keys[GLFW_KEY_SPACE] && !keysPressed[GLFW_KEY_SPACE])
	{
		shadows = !shadows;
		keysPressed[GLFW_KEY_SPACE] = true;
	}
	//HDR
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !hdrKeyPressed)
	{
		hdr = !hdr;
		hdrKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		hdrKeyPressed = false;
	}
	//Bloom
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bloomKeyPressed)
	{
		bloom = !bloom;
		bloomKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		bloomKeyPressed = false;
	}
	//曝光度
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (exposure > 0.0f)
			exposure -= 0.001f;
		else
			exposure = 0.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		exposure += 0.001f;
	}

}

//每帧反馈
void FeedBack()
{
	//camera.report();
	//std::cout << (blinn ? "Blinn-Phong" : "      Phong") << " ";
	//std::cout << (gammaEnabled ? "Gamma enabled " : "Gamma disabled") << std::endl;
	//std::cout << (shadows ? "Shadows  enabled [SPACE] " : "Shadows disabled [SPACE]") << std::endl;
	//std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
	//std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
}

//材质加载
GLuint loadTexture(char const* path, bool gammaCorrection)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1)
		{
			internalFormat = dataFormat = GL_RED;
		}
		else if (nrComponents == 3)
		{
			internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

//回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes

	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
			keysPressed[key] = false;
		}
	}
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
