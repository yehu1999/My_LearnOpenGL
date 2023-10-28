	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal; //法向量数据

	out vec3 FragPos;
	out vec3 Normal;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0f);
		FragPos = vec3(model * vec4(aPos, 1.0));   //FragPos就是模型顶点在世界坐标下的位置
		Normal = mat3(transpose(inverse(model))) * aNormal;   //设置法向量矩阵，防止非等比缩放物体时破坏法向量(这一步消耗很大，实际上应该在CPU中完成计算再传入GPU)
	}