	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal; //法向量数据
	layout (location = 2) in vec2 aTexCoords; //纹理坐标数据

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
	vs_out.FragPos = aPos;  //FragPos就是模型顶点在世界坐标下的位置
	vs_out.Normal = aNormal;   //设置法向量矩阵，防止非等比缩放物体时破坏法向量(这一步消耗很大，实际上应该在CPU中完成计算再传入GPU)
	vs_out.TexCoords = aTexCoords;
}