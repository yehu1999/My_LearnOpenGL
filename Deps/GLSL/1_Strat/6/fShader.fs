#version 330 core
	out vec4 FragColor;

	//in vec3 ourColor;
	in vec2 TexCoord;

	uniform sampler2D texture1;
	uniform sampler2D texture2;
	uniform float play;

	void main()
	{
		FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), play);
		//0.2--->(80% texture1, 20% texture2)
	}