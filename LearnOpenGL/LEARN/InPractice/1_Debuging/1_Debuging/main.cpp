//���д��ļ�ǰ���뽫�����������ļ����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>
#include<mythings/debug.h>
#include<mythings/texture.h>
#include<mythings/render.h>
#include<random>

//���ò���
const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;

//��ɫ���ļ�·��
const char* PBR_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vPbrShader.vs";
const char* PBR_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fPbrShader.fs";
const char* PBRMAP_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fPbrMapShader.fs";
const char* L_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vLightShader.vs";
const char* L_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fLightShader.fs";
const char* C_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vERtoCubeMapShader.vs";
const char* C_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fERtoCubeMapShader.fs";
const char* B_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vBackgroundShader.vs";
const char* B_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fBackgroundShader.fs";
const char* I_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fIrradianceShader.fs";
const char* PF_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fPrefilterShader.fs";
const char* BRDF_vsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/vBrdfShader.vs";
const char* BRDF_fsPath = "E:/Learn/Deps/GLSL/6_PBR/3/2/fBrdfShader.fs";


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
GLuint planeVAO;
glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.8, 0.8, 1.0);
GLuint woodTexture, containerTexture;

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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  //����DebugOutput����Ӱ��opengl�����ٶȣ���������ʽ����ʱ�뽫��ע�͵�
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
	glDepthFunc(GL_LEQUAL);   //��Ȳ��Ժ�������ΪС�ڵ���(���ڻ�����պУ���Ϊ��պе���ȱ�����Ϊ���ֵ1.0)	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  //����������ͼ����֮�������ȷ���˵�ѡ��

	//ʹ����ɫ������������ɫ������ 
	Shader pbrShader(PBR_vsPath, PBR_fsPath);                 //��ɫ��pbr
	Shader pbrMapShader(PBR_vsPath, PBRMAP_fsPath);           //������pbr
	Shader lightShader(L_vsPath, L_fsPath);                   //��Դ��
	Shader equirectangularToCubemapShader(C_vsPath, C_fsPath);//�Ⱦ���״ͶӰ��ͼת��������ͼ
	Shader irradianceShader(C_vsPath, I_fsPath);              //���ն�
	Shader prefilterShader(C_vsPath, PF_fsPath);              //Ԥ����
	Shader brdfShader(BRDF_vsPath, BRDF_fsPath);              //Ԥ����BRDF
	Shader backgroundShader(B_vsPath, B_fsPath);              //��պ�
	//debugShader
	const char* vertPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.vs";
	const char* fargPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.fs";
	Shader debugShader(vertPath, fargPath);  //�ڳ�����������glad��ʼ��֮�󴴽�shader

	//��������
	unsigned int albedo[5] =
	{
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/gold/albedo.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/grass/albedo.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/plastic/albedo.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/rusted_iron/albedo.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/wall/albedo.png")
	};
	unsigned int normal[5] =
	{
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/gold/normal.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/grass/normal.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/plastic/normal.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/rusted_iron/normal.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/wall/normal.png")
	};
	unsigned int metallic[5] =
	{
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/gold/metallic.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/grass/metallic.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/plastic/metallic.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/rusted_iron/metallic.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/wall/metallic.png")
	};
	unsigned int roughness[5] =
	{
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/gold/roughness.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/grass/roughness.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/plastic/roughness.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/rusted_iron/roughness.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/wall/roughness.png")
	};
	unsigned int ao[5] =
	{
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/gold/ao.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/grass/ao.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/plastic/ao.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/rusted_iron/ao.png"),
		loadTexture("E:/Learn/Deps/Sources/textures/pbr/wall/ao.png")
	};
	//�Ⱦ���״ͶӰ��ͼ
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf("E:/Learn/Deps/Sources/textures/hdr/newport_loft.hdr", &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}
	//����ģ��
	//**

	// DATAs
	glm::vec3 lightPositions[] =
	{
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] =
	{
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	//֡����-------------------------------------------------------------------------
	//capture֡���壨�����������������㡱��
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	//��ɫ���� ����������ͼ��
	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//������������ͼ����hdr�Ⱦ���״ͶӰ��ͼת��Ϊ��������ͼ��
	equirectangularToCubemapShader.use();
	equirectangularToCubemapShader.setInt("equirectangularMap", 0);
	equirectangularToCubemapShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//���ɷ��ն���ͼ֡���� �����ڴ�����ն��������������
	unsigned int irradianceMap;
	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	//���ɷ��ն���ͼ������ɫ���ڶ���������ͼ���о����
	irradianceShader.use();
	irradianceShader.setInt("environmentMap", 0);
	irradianceShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		irradianceShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Ԥ�˲�������ͼ֡���� ������������ͼ���ж༶��ģ������Ӧ��ͬ�ֲڶȼ��𣬽�������֮��BRDF�ľ��淴����㣩
	unsigned int prefilterMap;
	glGenTextures(1, &prefilterMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����mipmap
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//���ɸ���Ԥ�˲�������ͼ
	prefilterShader.use();
	prefilterShader.setInt("environmentMap", 0);
	prefilterShader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		prefilterShader.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			prefilterShader.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCube();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//2D LUT����֡���� ��BRDF ������ͼ�����ڴ���BRDF���淴��ľ�������
	unsigned int brdfLUTTexture;
	glGenTextures(1, &brdfLUTTexture);
	// pre-allocate enough memory for the LUT texture.
	glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	// be sure to set wrapping mode to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//����LUT���� ������ɫ���ڶ���������ͼ���о����
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//������ɫ������Ԫ
	pbrShader.use();
	pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
	pbrShader.setFloat("ao", 1.0f);
	pbrShader.setInt("irradianceMap", 0);
	pbrShader.setInt("prefilterMap", 1);
	pbrShader.setInt("brdfLUT", 2);

	pbrMapShader.use();
	pbrMapShader.setInt("albedoMap", 0);
	pbrMapShader.setInt("normalMap", 1);
	pbrMapShader.setInt("metallicMap", 2);
	pbrMapShader.setInt("roughnessMap", 3);
	pbrMapShader.setInt("aoMap", 4);
	pbrMapShader.setInt("irradianceMap", 0);
	pbrMapShader.setInt("prefilterMap", 1);
	pbrMapShader.setInt("brdfLUT", 2);

	backgroundShader.use();
	backgroundShader.setInt("environmentMap", 0);

	//���ӿ�����Ϊԭʼ֡�������Ļ�ߴ�
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����һ�ε���ɫ����Ȼ���

		//render
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		pbrShader.use();
		pbrShader.setMat4("projection", projection);
		pbrShader.setMat4("view", view);
		pbrShader.setVec3("camPos", camera.Position);

		//��Ԥ�����IBL����
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); //���նȻ�����ͼ
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);  //Ԥ�˲�������ͼ
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);      //BRDF������ͼ

		//���ƴ�ɫ��������
		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
		for (int row = 0; row < nrRows; ++row)
		{
			pbrShader.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				pbrShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				pbrShader.setMat4("model", model);
				pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				renderSphere();
			}
		}
		//���Ʋ�������
		pbrMapShader.use();
		pbrMapShader.setMat4("projection", projection);
		pbrMapShader.setMat4("view", view);
		pbrMapShader.setVec3("camPos", camera.Position);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-4.5, 0.0, 5.0));
		model = glm::scale(model, glm::vec3(0.5f));
		for (int i = 0; i < 5; i++)
		{
			model = glm::translate(model, glm::vec3(3.0, 0.0, 0.0));
			pbrMapShader.setMat4("model", model);
			pbrMapShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, albedo[i]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normal[i]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, metallic[i]);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, roughness[i]);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, ao[i]);

			renderSphere();
		}

		//��Ⱦ��Դ
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			pbrShader.use();
			pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
			pbrMapShader.use();
			pbrMapShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrMapShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShader.setMat4("model", model);
			pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));

			//���ƹ�Դ
			lightShader.use();
			lightShader.setMat4("model", model);
			lightShader.setMat4("projection", projection);
			lightShader.setMat4("view", view);
			lightShader.setVec3("lightColor", 1.0, 1.0, 1.0);
			renderSphere();
		}

		//������պУ������ƣ���ֹ���������ǡ�����չʾ�͵������ɵ���������
		backgroundShader.use();
		backgroundShader.setMat4("projection", projection);
		backgroundShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);    //pbr������ͼ
		//glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); //pbr���ն���ͼ
		//glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
		renderCube();

		//debugShader
		DisplayFramebufferTexture(debugShader, brdfLUTTexture);

		//DEBUG:render BRDF map to screen
		//brdfShader.use();
		//renderQuad();

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