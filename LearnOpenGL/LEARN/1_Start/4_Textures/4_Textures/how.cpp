//��Ϊ�̳̣���cpp�ļ����޷����еģ��뽫���ļ��������ų�ȥ����main.cpp
#include<mythings/shader.h>
#include<iostream>
#include"stb_image.h" //���벻ͬ��ʽ������

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vsPath = "E:\\Learn\\Deps\\GLSL\\4\\vShader.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\4\\fShader.fs";

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
	Shader ourShader(vsPath, fsPath);
	//����
	/*
	�����Ѿ��˽⵽�����ǿ���Ϊÿ�����������ɫ������ͼ�ε�ϸ�ڣ�
	�Ӷ���������Ȥ��ͼ�񡣵��ǣ��������ͼ�ο���������ʵ�����Ǿͱ������㹻��Ķ��㣬
	�Ӷ�ָ���㹻�����ɫ���⽫������ܶ���⿪������Ϊÿ��ģ�Ͷ����������Ķ��㣬
	ÿ������������һ����ɫ���ԡ�


	�����Һͳ���Ա��ϲ��ʹ������(Texture)��
	������һ��2DͼƬ������Ҳ��1D��3D��������������������������ϸ�ڣ�
	���������������һ�Ż���ש���ֽ���޷��۵����ϵ����3D�ķ����ϣ�
	������ķ��ӿ�����������שǽ����ˡ���Ϊ���ǿ�����һ��ͼƬ�ϲ���ǳ����ϸ�ڣ�
	�����Ϳ���������ǳ���ϸ������ָ������Ķ��㡣

	����ͼ�����⣬����Ҳ���Ա�����������������ݣ�
	��Щ���ݿ��Է��͵���ɫ���ϣ������ⲻ���������ڵ����⡣
	*/

	//��������(Texture Coordinate)
	/*
	Ϊ���ܹ�������ӳ��(Map)���������ϣ�������Ҫָ�������ε�ÿ��������Զ�Ӧ������ĸ����֡�
	����ÿ������ͻ������һ����������(Texture Coordinate)�����������ô�����ͼ����ĸ����ֲ�������ע���ɼ�Ƭ����ɫ����
	֮����ͼ�ε�����Ƭ���Ͻ���Ƭ�β�ֵ(Fragment Interpolation)��

	����������x��y���ϣ���ΧΪ0��1֮�䣨ע������ʹ�õ���2D����ͼ�񣩡�
	ʹ�����������ȡ������ɫ��������(Sampling)��
	����������ʼ��(0, 0)��Ҳ��������ͼƬ�����½ǣ���ʼ��(1, 1)��������ͼƬ�����Ͻǡ�

	����Ϊ������ָ����3����������㡣
	����ϣ�������ε����½Ƕ�Ӧ��������½ǣ�������ǰ����������½Ƕ����������������Ϊ(0, 0)��
	�����ε��϶����Ӧ��ͼƬ������λ���������ǰ�����������������Ϊ(0.5, 1.0)��
	ͬ�����·��Ķ�������Ϊ(1, 0)��
	����ֻҪ��������ɫ������������������������ˣ����������ǻᱻ��Ƭ����ɫ���У�
	����Ϊÿ��Ƭ�ν�����������Ĳ�ֵ��

	�������꿴��������������
	*/
	float texCoords[] =
	{
	0.0f, 0.0f, // ���½�
	1.0f, 0.0f, // ���½�
	0.5f, 1.0f // ����
	};

	//����������Ľ��ͷǳ����ɣ������Բ��ü��ֲ�ͬ�Ĳ�ֵ��ʽ��
	//����������Ҫ�Լ�����OpenGL�����������������

	//�����Ʒ�ʽ
	/*
	��������ķ�Χͨ���Ǵ�(0, 0)��(1, 1)��
	��������ǰ��������������ڷ�Χ֮��ᷢ��ʲô��
	OpenGLĬ�ϵ���Ϊ���ظ��������ͼ�����ǻ����Ϻ��Ը�������������������֣���

	��OpenGL�ṩ�˸����ѡ��
	���Ʒ�ʽ	        ����
	GL_REPEAT	        �������Ĭ����Ϊ���ظ�����ͼ��
	GL_MIRRORED_REPEAT	��GL_REPEATһ������ÿ���ظ�ͼƬ�Ǿ�����õġ�
	GL_CLAMP_TO_EDGE	��������ᱻԼ����0��1֮�䣬�����Ĳ��ֻ��ظ���������ı�Ե������һ�ֱ�Ե�������Ч����
	GL_CLAMP_TO_BORDER	����������Ϊ�û�ָ���ı�Ե��ɫ��
	*/
	//ǰ���ᵽ��ÿ��ѡ�����ʹ��glTexParameteri�����Ե�����һ�����������ã�s��t�������ʹ��3D������ô����һ��r�����Ǻ�x��y��z�ǵȼ۵ģ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameter����
	/*
	��һ������ָ��������Ŀ�ꣻ����ʹ�õ���2D�����������Ŀ����GL_TEXTURE_2D���ڶ���������Ҫ����ָ�����õ�ѡ����Ӧ�õ������ᡣ
	���Ǵ������õ���WRAPѡ�����ָ��S��T�ᡣ
	���һ��������Ҫ���Ǵ���һ�����Ʒ�ʽ(Wrapping)��
	�����������OpenGL�����ǰ����������趨�����Ʒ�ʽΪGL_MIRRORED_REPEAT��
	*/
	//�������ѡ��GL_CLAMP_TO_BORDERѡ����ǻ���Ҫָ��һ����Ե����ɫ��
	//����Ҫʹ��glTexParameter������fv��׺��ʽ����GL_TEXTURE_BORDER_COLOR��Ϊ����ѡ�
	//���Ҵ���һ��float������Ϊ��Ե����ɫֵ��
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//�������
	/*
	�������겻�����ڷֱ���(Resolution)�������������⸡��ֵ������OpenGL��Ҫ֪����������������(Texture Pixel��Ҳ��Texel����ע1)ӳ�䵽�������ꡣ
	������һ���ܴ�����嵫������ķֱ��ʺܵ͵�ʱ����ͱ�ú���Ҫ�ˡ�

	������Ѿ��µ��ˣ�OpenGLҲ�ж����������(Texture Filtering)��ѡ�
	��������кܶ��ѡ�������������ֻ��������Ҫ�����֣�GL_NEAREST��GL_LINEAR��

	GL_NEAREST��Ҳ���ڽ����ˣ�Nearest Neighbor Filtering����OpenGLĬ�ϵ�������˷�ʽ��
	������ΪGL_NEAREST��ʱ��OpenGL��ѡ�����ĵ���ӽ�����������Ǹ����ء�

	GL_LINEAR��Ҳ�����Թ��ˣ�(Bi)linear Filtering����������������긽�����������أ������һ����ֵ�����Ƴ���Щ��������֮�����ɫ��
	һ���������ص����ľ�����������Խ������ô����������ص���ɫ�����յ�������ɫ�Ĺ���Խ��

	��ô������������˷�ʽ���������Ӿ�Ч���أ�
	�����ǿ�����һ���ܴ��������Ӧ��һ�ŵͷֱ��ʵ�����ᷢ��ʲô��
	�������Ŵ��ˣ�ÿ���������ض��ܿ�������

	GL_NEAREST�����˿���״��ͼ���������ܹ��������������������أ���GL_LINEAR�ܹ�������ƽ����ͼ�������ѿ����������������ء�
	GL_LINEAR���Բ�������ʵ�����������Щ�����߸�ϲ��8-bit����������ǻ���GL_NEARESTѡ�
	*/
	//�����зŴ�(Magnify)����С(Minify)������ʱ���������������˵�ѡ�
	//�����������������С��ʱ��ʹ���ڽ����ˣ����Ŵ�ʱʹ�����Թ��ˡ�
	//������Ҫʹ��glTexParameter*����Ϊ�Ŵ����Сָ�����˷�ʽ��
	//��δ��뿴������������Ʒ�ʽ�����ú����ƣ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//�༶��Զ����(Mipmap)
	/*
	�����������Ǻ�Զʱ��һ�������������ӿڿ�������ֻռ���ٵ�һƬ���أ�
	������������ʵ�ʵ������Ⱥܸߣ����������¶�����������������ɫ�ͺ�����ʧ�档
	������˵����ʹ�ø߷ֱ��������˷��˶����ڴ���Դ��

	OpenGLʹ��һ�ֽ����༶��Զ����(Mipmap)�ĸ��������������⣬
	������˵����һϵ�е�����ͼ�񣬺�һ������ͼ����ǰһ���Ķ���֮һ��
	�༶��Զ�����������ܼ򵥣���۲��ߵľ��볬��һ������ֵ��
	OpenGL��ʹ�ò�ͬ�Ķ༶��Զ���������ʺ�����ľ�����Ǹ���
	���ھ���Զ�������Ȳ���Ҳ���ᱻ�û�ע�⵽��
	ͬʱ���༶��Զ������һ�ӷ�֮�����������ܷǳ��á�

	�ֹ�Ϊÿ������ͼ�񴴽�һϵ�ж༶��Զ������鷳���Һ�OpenGL��һ��glGenerateMipmaps������
	�ڴ�����һ������������OpenGL�ͻ�е������������й����ˡ�
	����Ľ̳�����ῴ�������ʹ������

	����Ⱦ���л��༶��Զ������(Level)ʱ��OpenGL��������ͬ����Ķ༶��Զ�����֮����������ʵ����Ӳ�߽硣
	������ͨ���������һ�����л��༶��Զ������ʱ��Ҳ������������ͬ�༶��Զ������֮��ʹ��NEAREST��LINEAR���ˡ�
	Ϊ��ָ����ͬ�༶��Զ������֮��Ĺ��˷�ʽ�������ʹ�������ĸ�ѡ���е�һ������ԭ�еĹ��˷�ʽ��

	���˷�ʽ	����
	GL_NEAREST_MIPMAP_NEAREST	ʹ�����ڽ��Ķ༶��Զ������ƥ�����ش�С����ʹ���ڽ���ֵ�����������
	GL_LINEAR_MIPMAP_NEAREST	ʹ�����ڽ��Ķ༶��Զ�����𣬲�ʹ�����Բ�ֵ���в���
	GL_NEAREST_MIPMAP_LINEAR	��������ƥ�����ش�С�Ķ༶��Զ����֮��������Բ�ֵ��ʹ���ڽ���ֵ���в���
	GL_LINEAR_MIPMAP_LINEAR		�������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ����ʹ�����Բ�ֵ���в���
	*/

	//�����������һ�������ǿ���ʹ��glTexParameteri�����˷�ʽ����Ϊǰ�������ᵽ�ķ���֮һ��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	//һ�������Ĵ����ǣ����Ŵ���˵�ѡ������Ϊ�༶��Զ�������ѡ��֮һ��
	//����û���κ�Ч������Ϊ�༶��Զ������Ҫ��ʹ����������С������µģ�
	//����Ŵ󲻻�ʹ�ö༶��Զ����Ϊ�Ŵ�������ö༶��Զ�����ѡ������һ��GL_INVALID_ENUM������롣

	//�����봴������
	/*
	ʹ������֮ǰҪ���ĵ�һ�����ǰ����Ǽ��ص����ǵ�Ӧ���С�
	����ͼ����ܱ�����Ϊ���ָ����ĸ�ʽ��ÿ�ֶ����Լ������ݽṹ�����У�
	����������β��ܰ���Щͼ����ص�Ӧ�����أ�

	һ�����������ѡһ����Ҫ���ļ���ʽ������.PNG��
	Ȼ���Լ�дһ��ͼ�����������ͼ��ת��Ϊ�ֽ����С�
	д�Լ���ͼ���������Ȼ���ѣ�����Ȼͦ�鷳�ģ�
	�������Ҫ֧�ָ����ļ���ʽ�أ���Ͳ��ò�Ϊÿ����ϣ��֧�ֵĸ�ʽд�������ˡ�

	��һ���������Ҳ����һ�ָ��õ�ѡ��
	ʹ��һ��֧�ֶ������и�ʽ��ͼ����ؿ���Ϊ���ǽ��������⡣
	����˵����Ҫ�õ�stb_image.h�⡣
	*/

	//stb_image.h
	/*
	stb_image.h��Sean Barrett��һ���ǳ����еĵ�ͷ�ļ�ͼ����ؿ⣬
	���ܹ����ش󲿷����е��ļ���ʽ�������ܹ��ܼ򵥵����ϵ���Ĺ���֮�С�

	Ҫʹ��stb_image.h����ͼƬ��������Ҫʹ������stbi_load������
	*/
	int width, height, nrChannels;
	unsigned char* data = stbi_load("E:\\Learn\\Deps\\images_in\\container.jpg", &width, &height, &nrChannels, 0);
	/*
	����������Ƚ���һ��ͼ���ļ���λ����Ϊ���롣
	����������Ҫ����int��Ϊ���ĵڶ��������͵��ĸ�������
	stb_image.h������ͼ��Ŀ�ȡ��߶Ⱥ���ɫͨ���ĸ������������������
	���һ������������ͼ���ڴ�����ɫͨ������
	����֮�����������ʱ����õ���ͼ��Ŀ�Ⱥ͸߶ȵġ�
	*/

	//��������
	//��֮ǰ����OpenGL����һ��������Ҳ��ʹ��ID���õģ�
	//�����������
	unsigned int texture1;
	glGenTextures(1, &texture1);
	/*
	glGenTextures����������Ҫ�������������������
	Ȼ������Ǵ����ڵڶ���������unsigned int������
	�����ǵ�������ֻ�ǵ�����һ��unsigned int��
	*/
	//������
	//������������һ����������Ҫ��������֮���κε�����ָ��������õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture1);

	//����Ƿ�������ȡ��ͼ����Ϣ
	if (data)
	{
		//���������Ѿ����ˣ����ǿ���ʹ��ǰ�������ͼƬ��������һ�������ˡ�
		//�������ͨ��glTexImage2D�����ɣ�
		//����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		/*
		��һ������ָ��������Ŀ��(Target)��
			����ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ�����
			���κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��

		�ڶ�������Ϊ����ָ���༶��Զ����ļ���
			�����ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ���
			����������0��Ҳ���ǻ�������

		��������������OpenGL����ϣ����������Ϊ���ָ�ʽ��
			���ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��

		���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ�
			����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����

		�¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��

		���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�
			����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��

		���һ��������������ͼ�����ݡ�

		������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��
		Ȼ����Ŀǰֻ�л�������(Base-level)������ͼ�񱻼����ˣ�
		���Ҫʹ�ö༶��Զ�������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ�����������

		���ߣ�ֱ������������֮�����glGenerateMipmap��
		���Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
		*/
		//����MipMap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	//�ͷ�ͼ���ڴ�(������Ҫ�ˣ�
	//�������������Ӧ�Ķ༶��Զ������ͷ�ͼ����ڴ���һ���ܺõ�ϰ�ߡ�
	stbi_image_free(data);

	//�ܽ�(����һ������Ĺ���
	/*
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	*/

	//Ӧ������
	//������Ҫ��֪OpenGL��β��������������Ǳ���ʹ������������¶������ݣ�
	float vertices[] = 
	{
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
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

	//��Ϊ�����������һ���������ԣ����Ҹ�����VBO���ڴ棬
	//���Ǿͱ����������ö�������ָ�롣	
	//���ö�������ָ��
		//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
		//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//�������������һ������Ķ������ԣ����Ǳ������OpenGL�����µĶ����ʽ��
	/*
	VERTEX_1   VERTEX_2    VERTEX_3
	XYZRGBST   XYZRGBST    XYZRGBST
BYTE       32         64          96 
	*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);//���õ�������������
	
	//ע�⣬����ͬ����Ҫ����ǰ�������������ԵĲ�������Ϊ8 * sizeof(float)��

	//����������Ҫ����������ɫ��ʹ���ܹ����ܶ�������Ϊһ���������ԣ��������괫��Ƭ����ɫ����
	/*
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 aTexCoord;

	out vec3 ourColor;
	out vec2 TexCoord;

	void main()
	{
		gl_Position = vec4(aPos, 1.0);
		ourColor = aColor;
		TexCoord = aTexCoord;
	}
	
	Ƭ����ɫ��Ӧ�ý���������������TexCoord��Ϊ���������
	
	Ƭ����ɫ��ҲӦ���ܷ���������󣬵������������ܰ�������󴫸�Ƭ����ɫ���أ�
	GLSL��һ�����������ʹ�õ��ڽ��������ͣ�����������(Sampler)��
	��������������Ϊ��׺������sampler1D��sampler3D���������ǵ������е�sampler2D��
	���ǿ��Լ�����һ��uniform sampler2D��һ��������ӵ�Ƭ����ɫ���У�
	�Ժ����ǻ������ֵ�����uniform��
	#version 330 core
	out vec4 FragColor;

	in vec3 ourColor;
	in vec2 TexCoord;

	uniform sampler2D ourTexture;

	void main()
	{
		FragColor = texture(ourTexture, TexCoord);
	}

	����ʹ��GLSL�ڽ���texture�����������������ɫ��
	����һ��������������������ڶ��������Ƕ�Ӧ���������ꡣ
	texture������ʹ��֮ǰ���õ������������Ӧ����ɫֵ���в�����
	���Ƭ����ɫ���������������ģ���ֵ�����������ϵ�(���˺��)��ɫ��
	*/

	//����ֻʣ���ڵ���glDrawElements֮ǰ�������ˣ�
	//�����Զ�������ֵ��Ƭ����ɫ���Ĳ�������
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//��������(part1)

	//����Ԫ(part2)
	/*
	����ܻ����Ϊʲôsampler2D�����Ǹ�uniform������ȴ����glUniform������ֵ��
	ʹ��glUniform1i�����ǿ��Ը��������������һ��λ��ֵ�������Ļ������ܹ���һ��Ƭ����ɫ�������ö������
	
	һ�������λ��ֵͨ����Ϊһ������Ԫ(Texture Unit)��
	һ�������Ĭ������Ԫ��0������Ĭ�ϵļ�������Ԫ��
	���Խ̳�ǰ�沿������û�з���һ��λ��ֵ��
	
	����Ԫ����ҪĿ��������������ɫ���п���ʹ�ö���һ��������
	ͨ��������Ԫ��ֵ�������������ǿ���һ�ΰ󶨶������
	ֻҪ�������ȼ����Ӧ������Ԫ������glBindTextureһ����
	���ǿ���ʹ��glActiveTexture��������Ԫ������������Ҫʹ�õ�����Ԫ��
	*/
	glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
	glBindTexture(GL_TEXTURE_2D, texture1);
	//��������Ԫ֮�󣬽�������glBindTexture�������û�����������ǰ���������Ԫ��
	//����ԪGL_TEXTURE0Ĭ�����Ǳ��������������ǰ��������ﵱ����ʹ��glBindTexture��ʱ��
	//���輤���κ�����Ԫ��

	//OpenGL���ٱ�֤��16������Ԫ����ʹ�ã�
	//Ҳ����˵����Լ����GL_TEXTURE0��GL_TEXTRUE15��

	//���Ƕ��ǰ�˳����ģ���������Ҳ����ͨ��GL_TEXTURE0 + 8�ķ�ʽ���GL_TEXTURE8��
	//���ڵ�������Ҫѭ��һЩ����Ԫ��ʱ�������á�

	//������Ȼ��Ҫ�༭Ƭ����ɫ����������һ����������
	//��Ӧ�������˵�ǳ�ֱ���ˣ�
	/*
	#version 330 core
	...

	uniform sampler2D texture1;
	uniform sampler2D texture2;

	void main()
	{
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	}
	���������ɫ��������������Ľ�ϡ�
	GLSL�ڽ���mix������Ҫ��������ֵ��Ϊ�������������Ǹ��ݵ����������������Բ�ֵ��
	���������ֵ��0.0�����᷵�ص�һ�����룻
	�����1.0���᷵�صڶ�������ֵ��
	0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ����������������Ļ��ɫ��
	*/
	
	//���벢������һ������
	//��Ӧ�ö���Щ�������Ϥ�ˡ�
	//�ǵô�����һ�������������ͼƬ��ʹ��glTexImage2D������������(�����ֻ����������û�к�����������)��
	unsigned int texture2;
	glGenTextures(1, &texture2);

	//Ϊ��ʹ�õڶ��������Լ���һ���������Ǳ���ı�һ����Ⱦ���̣�
	//�Ȱ�����������Ӧ������Ԫ��Ȼ�����ĸ�uniform��������Ӧ�ĸ�����Ԫ��

	glActiveTexture(GL_TEXTURE0);//����������Ԫ
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//���������������Ԫ�Ķ�Ӧ��ϵ
	//���ǻ�Ҫͨ��ʹ��glUniform1i����ÿ���������ķ�ʽ����OpenGLÿ����ɫ�������������ĸ�����Ԫ������ֻ��Ҫ����һ
	//�μ��ɣ���������������Ⱦѭ����ǰ�棺
	ourShader.use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // �ֶ�����
	ourShader.setInt("texture2", 1); // ����ʹ����ɫ��������
	ourShader.setTex("texture2", 1); // �Ҵ�����һ�������Եĺ�������Ȼ��setIntһģһ��

	//�����ע�⵽�������µߵ��ˣ�
	//������ΪOpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ�����
	//�����ˣ�stb_image.h�ܹ���ͼ�����ʱ�������Ƿ�תy�ᣬ
	//ֻ��Ҫ�ڼ����κ�ͼ��ǰ����������伴�ɣ�
	stbi_set_flip_vertically_on_load(true);
}