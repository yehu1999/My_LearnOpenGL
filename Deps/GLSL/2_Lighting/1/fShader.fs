	#version 330 core
	out vec4 FragColor;

	uniform vec3 objectColor;
	uniform vec3 lightColor;

	void main()
	{
		//将光源的颜色和物体（反射的）颜色相乘
		FragColor = vec4(lightColor * objectColor, 1.0);
	}