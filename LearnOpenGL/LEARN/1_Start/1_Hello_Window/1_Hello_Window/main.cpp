#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//��ȷ�����ڰ���GLFW��ͷ�ļ�֮ǰ������GLAD��ͷ�ļ���
//GLAD��ͷ�ļ���������ȷ��OpenGLͷ�ļ�������GL/gl.h����
//������Ҫ������������OpenGL��ͷ�ļ�֮ǰ����GLAD��

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	//��ʼ��GLFW
	glfwInit();
	//OpenGL�汾��Ϊ3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//ʹ�ú���ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "window_0", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//ע��framebuffer_size_callback����
	//����GLFW����ϣ��ÿ�����ڵ�����С��ʱ������������
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//��ʼ��GLAD�����ʧ�����˳�����
	//GLAD����������OpenGL�ĺ���ָ��ģ�
	//�����ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
	//���Ǹ�GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����
	//GLFW�����ǵ���glfwGetProcAddress�����������Ǳ����ϵͳ��������ȷ�ĺ�����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//��Ⱦѭ��
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);//������

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);//˫�����ͼ(��Ⱦ)

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//������պ����ɫ
		glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����֡�������������õ���ɫ����

		glfwPollEvents();//�¼����

		/*
		glfwWindowShouldClose����:������ÿ��ѭ���Ŀ�ʼǰ���һ��GLFW�Ƿ�Ҫ���˳���
			����ǵĻ��ú�������trueȻ����Ⱦѭ��������ˣ�֮��Ϊ���ǾͿ��Թر�Ӧ�ó����ˡ�

		glfwPollEvents����:�����û�д���ʲô�¼�������������롢����ƶ��ȣ������´���״̬��
			�����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã���

		glfwSwapBuffers����:�ύ����ɫ���壨����һ��������GLFW����ÿһ��������ɫֵ�Ĵ󻺳壩��
			������һ�����б��������ƣ����ҽ�����Ϊ�����ʾ����Ļ�ϡ�
		*/
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	//��Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//��ESC�򽫵��½�����Ⱦѭ��
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
//֡�����С�ص������������ڴ�С�ı䣬�ӿ�Ҳ������Ӧ����
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	//�����ӿڵĺ���
	//����ǰ�����������ƴ������½ǵ�λ�á�
	//�������͵��ĸ�����������Ⱦ���ڵĿ�Ⱥ͸߶ȣ����أ�
}