//���д��ļ�ǰ���뽫�����������ļ����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>
#include<mythings/debug.h>
#include<mythings/render.h>
//#include<mythings/texture.h>

#include<random>

//���ò���
const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

//��ɫ���ļ�·��
const char* PBR_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vPbrShader.vs";
const char* PBR_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fPbrShader.fs";

//�����
Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));//�����ʼλ��
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

//����
GLboolean keys[1024];
GLboolean keysPressed[1024];
GLboolean firstMouse = true;

//ʱ��
GLfloat deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
GLfloat lastFrame = 0.0f; // ��һ֡��ʱ��

//ȫ�ֱ���
glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.8, 0.8, 1.0);

//ѡ��
GLboolean hdr = true; // Change with 'Space'
GLboolean hdrKeyPressed = false;
GLfloat exposure = 0.154f; // �ع�� Change with Q and E
GLboolean blinn = false;
GLboolean blinnKeyPressed = false;
GLboolean gammaEnabled = false;
GLboolean gammaKeyPressed = false;
GLboolean bloom = true;
GLboolean bloomKeyPressed = false;
GLboolean shadows = true;

//����
void controller(GLFWwindow* window);
void processInput(GLFWwindow* window);
void FeedBack();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//������
int main()
{
	//��������
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  //����DebugOutput����Ӱ��opengl�����ٶȣ���������ʽ����ʱ�뽫��ע�͵�
	//��������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//׷�ٲ����ع��
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//ע�ᴰ�ڴ�С�����ص�����
	glfwSetCursorPosCallback(window, mouse_callback);//ע�����ص�����(���һ�ƶ�ע��ĺ����ͻᱻ����)
	glfwSetScrollCallback(window, scroll_callback);//ע����ֻص�����
	glfwSetKeyCallback(window, key_callback);//ע����̻ص�����

	//glad����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//��ʼ���������������
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);//���õ������
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);//���� OpenGL �ڷ�������ʱֱ�ӵ��ûص�����
		glDebugMessageCallback(glDebugOutput, nullptr);  //ע��DebugOutput�ص�����
		//��������Ĵ�������
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);      //�ر�������Ϣ
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0, nullptr, GL_TRUE); //������ERROR������Ϣ
		//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
		//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);    //����������Ϣ
	}

	//�����ϵͳ�в����Զ��������Ϣ
	//glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "error message here");

	//����y�ᷴת(�Ѿ���model.h���غ��ڴ������)
	//stbi_set_flip_vertically_on_load(true);

	//����openGL״̬
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);   //��Ȳ��Ժ�������ΪС�ڵ���(���ڻ�����պУ���Ϊ��պе���ȱ�����Ϊ���ֵ1.0)	//glEnable(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  //����������ͼ����֮�������ȷ���˵�ѡ��

	//ʹ����ɫ������������ɫ������ 
	Shader textShader("E:/Learn/Deps/GLSL/TextRendering/vTextShader.vs", "E:/Learn/Deps/GLSL/TextRendering/fTextShader.fs");
	//debugShader
	const char* vertPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.vs";
	const char* fargPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.fs";
	Shader debugShader(vertPath, fargPath);  //�ڳ�����������glad��ʼ��֮�󴴽�shader

	//��������
	//**

	//����ģ��
	//**

	//DATAs
	//**

	//FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// find path to font
	std::string font_name = "E:/Learn/Deps/Sources/fonts/arial.ttf";
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//֡����-------------------------------------------------------------------------
	//**

	//������ɫ������Ԫ
	//**

	//�����߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		//per-frame time logic
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//�¼����
		glfwPollEvents();
		//������
		processInput(window);

		//����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����һ�ε���ɫ����Ȼ���

		//render
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		textShader.use();
		textShader.setMat4("projection", projection);

		//render text
		RenderText(textShader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(textShader, "HELLO FREETYPY!", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		//˫����
		glfwSwapBuffers(window);
		//���з���
		FeedBack();
	}
	std::cout << "MISSION COMPLETE!" << std::endl;

	//��ֹ�����������ǰ����� GLFW ��Դ��
	glfwTerminate();

	return 0;
}

//ÿ֡����
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
	//�������WASD
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
		blinnKeyPressed = true;//B����סʱ�����ظ��ı�blinn��ֵ
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
	{
		blinnKeyPressed = false;//B�ɿ�ʱ����blinnֵ���Ըı�
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
	//�ع��
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

//ÿ֡����
void FeedBack()
{
	//camera.report();
	//std::cout << (blinn ? "Blinn-Phong" : "      Phong") << " ";
	//std::cout << (gammaEnabled ? "Gamma enabled " : "Gamma disabled") << std::endl;
	//std::cout << (shadows ? "Shadows  enabled [SPACE] " : "Shadows disabled [SPACE]") << std::endl;
	//std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;
	//std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
}

//�ص�����
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
	// ע������yƫ�������෴�ģ���Ϊy�����Ǵӵײ����������������
	// ���ٽ���һ��Ϊʲô���෴�ģ���ס���߷�����View_z�Ḻ����
	// �����ȡ����Screen_yƫ�������ӣ�pitch���ӣ���̧����ԭ����z�᷽����������䷴����������ߣ�Ҳ��������ʵ������ѹ

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
