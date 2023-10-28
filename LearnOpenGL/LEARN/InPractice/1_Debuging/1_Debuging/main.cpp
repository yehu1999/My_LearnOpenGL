//运行此文件前，请将其他非依赖文件从生成中排除
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>
#include<mythings/debug.h>
#include<mythings/texture.h>
#include<mythings/render.h>
#include<random>

//设置参数
const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;

//着色器文件路径
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
GLuint planeVAO;
glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
glm::vec3 lightColor = glm::vec3(0.8, 0.8, 1.0);
GLuint woodTexture, containerTexture;

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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  //开启DebugOutput，会影响opengl运行速度，在生成正式程序时请将其注释掉
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
	glDepthFunc(GL_LEQUAL);   //深度测试函数设置为小于等于(用于绘制天空盒，因为天空盒的深度被设置为最大值1.0)	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_FRAMEBUFFER_SRGB);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);  //在立方体贴图的面之间进行正确过滤的选项

	//使用着色器函数创建着色器程序 
	Shader pbrShader(PBR_vsPath, PBR_fsPath);                 //单色球pbr
	Shader pbrMapShader(PBR_vsPath, PBRMAP_fsPath);           //材质球pbr
	Shader lightShader(L_vsPath, L_fsPath);                   //光源球
	Shader equirectangularToCubemapShader(C_vsPath, C_fsPath);//等距柱状投影贴图转立方体贴图
	Shader irradianceShader(C_vsPath, I_fsPath);              //辐照度
	Shader prefilterShader(C_vsPath, PF_fsPath);              //预过滤
	Shader brdfShader(BRDF_vsPath, BRDF_fsPath);              //预计算BRDF
	Shader backgroundShader(B_vsPath, B_fsPath);              //天空盒
	//debugShader
	const char* vertPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.vs";
	const char* fargPath = "E:/Learn/Deps/GLSL/Debug/DisplayFramebufferTexture.fs";
	Shader debugShader(vertPath, fargPath);  //在程序主函数的glad初始化之后创建shader

	//加载纹理
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
	//等距柱状投影贴图
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
	//加载模型
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

	//帧缓冲-------------------------------------------------------------------------
	//capture帧缓冲（用于向六个方向“拍摄”）
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	//颜色附件 （立方体贴图）
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

	//生成立方体贴图（将hdr等距柱状投影贴图转换为立方体贴图）
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

	//生成辐照度贴图帧缓冲 （用于储存辐照度漫反射卷积结果）
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

	//生成辐照度贴图（在着色器内对立方体贴图进行卷积）
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

	//预滤波环境贴图帧缓冲 （对立方体贴图进行多级别模糊，对应不同粗糙度级别，将被用于之后BRDF的镜面反射计算）
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
	//生成mipmap
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//生成各级预滤波环境贴图
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

	//2D LUT纹理帧缓冲 （BRDF 积分贴图，用于储存BRDF镜面反射的卷积结果）
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

	//生成LUT纹理 （在着色器内对立方体贴图进行卷积）
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

	glViewport(0, 0, 512, 512);
	brdfShader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//设置着色器纹理单元
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

	//将视口配置为原始帧缓冲的屏幕尺寸
	int scrWidth, scrHeight;
	glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, scrWidth, scrHeight);

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
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		pbrShader.use();
		pbrShader.setMat4("projection", projection);
		pbrShader.setMat4("view", view);
		pbrShader.setVec3("camPos", camera.Position);

		//绑定预计算的IBL数据
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); //辐照度环境贴图
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);  //预滤波环境贴图
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);      //BRDF环境贴图

		//绘制纯色球体阵列
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
		//绘制材质球体
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

		//渲染光源
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

			//绘制光源
			lightShader.use();
			lightShader.setMat4("model", model);
			lightShader.setMat4("projection", projection);
			lightShader.setMat4("view", view);
			lightShader.setVec3("lightColor", 1.0, 1.0, 1.0);
			renderSphere();
		}

		//绘制天空盒（最后绘制，防止场景被覆盖。用于展示和调试生成的立体纹理）
		backgroundShader.use();
		backgroundShader.setMat4("projection", projection);
		backgroundShader.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);    //pbr立体贴图
		//glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); //pbr辐照度贴图
		//glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // display prefilter map
		renderCube();

		//debugShader
		DisplayFramebufferTexture(debugShader, brdfLUTTexture);

		//DEBUG:render BRDF map to screen
		//brdfShader.use();
		//renderQuad();

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