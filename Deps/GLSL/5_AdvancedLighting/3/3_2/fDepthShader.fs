#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{             
	//获取光源与片段间距离
	float lightDistance = length(FragPos.xyz - lightPos);

	//通过/far将距离统一到[0,1]
	lightDistance = lightDistance / far_plane;

	//将这个距离作为深度写入片段
	gl_FragDepth = lightDistance;
}
