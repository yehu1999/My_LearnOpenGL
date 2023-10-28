#version 330 core

//输出的G缓冲纹理
layout (location = 0) out vec3 gPosition;  //位置纹理
layout (location = 1) out vec3 gNormal;    //法线纹理
layout (location = 2) out vec4 gColorSpec; //颜色+镜面反射强度纹理

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{           
    // 存储片段位置到G缓冲纹理中
	gPosition = FragPos;
	// 存储片段法线到G缓冲纹理中
	gNormal = normalize(Normal);
	// 存储片段漫反射颜色到G缓冲纹理中
	gColorSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
	// 存储镜面强度到G缓冲纹理中
	gColorSpec.a = texture(texture_specular1, TexCoords).r;
}