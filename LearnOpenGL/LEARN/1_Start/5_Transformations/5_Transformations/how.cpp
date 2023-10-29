//本文件为教程，不可执行，请从生成中排除本文件，去运行main.cpp
#include<mythings/shader.h>
#include"stb_image.h"

const char* vsPath = "E:\\Learn\\Deps\\GLSL\\4\\vShader_.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\4\\fShader_.fs";


int main()
{
	Shader ourShader(vsPath, fsPath);
	//不再复述教程中的向量矩阵知识，需要时自己查阅复习，直接进入正题
	//GLM——专门为OpenGL量身定做的数学库
	//GLM是OpenGL Mathematics的缩写，它是一个只有头文件的库，
	//也就是说我们只需包含对应的头文件就行了，不用链接和编译。
	//注意
	/*
	GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0），
	而不是单位矩阵（对角元素为1，其它元素为0）。
	如果你使用的是0.9.9或0.9.9以上的版本，你需要将所有的矩阵初始化改为 glm::mat4 mat = glm::mat4(1.0f)。
	如果你想与本教程的代码保持一致，
	请使用低于0.9.9版本的GLM，或者改用上述代码初始化所有的矩阵。
	*/
	//我们需要的GLM的大多数功能都可以从下面这3个头文件中找到：
	/*
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	*/
	//下面我们直接介绍几种变换Transformation

	//待变换的原始坐标向量(1,0,0)
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//即将录入变换数据的单位矩阵(录入数据即变成相应的变换矩阵)
	glm::mat4 trans;

	//位移Translate
	//位移(1,1,0)
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//旋转Rotate
	//绕z轴逆时针旋转90度
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//缩放Scale
	//对每个坐标轴方向进行0.5倍缩放
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	//GLM会自动将矩阵相乘，返回的结果是一个包括了多个变换的变换矩阵。
	//进行变换(相当于先位移再旋转再缩放)
	vec = trans * vec;

	//下一个问题是：如何把矩阵传递给着色器？
	//我们在前面简单提到过GLSL里也有一个mat4类型。
	//所以我们将修改顶点着色器让其接收一个mat4的uniform变量，
	//然后再用矩阵uniform乘以位置向量：
	/*
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTexCoord;

	out vec2 TexCoord;

	uniform mat4 transform;

	void main()
	{
		gl_Position = transform * vec4(aPos, 1.0f);
		TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
	}
	*/
	//在把位置向量传给gl_Position之前，我们先添加一个uniform mat4，并且将其与变换矩阵相乘。
	//当然，我们仍需要把变换矩阵传递给着色器：
	unsigned int transformLocation = glGetUniformLocation(ourShader.ID, "transform");
	//我们首先查询uniform变量的地址，然后用有Matrix4fv后缀的glUniform函数把矩阵数据发送给着色器。
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
	/*
	第一个参数你现在应该很熟悉了，它是uniform的位置值。

	第二个参数告诉OpenGL我们将要发送多少个矩阵，这里是1。

	第三个参数询问我们是否希望对我们的矩阵进行转置(Transpose)，
		也就是说交换我们矩阵的行和列。OpenGL开发者通常使用一种内部矩阵布局，
		叫做列主序(Column-major Ordering)布局。GLM的默认布局就是列主序，
		所以并不需要转置矩阵，我们填GL_FALSE。

	最后一个参数是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种，
		因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
	*/
	//以上两步被我整合进Shader类的setMat4fv()函数中,简化为：
	ourShader.setMat4fv("transform", trans, 1, GL_FALSE);

	//要让箱子随着时间推移旋转，我们必须在游戏循环中更新变换矩阵，
	//因为它在每一次渲染迭代中都要更新。
	//我们使用GLFW的时间函数来获取不同时间的角度：
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	return 0;
}