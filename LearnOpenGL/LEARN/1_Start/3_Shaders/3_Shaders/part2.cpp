//part2更多属性
//运行part2前请将part1从生成中排除，并将part2加入生成
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//我们改了着色器源码，但是依旧是使用字符串传递源代码，
//在shader和part3中，我们将使用封装后的着色器
// 
//顶点着色器的源代码
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"  // 位置变量的属性位置值为 0 
"layout (location = 1) in vec3 aColor;\n"// 颜色变量的属性位置值为 1
"out vec3 ourColor;\n"                   // 向片段着色器输出一个颜色
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"               // 将ourColor设置为我们从顶点数据那里得到的输入颜色
"}\0";
//片段着色器的源代码
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
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//编译顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//附加源码
	glCompileShader(vertexShader);//编译
	//检测编译情况
	int  success;//表示是否成功编译
	char infoLog[512];//储存错误消息（如果有的话）的容器
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//如果编译失败，则获取失败信息并打印
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//编译片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//附加源码
	glCompileShader(fragmentShader);//编译
	//检测编译情况
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//如果编译失败，则获取失败信息并打印
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//创建着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//链接着色器
	glAttachShader(shaderProgram, vertexShader);//顶点着色器附加
	glAttachShader(shaderProgram, fragmentShader);//片段着色器附加
	glLinkProgram(shaderProgram);//链接
	//检测链接着色器程序情况
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//顶点数据
	float vertices[] = {
		// 位置              // 颜色
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	//创建VAO，VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//绑定VAO（先）
	glBindVertexArray(VAO);
	//绑定VBO（后）
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据通过VBO存进显卡
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//因为我们添加了另一个顶点属性，并且更新了VBO的内存，
	//我们就必须重新配置顶点属性指针。	
	//设置顶点属性指针
	    //位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	    //颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//更新后的VBO内存中的数据现在看起来像这样：
	/*
	VERTEX1     VERTEX2    VERTEX3
	XYZ RGB     XYZ RGB    XYZ RGB
BYTE0      24          48         72
	这里也可以看出来着色器源码中的layout (location = 0)是啥意思了
	location即每个顶点的第n-1个属性的索引
	例如这里第一个属性是vec3的XYZ值，location=0；
	        第二个属性是vec3的RGB值，location=1；
	这与glVertexAttribPointer函数的第一个参数是对应的

	由于我们现在有了两个顶点属性，我们不得不重新计算步长值。
	为获得数据队列中下一个属性值（比如位置向量的下个x分量）
	我们必须向右移动6个float，其中3个是位置值，另外3个是颜色值。
	这使我们的步长值为6乘以float的字节数（=24字节）。
	同样，这次我们必须指定一个偏移量。
	对于每个顶点来说，位置顶点属性在前，所以它的偏移量是0。
	颜色属性紧随位置数据之后，所以偏移量就是3 * sizeof(float)，用字节来计算就是12字节。

	嗯总结一下：
	glVertexAttribPointer
	(顶点属性索引，属性数据长度，数据类型，是否标准化，顶点数据总步长，偏移量)
	*/

	//插值
	/*
	本代码运行后获得了三种颜色在三角形中由三个顶点向中间渐变的图像，
	这是颜色向三角形中每个像素做插值的结果，插值算法已经在games101的课程中讲过了。

	这在openGL中叫做片段插值。
	当渲染一个三角形时，光栅化(Rasterization)阶段通常会造成比原指定顶点更多的片段。
	光栅会根据每个片段在三角形形状上所处相对位置决定这些片段的位置。
	
	基于这些位置，它会插值(Interpolate)所有片段着色器的输入变量。
	比如说，我们有一个线段，上面的端点是绿色的，下面的端点是蓝色的。
	如果一个片段着色器在线段的70%的位置运行，
	它的颜色输入属性就会是一个绿色和蓝色的线性结合；
	更精确地说就是30%蓝 + 70%绿。

	片段插值会被应用到片段着色器的所有输入属性上。
	*/





	//解绑VBO和VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//先解绑VAO
	glBindVertexArray(0);//再解绑VBO


	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);
		// 渲染
		// 清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// 记得激活着色器
		glUseProgram(shaderProgram);
		// 更新uniform颜色
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		// 绘制三角形
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// 交换缓冲并查询IO事件
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//结束，释放资源(VAO,VBO,着色器程序)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	//终止，清除所有先前分配的 GLFW 资源。
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

