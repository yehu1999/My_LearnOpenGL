#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;


uniform vec3 cameraPos;
uniform samplerCube skybox;


void main()
{    
	//反射
	/*
    vec3 I = normalize(Position - cameraPos);        //观察方向
	vec3 R = reflect(I, normalize(Normal));          //反射方向
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
	*/

	//折射
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);

}