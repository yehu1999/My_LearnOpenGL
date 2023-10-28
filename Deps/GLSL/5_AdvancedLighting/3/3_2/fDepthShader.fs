#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{             
	//��ȡ��Դ��Ƭ�μ����
	float lightDistance = length(FragPos.xyz - lightPos);

	//ͨ��/far������ͳһ��[0,1]
	lightDistance = lightDistance / far_plane;

	//�����������Ϊ���д��Ƭ��
	gl_FragDepth = lightDistance;
}
