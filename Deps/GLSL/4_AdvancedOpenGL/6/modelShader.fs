#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
//下面这貌似两个没用上
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_reflect1;

void main()
{    
	vec3 ambient  = vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse  = vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = vec3(texture(texture_specular1, TexCoords));

	vec3 reflect = vec3(texture(texture_specular1, TexCoords));
	vec3 I = normalize(FragPos - viewPos);           //观察方向
	vec3 R = reflect(I, normalize(Normal));          //反射方向
	reflect = vec3(texture(skybox, R));

	//没找到具体文章，尝试几番，觉得将reflect值×在镜面反射上效果最好
	vec3 result = 0.5 * ambient + 0.5 * reflect * specular + diffuse;

	FragColor = vec4(result, 1.0);

}
