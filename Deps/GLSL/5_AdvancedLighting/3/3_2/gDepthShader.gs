#version 330 core
layout (triangles) in;                            //每输入一个三角形
layout (triangle_strip, max_vertices=18) out;     //输出6个三角形(18个顶点)，立方体贴图的每个面各一个

uniform mat4 shadowMatrices[6];                   //光空间变换矩阵

out vec4 FragPos;                                 // FragPos from GS (output per emitvertex)

void main()
{
	for(int face = 0; face < 6; ++face)
	{
		gl_Layer = face;               // built-in variable that specifies to which face we render.
		for(int i = 0; i < 3; ++i)     // for each triangle's vertices
		{
			FragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * FragPos;   //变换到光空间坐标
			EmitVertex();
		}
		EndPrimitive();  //将顶点信息传到片段着色器，用来获取深度值
	}
}