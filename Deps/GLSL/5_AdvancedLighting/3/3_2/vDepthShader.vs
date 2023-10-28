#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(position, 1.0f);
}

//将顶点变换到光空间的任务交给了几何着色器