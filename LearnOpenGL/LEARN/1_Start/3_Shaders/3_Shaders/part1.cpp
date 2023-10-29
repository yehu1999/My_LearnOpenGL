#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//顶点着色器的源代码
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";
//片段着色器的源代码
const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
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
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
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
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//索引从0开始，每个顶点含3个float值
	glEnableVertexAttribArray(0);
	//解绑VBO和VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//先解绑VAO
	glBindVertexArray(0);//再解绑VBO
	//着色器
	/*
	在Hello Triangle中提到，着色器(Shader)是运行在GPU上的小程序。
	这些小程序为图形渲染管线的某个特定部分而运行。
	从基本意义上来说，着色器只是一种把输入转化为输出的程序。
	着色器也是一种非常独立的程序，因为它们之间不能相互通信；
	它们之间唯一的沟通只有通过输入和输出。

	前面我们简要地触及了一点着色器的皮毛，并了解了如何恰当地使用它们。
	现在我们会用一种更加广泛的形式详细解释着色器，
	特别是OpenGL着色器语言(GLSL)。
	*/

	//GLSL
	/*
	着色器是使用一种叫GLSL的类C语言写成的。
	GLSL是为图形计算量身定制的，
	它包含一些针对向量和矩阵操作的有用特性。
	
	着色器的开头总是要声明版本，接着是输入和输出变量、uniform和main函数。
	每个着色器的入口点都是main函数，
	在这个函数中我们处理所有的输入变量，并将结果输出到输出变量中。
	
	一个典型的着色器有下面的结构：

	#version version_number     //声明版本

	in type in_variable_name;   //输入变量
	in type in_variable_name;   

	out type out_variable_name; //输出变量

	uniform type uniform_name;

	int main()
	{
	  // 处理输入并进行一些图形操作
	  ...
	  // 输出处理过的结果到输出变量
	  out_variable_name = weird_stuff_we_processed;
	}


	当我们特别谈论到顶点着色器的时候，每个输入变量也叫顶点属性(Vertex Attribute)。
	我们能声明的顶点属性是有上限的，它一般由硬件来决定。
	OpenGL确保至少有16个包含4分量的顶点属性可用，
	但是有些硬件或许允许更多的顶点属性，
	你可以查询GL_MAX_VERTEX_ATTRIBS来获取具体的上限：
	*/

	//获取顶点属性数量上限
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//数据类型
	/*
	和其他编程语言一样，GLSL有数据类型可以来指定变量的种类。
	GLSL中包含C等其它语言大部分的默认基础数据类型：
	int、float、double、uint和bool。
	GLSL也有两种容器类型，它们会在这个教程中使用很多，
	分别是向量(Vector)和矩阵(Matrix)，其中矩阵我们会在之后的教程里再讨论。
	
	向量：
	GLSL中的向量是一个可以包含有2、3或者4个分量的容器，
	分量的类型可以是前面默认基础类型的任意一个。
	它们可以是下面的形式（n代表分量的数量）：
	类型	含义
	vecn	包含n个float分量的默认向量
	bvecn	包含n个bool分量的向量
	ivecn	包含n个int分量的向量
	uvecn	包含n个unsigned int分量的向量
	dvecn	包含n个double分量的向量

	一个向量的分量可以通过vec.x这种方式获取，这里x是指这个向量的第一个分量。
	你可以分别使用.x、.y、.z和.w来获取它们的第1、2、3、4个分量。
	GLSL也允许你对颜色使用rgba，
	或是对纹理坐标使用stpq访问相同的分量。

	向量这一数据类型也允许一些有趣而灵活的分量选择方式，
	叫做重组(Swizzling)。
	重组允许这样的语法：
	vec2 someVec;
	vec4 differentVec = someVec.xyxx;
	vec3 anotherVec = differentVec.zyw;
	vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
	你可以使用上面4个字母任意组合来创建一个和原来向量一样长的（同类型）新向量，只要原来向量有那些分量即可；
	然而，你不允许在一个vec2向量中去获取.z元素。
	我们也可以把一个向量作为一个参数传给不同的向量构造函数，以减少需求参数的数量：
	vec2 vect = vec2(0.5, 0.7);
	vec4 result = vec4(vect, 0.0, 0.0);
	vec4 otherResult = vec4(result.xyz, 1.0);
	向量是一种灵活的数据类型，我们可以把它用在各种输入和输出上。
	*/

	//输入与输出
	/*
	虽然着色器是各自独立的小程序，但是它们都是一个整体的一部分，
	出于这样的原因，我们希望每个着色器都有输入和输出，这样才能进行数据交流和传递。
	
	GLSL定义了in和out关键字专门来实现这个目的。
	每个着色器使用这两个关键字设定输入和输出，
	只要一个输出变量与下一个着色器阶段的输入匹配，它就会传递下去。
	但在顶点和片段着色器中会有点不同。

	顶点着色器的输入特殊在，它从顶点数据中直接接收输入。
	为了定义顶点数据该如何管理，我们使用location这一元数据指定输入变量，
	这样我们才可以在CPU上配置顶点属性。
	我们已经在前面的看过这个了，layout (location = 0)。
	顶点着色器需要为它的输入提供一个额外的layout标识，这样我们才能把它链接到顶点数据。
	
	另一个例外是片段着色器，它需要一个vec4颜色输出变量，因为片段着色器需要生成一个最终输出的颜色。
	如果你在片段着色器没有定义输出颜色，OpenGL会把你的物体渲染为黑色（或白色）。
	
	所以，如果我们打算从一个着色器向另一个着色器发送数据，我们必须在发送方着色器中声明一个输出，在接收方着色器中声明一个类似的输入。
	当类型和名字都一样的时候，OpenGL就会把两个变量链接到一起，它们之间就能发送数据了（这是在链接程序对象时完成的）。
	
	为了展示这是如何工作的，我们会稍微改动一下之前教程里的那个着色器，
	让顶点着色器为片段着色器决定颜色：
	顶点着色器
	#version 330 core
	layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为0
	out vec4 vertexColor; // 为片段着色器指定一个颜色输出
	void main()
	{
		gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数
		vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色
	}
	片段着色器
	#version 330 core
	out vec4 FragColor;
	in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
	void main()
	{
		FragColor = vertexColor;
	}
	*/


	//Uniform
	/*
	Uniform是一种从CPU中的应用向GPU中的着色器发送数据的方式，但uniform和顶点属性有些不同。
	#version 330 core
	out vec4 FragColor;
	uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量
	void main()
	{
		FragColor = ourColor;
	}
	首先，uniform是全局的(Global)。
	全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，
	而且它可以被着色器程序的任意着色器在任意阶段访问。
	第二，无论你把uniform值设置成什么，uniform会一直保存它们的数据，直到它们被重置或更新。
	
	如果你声明了一个uniform却在GLSL代码中没用过，编译器会静默移除这个变量，
	导致最后编译出的版本中并不会包含它，这可能导致几个非常麻烦的错误，记住这点！
	
	这个uniform现在还是空的；我们还没有给它添加任何数据，所以下面我们就做这件事。
	我们首先需要找到着色器中uniform属性的索引/位置值。
	当我们得到uniform的索引/位置值后，我们就可以更新它的值了。
	这次我们不去给像素传递单独一个颜色，而是让它随着时间改变颜色：
	*/
float timeValue = glfwGetTime();  //获取运行的秒数
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");//查询uniform ourColor的位置值
glUseProgram(shaderProgram);//激活着色器程序
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//通过glUniform4f函数设置uniform值

//注意，查询uniform地址不要求你之前使用过着色器程序，
//但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，
//因为它是在当前激活的着色器程序中设置uniform的。

/*
* 因为OpenGL在其核心是一个C库，所以它不支持类型重载，
在函数参数不同的时候就要为其定义新的函数；
glUniform是一个典型例子。
这个函数有一个特定的后缀，标识设定的uniform的类型。可能的后缀有：
	后缀	含义
	f	函数需要一个float作为它的值
	i	函数需要一个int作为它的值
	ui	函数需要一个unsigned int作为它的值
	3f	函数需要3个float作为它的值
	fv	函数需要一个float向量/数组作为它的值
每当你打算配置一个OpenGL的选项时就可以简单地根据这些规则选择适合你的数据类型的重载函数。
在我们的例子里，我们希望分别设定uniform的4个float值，
所以我们通过glUniform4f传递我们的数据(注意，我们也可以使用fv版本)。
*/

//现在你知道如何设置uniform变量的值了，我们可以使用它们来渲染了。
//如果我们打算让颜色慢慢变化，我们就要在游戏循环的每一次迭代中（所以他会逐帧改变）更新这个uniform，
//否则三角形就不会改变颜色。
//下面我们就计算greenValue然后每个渲染迭代都更新这个uniform：

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

/*
可以看到，uniform对于设置一个在渲染迭代中会改变的属性是一个非常有用的工具，
它也是一个在程序和着色器间数据交互的很好工具，但假如我们打算为每个顶点设置一个颜色的时候该怎么办？
这种情况下，我们就不得不声明和顶点数目一样多的uniform了。
在这一问题上更好的解决方案是在顶点属性中包含更多的数据，这是我们接下来要做的事情。
见part2
*/

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

