#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = aPos;  //天空盒的位置坐标也是纹理坐标
	vec4 pos = projection * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww;  //xyww即z=w，这使得深度测试始终为最大值1
}