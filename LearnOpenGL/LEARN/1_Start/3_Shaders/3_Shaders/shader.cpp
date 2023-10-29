#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//1.���ļ�·���л�ȡ����/Ƭ����ɫ��Դ���ļ�
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//��֤ifstream�����׳��쳣
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.clear();
		fShaderFile.clear();
		//���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		//��ȡ�ļ��Ļ������ݵ�������
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//�ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		//ת�����ݵ�string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	//ת��Ϊc����ַ���
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	
	//������ɫ��
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);//����
	checkCompileErrors(vertex, "VERTEX");	//������
	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);//����
	checkCompileErrors(fragment, "FRAGMENT");	//������

	//������ɫ������
	ID = glCreateProgram();
	//������ɫ��
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);//����
	checkCompileErrors(ID, "PROGRAM");	//���Ӽ��

	//ɾ����ɫ��(������Ͳ���Ҫ��)
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(ID);
}
void Shader::del()
{
	glDeleteProgram(ID);
}
void Shader::setBool(const std::string& name, bool value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec4f(const std::string& name, float v1, float v2, float v3, float v4)const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
}
void Shader::setVec3f(const std::string& name, float v1, float v2, float v3)const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
}
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

}