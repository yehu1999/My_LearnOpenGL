#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT
{
	vec3 color;
} gs_in[];

out vec3 fColor;

void build_house(vec4 position)
{
	fColor = gs_in[0].color;                                // gs_in[0] 因为只有一个输入顶点

	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
	EmitVertex();                                           //可以发现，每次Emit发射的顶点将包括所有现在已定义的out值(比如这里的gl_Position和fColor)
	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
	EmitVertex();
	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
	EmitVertex();
	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
	EmitVertex();
	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
	fColor = vec3(1.0, 1.0, 1.0);                           //将最后一个顶点的颜色设置为白色，给屋顶落上一些雪。
	EmitVertex();
	EndPrimitive();
}

void main() {
	build_house(gl_in[0].gl_Position);
}