#version 330 core

//�����G��������
layout (location = 0) out vec3 gPosition;  //λ������
layout (location = 1) out vec3 gNormal;    //��������
layout (location = 2) out vec4 gColorSpec; //��ɫ+���淴��ǿ������

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{           
    // �洢Ƭ��λ�õ�G����������
	gPosition = FragPos;
	// �洢Ƭ�η��ߵ�G����������
	gNormal = normalize(Normal);
	// �洢Ƭ����������ɫ��G����������
	gColorSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
	// �洢����ǿ�ȵ�G����������
	gColorSpec.a = texture(texture_specular1, TexCoords).r;
}