//我猜你会说我们的箱子喜欢跳70年代的迪斯科。
//写计算机教程的老兄都好幽默

//若想运行本文件请确保how.cpp与part2.cpp已经从生成中排除
//并重新编译
#include<mythings/shader.h>
#include"stb_image.h" //导入不同格式的纹理

void controller(GLFWwindow* window, Shader& shader);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\4\\vShader.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\1_Strat\\4\\fShader.fs";

//控制数据
float w = 0, a = 0, s = 0, d = 0;
float up = 0, down = 0;

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

	//顶点数据
	float vertices[] =
	{
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	//索引数据
	unsigned int indices[] =
	{
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	//创建VAO，VBO，EBO
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//绑定VAO（先）
	glBindVertexArray(VAO);
	//绑定VBO（后）
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据通过VBO存进显卡
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//通过EBO将索引数据复制到缓冲
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//设置顶点属性指针
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//纹理属性
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//创建纹理对象
	unsigned int texture;
	glGenTextures(1, &texture);
	//绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//使用stb_image.h加载图片
	int width, height, nrChannels;
	unsigned char* data = stbi_load("E:\\Learn\\Deps\\images_in\\container.jpg", &width, &height, &nrChannels, 0);
	//检测是否正常读取到图像信息
	if (data)
	{
		//生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//生成MipMap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	//释放图像内存
	stbi_image_free(data);

	//解绑VBO和VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//先解绑VAO
	glBindVertexArray(0);//再解绑VBO

	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		//输入检测
		processInput(window, shader);
		//清屏
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//记得激活着色器
		shader.use();
		//绘制三角形
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);//顶点索引从0开始，一共画3个顶点
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//开启线框模式
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//添加EBO后索引绘图
		//双缓冲
		glfwSwapBuffers(window);
		//事件检测
		glfwPollEvents();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//关闭线框模式
	//结束，释放资源(VAO,VBO,着色器程序)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	shader.del();

	//终止，清除所有先前分配的 GLFW 资源。
	glfwTerminate();

	return 0;
}

void controller(GLFWwindow* window, Shader& shader)
{
	//退出
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//水平移动
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		w += 0.0001;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		a -= 0.0001;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		s -= 0.0001;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		d += 0.0001;
	//竖直移动
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
