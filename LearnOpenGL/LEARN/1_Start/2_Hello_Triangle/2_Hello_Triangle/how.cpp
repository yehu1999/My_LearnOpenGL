#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
//本节重点：
//VAO,VBO,EBO
//顶点渲染
//图元绘制基本流程




//顶点着色器的源代码
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//片段着色器的源代码
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

/*图形渲染管线
顶点--(顶点着色器)--<标准化设备坐标>--(图元装配)-->
图元--(几何着色器)--<屏幕空间坐标>--(光栅化)-->
片段--(片段着色器)--(测试与混合)

OpenGL没有默认的顶点着色器和片段着色器，需要用户自己提供
*/

int how()
{
	//顶点输入
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	/*深度
	*
	由于OpenGL是在3D空间中工作的，而我们渲染的是一个2D三角形，
	我们将它顶点的z坐标设置为0.0。这样子的话三角形每一点的深度(Depth)都是一样的，
	从而使它看上去像是2D的。

	通常深度可以理解为z坐标，它代表一个像素在空间中和你的距离，
	如果离你远就可能被别的像素遮挡，你就看不到它了，
	它会被丢弃，以节省资源。
	*/

	/*标准化设备坐标(Normalized Device Coordinates, NDC)
	*
	OpenGL不是简单地把所有的3D坐标变换为屏幕上的2D像素；
	OpenGL仅当3D坐标在3个轴（x、y和z）上-1.0到1.0的范围内时才处理它。
	所有在这个范围内的坐标叫做标准化设备坐标(Normalized Device Coordinates)，
	此范围内的坐标最终显示在屏幕上（在这个范围以外的坐标则不会显示）。

	一旦你的顶点坐标已经在顶点着色器中处理过，它们就应该是标准化设备坐标了，
	标准化设备坐标是一个x、y和z值在-1.0到1.0的一小段空间。
	任何落在范围外的坐标都会被丢弃/裁剪，不会显示在你的屏幕上。

	与通常的屏幕坐标不同，y轴正方向为向上，
	(0, 0)坐标是这个图像的中心，而不是左上角。

	在接下来的视口变换，标准化设备坐标将被转换为屏幕空间坐标(Screen-space Coordinates)，
	之后屏幕空间坐标又被变换为片段输入到片段着色器中
	*/

	//创建顶点缓冲对象VBO
	unsigned int VBO;     //缓冲ID
	glGenBuffers(1, &VBO);//生成VBO缓冲对象
	/*
	我们通过顶点缓冲对象(Vertex Buffer Objects, VBO)管理这个内存，
	它会在GPU内存（通常被称为显存）中储存大量顶点。
	使用这些缓冲对象的好处是我们可以一次性的发送一大批数据到显卡上，而不是每个顶点发送一次。
	从CPU把数据发送到显卡相对较慢，所以只要可能我们都要尝试尽量一次性发送尽可能多的数据。
	当数据发送至显卡的内存中后，顶点着色器几乎能立即访问顶点，这是个非常快的过程。
	*/

	//将VBO缓冲对象绑定到GL_ARRAY_BUFFER(VBO的缓冲类型)目标上
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//解绑只需要再次调用该函数，参数传入(GL_ARRAY_BUFFER, 0)
	//在绑定缓冲类型(目标)之后，之后可以使用该缓冲类型(目标)来调用或配置该VBO对象

	//将用户数据复制到缓冲内存
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*glBufferData函数
	*
	glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。
	第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。
	第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。
	第三个参数是我们希望发送的实际数据。
	第四个参数指定了我们希望显卡如何管理给定的数据。
	它有三种形式：

		GL_STATIC_DRAW ：数据不会或几乎不会改变。
		GL_DYNAMIC_DRAW：数据会被改变很多。
		GL_STREAM_DRAW ：数据每次绘制时都会改变。

	三角形的位置数据不会改变，每次渲染调用时都保持原样，所以它的使用类型最好是GL_STATIC_DRAW。
	如果，比如说一个缓冲中的数据将频繁被改变，那么使用的类型就是GL_DYNAMIC_DRAW或GL_STREAM_DRAW，
	这样就能确保显卡把数据放在能够高速写入的内存部分。
	*/

	//现在我们已经把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理。
	//下面我们会创建一个顶点着色器和片段着色器来真正处理这些数据。现在我们开始着手创建它们吧。

	//顶点着色器
	//着色器语言GLSL(OpenGL Shading Language)
	/*
	下面是一段非常基础的GLSL顶点着色器的源代码：
	#version 330 core
	layout (location = 0) in vec3 aPos;

	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	}

	GLSL看起来很像C语言。每个着色器都起始于一个版本声明。
	OpenGL 3.3以及和更高版本中，GLSL版本号和OpenGL的版本是匹配的
	（比如说GLSL 420版本对应于OpenGL 4.2）。
	我们同样明确表示我们会使用核心模式。

	下一步，使用in关键字创建输入变量，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。
	现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性。

	GLSL有一个向量数据类型vec，它包含1到4个float分量，包含的数量可以从它的后缀数字看出来。
	在GLSL中一个向量有最多4个分量，每个分量值都代表空间中的一个坐标，
	它们可以通过vec.x、vec.y、vec.z和vec.w来获取。
	由于每个顶点都有一个3D坐标，我们就创建一个vec3输入变量aPos。

	我们同样也通过layout (location = 0)设定了输入变量的位置值(Location)

	为了设置顶点着色器的输出，我们必须把位置数据赋值给预定义的gl_Position变量，它在幕后是vec4类型的。
	在main函数的最后，我们将gl_Position设置的值会成为该顶点着色器的输出。

	当前这个顶点着色器可能是我们能想到的最简单的顶点着色器了，
	因为我们对输入数据什么都没有处理就把它传到着色器的输出了。
	在真实的程序里输入数据通常都不是标准化设备坐标，
	所以我们首先必须先把它们转换至OpenGL的可视区域内，这个例子里没有体现。
	*/

	//编译着色器
	//我们暂时将顶点着色器的源代码硬编码在代码文件顶部的C风格字符串中
	//为了能够让OpenGL使用它，我们必须在运行时动态编译它的源代码。
	//我们首先要做的是创建一个着色器对象，注意还是用ID来引用的。
	//所以我们储存这个顶点着色器为unsigned int，然后用glCreateShader创建这个着色器：
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//我们把需要创建的着色器类型以参数形式提供给glCreateShader。
	//由于我们正在创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	//下一步我们把这个着色器源码附加到着色器对象上，然后编译它：
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//附加源码
	//glShaderSource函数把要编译的着色器对象作为第一个参数。
	//第二参数指定了传递的源码字符串数量，这里只有一个。
	//第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
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

	//创建片段着色器
	/*
	//片段着色器所做的是计算像素最后的颜色输出。
	//为了让事情更简单，我们的片段着色器将会一直输出橘黄色。
	在计算机图形中颜色被表示为有4个元素的数组：
	红色、绿色、蓝色和alpha(透明度)分量，通常缩写为RGBA。
	当在OpenGL或GLSL中定义一个颜色的时候，我们把颜色每个分量的强度设置在0.0到1.0之间。
	比如说我们设置红为1.0f，绿为1.0f，我们会得到两个颜色的混合色，即黄色。
	这三种颜色分量的不同调配可以生成超过1600万种不同的颜色！

	下面是一段非常基础的GLSL片段着色器的源代码：
	#version 330 core
	out vec4 FragColor;

	void main()
	{
		FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	}

	片段着色器只需要一个输出变量，这个变量是一个4分量向量，
	它表示的是最终的输出颜色，我们应该自己将其计算出来。
	声明输出变量可以使用out关键字，这里我们命名为FragColor。
	下面，我们将一个Alpha值为1.0(1.0代表完全不透明)的橘黄色的vec4赋值给颜色输出。
	*/
	//编译片段着色器的过程与顶点着色器类似，只不过我们使用GL_FRAGMENT_SHADER常量作为着色器类型：
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);//附加源码
	glCompileShader(fragmentShader);//编译


	//两个着色器现在都编译了，剩下的事情是把两个着色器对象链接到一个用来渲染的着色器程序(Shader Program)中。
	/*着色器程序对象(Shader Program Object)
	* 
	着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。
	如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，
	然后在渲染对象的时候激活这个着色器程序。
	已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
	
	当链接着色器至一个程序的时候，它会把每个着色器的输出链接到下个着色器的输入。
	当输出和输入不匹配的时候，你会得到一个连接错误。
	*/
	//创建着色器程序
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	//现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们：
	glAttachShader(shaderProgram, vertexShader);//顶点着色器附加
	glAttachShader(shaderProgram, fragmentShader);//片段着色器附加
	glLinkProgram(shaderProgram);//链接着色器

	//检测链接着色器程序情况
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
	}

	//激活着色器程
	glUseProgram(shaderProgram);
	//在glUseProgram函数调用之后，
	//每个着色器调用和渲染调用都会使用这个程序对象了。

	//删除着色器对象
	//在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了(sinimasai)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//设置顶点属性指针
	//现在，我们已经把输入顶点数据发送给了GPU，
	//并指示了GPU如何在顶点和片段着色器中处理它。
	//但还没结束，OpenGL还不知道它该如何解释内存中的顶点数据，
	//以及它该如何将顶点数据链接到顶点着色器的属性上。
	//我们必须在渲染前指定OpenGL该如何解释顶点数据。
	/*
	我们的顶点缓冲数据会被解析为下面这样子：
	| vector1 | vector2 | vector3 | ...
	| x  y  z | x  y  z | x  y  z | ...
	0         12        24        36     BYTE
	*位置数据被储存为32位（4字节）浮点值。
	*每个位置包含3个这样的值。
	*在这3个值之间没有空隙（或其他值）。这几个值在数组中紧密排列(Tightly Packed)。
	*数据中第一个值在缓冲开始的位置。
	
	有了这些信息我们就可以使用glVertexAttribPointer函数
	告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了：
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glVertexAttribPointer函数的参数非常多，所以我会逐一介绍它们：
	/*
	1.第一个参数指定我们要配置的顶点属性。
	  与顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)对应，
	  它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。

	2.第二个参数指定顶点属性的大小。
	  顶点属性是一个vec3，它由3个值组成，所以大小是3。

	3.第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。

	4.下个参数定义我们是否希望数据被标准化(Normalize)。
	  如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。
	  我们把它设置为GL_FALSE。

	5.第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。
	  由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
	  要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）
	  我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。
	  一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子
	 （译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。

	6.最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。
	  它表示位置数据在缓冲中起始位置的偏移量(Offset)。
	  由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
	*/
	glEnableVertexAttribArray(0);
	//现在我们已经定义了OpenGL该如何解释顶点数据，我们现在应该使用glEnableVertexAttribArray，
	//以顶点属性位置值作为参数，启用顶点属性0；顶点属性默认是禁用的。
	/*
	每个顶点属性从一个VBO管理的内存中获得它的数据，
	而具体是从哪个VBO（程序中可以有多个VBO）获取
	则是通过在调用glVertexAttribPointer时绑定到GL_ARRAY_BUFFER的VBO决定的。由于在调用glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据。
	*/

	//顶点数组对象VAO
	//VAO可以像VBO一样被绑定，任何随后的顶点属性调用都会储存在这个VAO中。
	//这样的好处就是，当配置顶点属性指针时，你只需要将那些调用执行一次，
	//之后再绘制物体的时候只需要绑定相应的VAO就行了。
	//这使得，在不同顶点数据和属性配置之间切换变得非常简单，
	//只需要绑定不同VAO就好，而设置的所有状态都将储存在VAO中
	/*
	OpenGL的核心模式要求我们使用VAO，所以它知道该如何处理我们的顶点输入。
	如果我们绑定VAO失败，OpenGL会拒绝绘制任何东西。

	一个顶点数组对象会储存以下这些内容：
	glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
	通过glVertexAttribPointer设置的顶点属性配置。
	通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
	*/

	//创建VAO对象(与创建VBO的方式很类似)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);//解绑只需要再次调用该函数，参数传入0
	//从绑定之后起，我们应该绑定和配置对应的VBO和属性指针，之后解绑VAO供之后使用。
	//当我们打算绘制一个物体的时候，我们只要在绘制物体前简单地把VAO绑定到希望使用的设定上就行了。

	//在这之后，便可以绘制图元了
	glDrawArrays(GL_TRIANGLES, 0, 3);
	/*
	glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型。
	第二个参数指定了顶点数组的起始索引，我们这里填0。
	最后一个参数指定我们打算绘制多少个顶点，这里是3
	（我们只从我们的数据中渲染一个三角形，它只有3个顶点长）
	*/
	
	//元素缓冲对象(Element Buffer Object，EBO)
	//也叫索引缓冲对象IBO(Index Buffer Object，IBO)
	/*
	在渲染顶点这一话题上我们还有最后一个需要讨论的东西：
	EBO或IBO
	
	 EBO是一个缓冲区，就像一个顶点缓冲区对象一样VBO，
	 它存储 OpenGL 用来决定要绘制哪些顶点的索引。
	 从而进行索引绘制。

	 什么是索引绘制？它有什么用？这里以绘制一个矩形为例，
	 在openGl中通常没有矩形定义，这就需要我们绘制两个三角形来组成矩形。
	 可以发现，在普通的VBO中，绘制两个三角形需要6个顶点，
	 其中有两个顶点是重复的，这就浪费了资源。
	 此时，我们可以只储存矩形的4个顶点，再通过给这4个顶点标号，
	 利用标号(即索引)来重复利用其中需要多次使用的点。
	*/
	float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
	};

	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
//创建EBO
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//绑定EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//通过EBO将索引数据复制到缓冲
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);	

	//当使用索引绘制时，相应的绘制函数也要改变
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	/*
	第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
	第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
	第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
	最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），这里填写0
	*/

	/*
	好了，这下穿起来了，
	VBO提供顶点缓冲，
	VAO提供顶点配置，
	EBO提供顶点索引。
	
	它们共用glBindBuffer()函数，
	如果同时使用GL_ELEMENT_ARRAY_BUFFER为绑定目标，
	则三者在绑定解绑时相互影响，通过函数记录各自是否与自身绑定，
	因此在绑定或解绑时要考虑各个对象的绑定情况。
	
	绑定的先后顺序是：VAO VBO EBO 
	缓冲的先后顺序是：VBO EBO
	解绑的先后顺序是：VAO VBO EBO
	*/

	//你会发现无法直接运行这个cpp文件，请将此文件从项目中排除
	//这是因为这是一个按照教学流程编写的教学代码
	//教学顺序没有完全按照程序需要的流程
	//接下来将总结程序流程，完整可执行代码将在main.cpp中


	//
	/*
		// ..:: 初始化代码 :: ..
		// 1. 绑定顶点数组对象
		glBindVertexArray(VAO);
		// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 4. 设定顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		[...]

		// ..:: 绘制代码（渲染循环中） :: ..
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	*/






	return 0;
}
