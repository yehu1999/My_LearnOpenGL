//���ļ�Ϊ�̳̣�����ִ�У�����������ų����ļ���ȥ����main.cpp
#include<mythings/shader.h>
#include"stb_image.h"

const char* vsPath = "E:\\Learn\\Deps\\GLSL\\4\\vShader_.vs";
const char* fsPath = "E:\\Learn\\Deps\\GLSL\\4\\fShader_.fs";


int main()
{
	Shader ourShader(vsPath, fsPath);
	//���ٸ����̳��е���������֪ʶ����Ҫʱ�Լ����ĸ�ϰ��ֱ�ӽ�������
	//GLM����ר��ΪOpenGL����������ѧ��
	//GLM��OpenGL Mathematics����д������һ��ֻ��ͷ�ļ��Ŀ⣬
	//Ҳ����˵����ֻ�������Ӧ��ͷ�ļ������ˣ��������Ӻͱ��롣
	//ע��
	/*
	GLM���0.9.9�汾��Ĭ�ϻὫ�������ͳ�ʼ��Ϊһ�����������Ԫ�ؾ�Ϊ0����
	�����ǵ�λ���󣨶Խ�Ԫ��Ϊ1������Ԫ��Ϊ0����
	�����ʹ�õ���0.9.9��0.9.9���ϵİ汾������Ҫ�����еľ����ʼ����Ϊ glm::mat4 mat = glm::mat4(1.0f)��
	��������뱾�̵̳Ĵ��뱣��һ�£�
	��ʹ�õ���0.9.9�汾��GLM�����߸������������ʼ�����еľ���
	*/
	//������Ҫ��GLM�Ĵ�������ܶ����Դ�������3��ͷ�ļ����ҵ���
	/*
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/type_ptr.hpp>
	*/
	//��������ֱ�ӽ��ܼ��ֱ任Transformation

	//���任��ԭʼ��������(1,0,0)
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//����¼��任���ݵĵ�λ����(¼�����ݼ������Ӧ�ı任����)
	glm::mat4 trans;

	//λ��Translate
	//λ��(1,1,0)
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//��תRotate
	//��z����ʱ����ת90��
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//����Scale
	//��ÿ�������᷽�����0.5������
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	//GLM���Զ���������ˣ����صĽ����һ�������˶���任�ı任����
	//���б任(�൱����λ������ת������)
	vec = trans * vec;

	//��һ�������ǣ���ΰѾ��󴫵ݸ���ɫ����
	//������ǰ����ᵽ��GLSL��Ҳ��һ��mat4���͡�
	//�������ǽ��޸Ķ�����ɫ���������һ��mat4��uniform������
	//Ȼ�����þ���uniform����λ��������
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
	//�ڰ�λ����������gl_Position֮ǰ�����������һ��uniform mat4�����ҽ�����任������ˡ�
	//��Ȼ����������Ҫ�ѱ任���󴫵ݸ���ɫ����
	unsigned int transformLocation = glGetUniformLocation(ourShader.ID, "transform");
	//�������Ȳ�ѯuniform�����ĵ�ַ��Ȼ������Matrix4fv��׺��glUniform�����Ѿ������ݷ��͸���ɫ����
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(trans));
	/*
	��һ������������Ӧ�ú���Ϥ�ˣ�����uniform��λ��ֵ��

	�ڶ�����������OpenGL���ǽ�Ҫ���Ͷ��ٸ�����������1��

	����������ѯ�������Ƿ�ϣ�������ǵľ������ת��(Transpose)��
		Ҳ����˵�������Ǿ�����к��С�OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�
		����������(Column-major Ordering)���֡�GLM��Ĭ�ϲ��־���������
		���Բ�����Ҫת�þ���������GL_FALSE��

	���һ�������������ľ������ݣ�����GLM�����ǰ����ǵľ��󴢴�ΪOpenGL��ϣ�����ܵ����֣�
		�������Ҫ����GLM���Դ��ĺ���value_ptr���任��Щ���ݡ�
	*/
	//���������������Ͻ�Shader���setMat4fv()������,��Ϊ��
	ourShader.setMat4fv("transform", trans, 1, GL_FALSE);

	//Ҫ����������ʱ��������ת�����Ǳ�������Ϸѭ���и��±任����
	//��Ϊ����ÿһ����Ⱦ�����ж�Ҫ���¡�
	//����ʹ��GLFW��ʱ�亯������ȡ��ͬʱ��ĽǶȣ�
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	return 0;
}