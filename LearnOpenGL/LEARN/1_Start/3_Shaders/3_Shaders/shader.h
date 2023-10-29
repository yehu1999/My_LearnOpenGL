//part3着色器封装
#ifndef SHADER_H
#define SHADER_H
//这里，我们在头文件顶部使用了几个预处理指令(Preprocessor Directives)。
//这些预处理指令会告知你的编译器只在它没被包含过的情况下才包含和编译这个头文件，
//即使多个文件都包含了这个着色器头文件。它是用来防止链接冲突的。

#include <glad/glad.h>; // 包含glad来获取所有的必须OpenGL头文件
#include<GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//编写、编译、管理着色器是件麻烦事。
//在着色器主题的最后，我们会写一个类来让我们的生活轻松一点，
//它可以从硬盘读取着色器，然后编译并链接它们，并对它们进行错误检测，
//这就变得很好用了。这也会让你了解该如何封装目前所学的知识到一个抽象对象中。

class Shader
{
public:
	//着色器程序ID
	unsigned int ID;
	//构造函数读取并构建着色器
	Shader(const char* vertexPath, const char* fragmentPath);
	//激活着色器程序函数
	void use();
	//删除着色器
	void del();
	//uniform工具函数
	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setVec4f(const std::string& name, float v1, float v2, float v3, float v4)const;
	void setVec3f(const std::string& name, float v1, float v2, float v3)const;
	/*
	着色器类储存了着色器程序的ID。
	它的构造器需要顶点和片段着色器源代码的文件路径，这样我们就可以把源码的文本文件储存在硬盘上了。
	除此之外，为了让我们的生活更轻松一点，还加入了一些工具函数：
	use用来激活着色器程序，所有的set…函数能够查询一个unform的位置值并设置它的值。
	*/

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif