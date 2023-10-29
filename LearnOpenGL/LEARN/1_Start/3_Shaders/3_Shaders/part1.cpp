#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//������ɫ����Դ����
const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";
//Ƭ����ɫ����Դ����
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
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//���붥����ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//����Դ��
	glCompileShader(vertexShader);//����
	//���������
	int  success;//��ʾ�Ƿ�ɹ�����
	char infoLog[512];//���������Ϣ������еĻ���������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//�������ʧ�ܣ����ȡʧ����Ϣ����ӡ
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//����Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//����Դ��
	glCompileShader(fragmentShader);//����
	//���������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		//�������ʧ�ܣ����ȡʧ����Ϣ����ӡ
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//������ɫ������
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//������ɫ��
	glAttachShader(shaderProgram, vertexShader);//������ɫ������
	glAttachShader(shaderProgram, fragmentShader);//Ƭ����ɫ������
	glLinkProgram(shaderProgram);//����
	//���������ɫ���������
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��������
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	//����VAO��VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//��VAO���ȣ�
	glBindVertexArray(VAO);
	//��VBO����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//����������ͨ��VBO����Կ�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//������0��ʼ��ÿ�����㺬3��floatֵ
	glEnableVertexAttribArray(0);
	//���VBO��VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);//�Ƚ��VAO
	glBindVertexArray(0);//�ٽ��VBO
	//��ɫ��
	/*
	��Hello Triangle���ᵽ����ɫ��(Shader)��������GPU�ϵ�С����
	��ЩС����Ϊͼ����Ⱦ���ߵ�ĳ���ض����ֶ����С�
	�ӻ�����������˵����ɫ��ֻ��һ�ְ�����ת��Ϊ����ĳ���
	��ɫ��Ҳ��һ�ַǳ������ĳ�����Ϊ����֮�䲻���໥ͨ�ţ�
	����֮��Ψһ�Ĺ�ֻͨ��ͨ������������

	ǰ�����Ǽ�Ҫ�ش�����һ����ɫ����Ƥë�����˽������ǡ����ʹ�����ǡ�
	�������ǻ���һ�ָ��ӹ㷺����ʽ��ϸ������ɫ����
	�ر���OpenGL��ɫ������(GLSL)��
	*/

	//GLSL
	/*
	��ɫ����ʹ��һ�ֽ�GLSL����C����д�ɵġ�
	GLSL��Ϊͼ�μ��������Ƶģ�
	������һЩ��������;���������������ԡ�
	
	��ɫ���Ŀ�ͷ����Ҫ�����汾����������������������uniform��main������
	ÿ����ɫ������ڵ㶼��main������
	��������������Ǵ������е������������������������������С�
	
	һ�����͵���ɫ��������Ľṹ��

	#version version_number     //�����汾

	in type in_variable_name;   //�������
	in type in_variable_name;   

	out type out_variable_name; //�������

	uniform type uniform_name;

	int main()
	{
	  // �������벢����һЩͼ�β���
	  ...
	  // ���������Ľ�����������
	  out_variable_name = weird_stuff_we_processed;
	}


	�������ر�̸�۵�������ɫ����ʱ��ÿ���������Ҳ�ж�������(Vertex Attribute)��
	�����������Ķ��������������޵ģ���һ����Ӳ����������
	OpenGLȷ��������16������4�����Ķ������Կ��ã�
	������ЩӲ�������������Ķ������ԣ�
	����Բ�ѯGL_MAX_VERTEX_ATTRIBS����ȡ��������ޣ�
	*/

	//��ȡ����������������
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	//��������
	/*
	�������������һ����GLSL���������Ϳ�����ָ�����������ࡣ
	GLSL�а���C���������Դ󲿷ֵ�Ĭ�ϻ����������ͣ�
	int��float��double��uint��bool��
	GLSLҲ�������������ͣ����ǻ�������̳���ʹ�úܶ࣬
	�ֱ�������(Vector)�;���(Matrix)�����о������ǻ���֮��Ľ̳��������ۡ�
	
	������
	GLSL�е�������һ�����԰�����2��3����4��������������
	���������Ϳ�����ǰ��Ĭ�ϻ������͵�����һ����
	���ǿ������������ʽ��n�����������������
	����	����
	vecn	����n��float������Ĭ������
	bvecn	����n��bool����������
	ivecn	����n��int����������
	uvecn	����n��unsigned int����������
	dvecn	����n��double����������

	һ�������ķ�������ͨ��vec.x���ַ�ʽ��ȡ������x��ָ��������ĵ�һ��������
	����Էֱ�ʹ��.x��.y��.z��.w����ȡ���ǵĵ�1��2��3��4��������
	GLSLҲ���������ɫʹ��rgba��
	���Ƕ���������ʹ��stpq������ͬ�ķ�����

	������һ��������Ҳ����һЩ��Ȥ�����ķ���ѡ��ʽ��
	��������(Swizzling)��
	���������������﷨��
	vec2 someVec;
	vec4 differentVec = someVec.xyxx;
	vec3 anotherVec = differentVec.zyw;
	vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
	�����ʹ������4����ĸ�������������һ����ԭ������һ�����ģ�ͬ���ͣ���������ֻҪԭ����������Щ�������ɣ�
	Ȼ�����㲻������һ��vec2������ȥ��ȡ.zԪ�ء�
	����Ҳ���԰�һ��������Ϊһ������������ͬ���������캯�����Լ������������������
	vec2 vect = vec2(0.5, 0.7);
	vec4 result = vec4(vect, 0.0, 0.0);
	vec4 otherResult = vec4(result.xyz, 1.0);
	������һ�������������ͣ����ǿ��԰������ڸ������������ϡ�
	*/

	//���������
	/*
	��Ȼ��ɫ���Ǹ��Զ�����С���򣬵������Ƕ���һ�������һ���֣�
	����������ԭ������ϣ��ÿ����ɫ�����������������������ܽ������ݽ����ʹ��ݡ�
	
	GLSL������in��out�ؼ���ר����ʵ�����Ŀ�ġ�
	ÿ����ɫ��ʹ���������ؼ����趨����������
	ֻҪһ�������������һ����ɫ���׶ε�����ƥ�䣬���ͻᴫ����ȥ��
	���ڶ����Ƭ����ɫ���л��е㲻ͬ��

	������ɫ�������������ڣ����Ӷ���������ֱ�ӽ������롣
	Ϊ�˶��嶥�����ݸ���ι�������ʹ��location��һԪ����ָ�����������
	�������ǲſ�����CPU�����ö������ԡ�
	�����Ѿ���ǰ��Ŀ�������ˣ�layout (location = 0)��
	������ɫ����ҪΪ���������ṩһ�������layout��ʶ���������ǲ��ܰ������ӵ��������ݡ�
	
	��һ��������Ƭ����ɫ��������Ҫһ��vec4��ɫ�����������ΪƬ����ɫ����Ҫ����һ�������������ɫ��
	�������Ƭ����ɫ��û�ж��������ɫ��OpenGL������������ȾΪ��ɫ�����ɫ����
	
	���ԣ�������Ǵ����һ����ɫ������һ����ɫ���������ݣ����Ǳ����ڷ��ͷ���ɫ��������һ��������ڽ��շ���ɫ��������һ�����Ƶ����롣
	�����ͺ����ֶ�һ����ʱ��OpenGL�ͻ�������������ӵ�һ������֮����ܷ��������ˣ����������ӳ������ʱ��ɵģ���
	
	Ϊ��չʾ������ι����ģ����ǻ���΢�Ķ�һ��֮ǰ�̳�����Ǹ���ɫ����
	�ö�����ɫ��ΪƬ����ɫ��������ɫ��
	������ɫ��
	#version 330 core
	layout (location = 0) in vec3 aPos; // λ�ñ���������λ��ֵΪ0
	out vec4 vertexColor; // ΪƬ����ɫ��ָ��һ����ɫ���
	void main()
	{
		gl_Position = vec4(aPos, 1.0); // ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
		vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // �������������Ϊ����ɫ
	}
	Ƭ����ɫ��
	#version 330 core
	out vec4 FragColor;
	in vec4 vertexColor; // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
	void main()
	{
		FragColor = vertexColor;
	}
	*/


	//Uniform
	/*
	Uniform��һ�ִ�CPU�е�Ӧ����GPU�е���ɫ���������ݵķ�ʽ����uniform�Ͷ���������Щ��ͬ��
	#version 330 core
	out vec4 FragColor;
	uniform vec4 ourColor; // ��OpenGL����������趨�������
	void main()
	{
		FragColor = ourColor;
	}
	���ȣ�uniform��ȫ�ֵ�(Global)��
	ȫ����ζ��uniform����������ÿ����ɫ����������ж��Ƕ�һ�޶��ģ�
	���������Ա���ɫ�������������ɫ��������׶η��ʡ�
	�ڶ����������uniformֵ���ó�ʲô��uniform��һֱ�������ǵ����ݣ�ֱ�����Ǳ����û���¡�
	
	�����������һ��uniformȴ��GLSL������û�ù����������ᾲĬ�Ƴ����������
	������������İ汾�в����������������ܵ��¼����ǳ��鷳�Ĵ��󣬼�ס��㣡
	
	���uniform���ڻ��ǿյģ����ǻ�û�и�������κ����ݣ������������Ǿ�������¡�
	����������Ҫ�ҵ���ɫ����uniform���Ե�����/λ��ֵ��
	�����ǵõ�uniform������/λ��ֵ�����ǾͿ��Ը�������ֵ�ˡ�
	������ǲ�ȥ�����ش��ݵ���һ����ɫ��������������ʱ��ı���ɫ��
	*/
float timeValue = glfwGetTime();  //��ȡ���е�����
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");//��ѯuniform ourColor��λ��ֵ
glUseProgram(shaderProgram);//������ɫ������
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);//ͨ��glUniform4f��������uniformֵ

//ע�⣬��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ������
//���Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram)��
//��Ϊ�����ڵ�ǰ�������ɫ������������uniform�ġ�

/*
* ��ΪOpenGL���������һ��C�⣬��������֧���������أ�
�ں���������ͬ��ʱ���ҪΪ�䶨���µĺ�����
glUniform��һ���������ӡ�
���������һ���ض��ĺ�׺����ʶ�趨��uniform�����͡����ܵĺ�׺�У�
	��׺	����
	f	������Ҫһ��float��Ϊ����ֵ
	i	������Ҫһ��int��Ϊ����ֵ
	ui	������Ҫһ��unsigned int��Ϊ����ֵ
	3f	������Ҫ3��float��Ϊ����ֵ
	fv	������Ҫһ��float����/������Ϊ����ֵ
ÿ�����������һ��OpenGL��ѡ��ʱ�Ϳ��Լ򵥵ظ�����Щ����ѡ���ʺ�����������͵����غ�����
�����ǵ����������ϣ���ֱ��趨uniform��4��floatֵ��
��������ͨ��glUniform4f�������ǵ�����(ע�⣬����Ҳ����ʹ��fv�汾)��
*/

//������֪���������uniform������ֵ�ˣ����ǿ���ʹ����������Ⱦ�ˡ�
//������Ǵ�������ɫ�����仯�����Ǿ�Ҫ����Ϸѭ����ÿһ�ε����У�����������֡�ı䣩�������uniform��
//���������ξͲ���ı���ɫ��
//�������Ǿͼ���greenValueȻ��ÿ����Ⱦ�������������uniform��

//��Ⱦѭ��
while (!glfwWindowShouldClose(window))
{
	// ����
	processInput(window);
	// ��Ⱦ
	// �����ɫ����
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// �ǵü�����ɫ��
	glUseProgram(shaderProgram);
	// ����uniform��ɫ
	float timeValue = glfwGetTime();
	float greenValue = sin(timeValue) / 2.0f + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
	// ����������
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// �������岢��ѯIO�¼�
	glfwSwapBuffers(window);
	glfwPollEvents();
}

/*
���Կ�����uniform��������һ������Ⱦ�����л�ı��������һ���ǳ����õĹ��ߣ�
��Ҳ��һ���ڳ������ɫ�������ݽ����ĺܺù��ߣ����������Ǵ���Ϊÿ����������һ����ɫ��ʱ�����ô�죿
��������£����ǾͲ��ò������Ͷ�����Ŀһ�����uniform�ˡ�
����һ�����ϸ��õĽ���������ڶ��������а�����������ݣ��������ǽ�����Ҫ�������顣
��part2
*/

//�������ͷ���Դ(VAO,VBO,��ɫ������)
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgram);

//��ֹ�����������ǰ����� GLFW ��Դ��
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

