#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//请确认是在包含GLFW的头文件之前包含了GLAD的头文件。
//GLAD的头文件包含了正确的OpenGL头文件（例如GL/gl.h），
//所以需要在其它依赖于OpenGL的头文件之前包含GLAD。

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	//初始化GLFW
	glfwInit();
	//OpenGL版本号为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//使用核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "window_0", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//注册framebuffer_size_callback函数
	//告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//初始化GLAD（如果失败则退出程序）
	//GLAD是用来管理OpenGL的函数指针的，
	//所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	//我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
	//GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//渲染循环
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);//输入检测

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);//双缓冲绘图(渲染)

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空后的颜色
		glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲帧，并用上面设置的颜色覆盖

		glfwPollEvents();//事件检测

		/*
		glfwWindowShouldClose函数:在我们每次循环的开始前检查一次GLFW是否被要求退出，
			如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。

		glfwPollEvents函数:检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，
			并调用对应的回调函数（可以通过回调方法手动设置）。

		glfwSwapBuffers函数:会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），
			它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		*/
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	//渲染循环结束后我们需要正确释放/删除之前的分配的所有资源

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//按ESC则将导致结束渲染循环
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//帧缓冲大小回调函数，当窗口大小改变，视口也做出相应调整
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	//设置视口的函数
	//函数前两个参数控制窗口左下角的位置。
	//第三个和第四个参数控制渲染窗口的宽度和高度（像素）
}