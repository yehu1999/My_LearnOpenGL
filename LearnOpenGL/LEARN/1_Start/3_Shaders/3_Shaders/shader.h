//part3��ɫ����װ
#ifndef SHADER_H
#define SHADER_H
//���������ͷ�ļ�����ʹ���˼���Ԥ����ָ��(Preprocessor Directives)��
//��ЩԤ����ָ����֪��ı�����ֻ����û��������������²Ű����ͱ������ͷ�ļ���
//��ʹ����ļ��������������ɫ��ͷ�ļ�������������ֹ���ӳ�ͻ�ġ�

#include <glad/glad.h>; // ����glad����ȡ���еı���OpenGLͷ�ļ�
#include<GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
//��д�����롢������ɫ���Ǽ��鷳�¡�
//����ɫ�������������ǻ�дһ�����������ǵ���������һ�㣬
//�����Դ�Ӳ�̶�ȡ��ɫ����Ȼ����벢�������ǣ��������ǽ��д����⣬
//��ͱ�úܺ����ˡ���Ҳ�������˽����η�װĿǰ��ѧ��֪ʶ��һ����������С�

class Shader
{
public:
	//��ɫ������ID
	unsigned int ID;
	//���캯����ȡ��������ɫ��
	Shader(const char* vertexPath, const char* fragmentPath);
	//������ɫ��������
	void use();
	//ɾ����ɫ��
	void del();
	//uniform���ߺ���
	void setBool(const std::string& name, bool value)const;
	void setInt(const std::string& name, int value)const;
	void setFloat(const std::string& name, float value)const;
	void setVec4f(const std::string& name, float v1, float v2, float v3, float v4)const;
	void setVec3f(const std::string& name, float v1, float v2, float v3)const;
	/*
	��ɫ���ഢ������ɫ�������ID��
	���Ĺ�������Ҫ�����Ƭ����ɫ��Դ������ļ�·�����������ǾͿ��԰�Դ����ı��ļ�������Ӳ�����ˡ�
	����֮�⣬Ϊ�������ǵ����������һ�㣬��������һЩ���ߺ�����
	use����������ɫ���������е�set�������ܹ���ѯһ��unform��λ��ֵ����������ֵ��
	*/

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif