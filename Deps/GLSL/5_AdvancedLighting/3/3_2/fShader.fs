#version 330 core
out vec4 FragColor;

in VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;
uniform bool shadows;

//����PCF��ƫ��������
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

//��Ӱ���㺯��
float ShadowCalculation(vec3 fragPos)
{
    float shadow = 0.0;       //��Ӱǿ��

    int samples = 20;         //������

    //��ȡ��Դ��Ƭ�ε�����    //Ӧ��PCF�󣬸���������ƫ�ƺ��β���
	vec3 lightToFrag = fragPos - lightPos;

    //��ǰ��Ⱦ��ǹ�Դ��Ƭ�εľ���
	float currentDepth = length(lightToFrag);

    //ƫ�ư뾶�����ݹ۲��ߵľ���������ƫ�ư뾶�ˣ��������Զ��ʱ����Ӱ����ͣ������˾͸�����
    float diskRadius = (1.0 + (currentDepth / far_plane)) / 25.0;  

    //PCF
    for(int i = 0; i < samples; ++i)
	{
	    //ͨ�����������������������ͼ�л�ȡ��Ƭ�ε�������   
	    float closestDepth = texture(depthMap, lightToFrag + sampleOffsetDirections[i] * diskRadius).r;
	    //�����ֵ���[0,1]��Χת����ԭʼֵ
	    closestDepth *= far_plane;
	    //��Ȳ���
	    float bias = 0.05;    //ƫ��ֵ:���ݱ��泯����ߵĽǶȸ��ģ���Ӱʧ��У����
	    if(currentDepth - bias > closestDepth)
			shadow += 1.0;
    }
    shadow /= float(samples);

	return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);

    //���ַ�ģ��
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    //������Ӱ
    float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;  
    //shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);

    //debug����ʾ��Ȼ���Ҷ�ͼ
    //vec3 lightToFrag = fs_in.FragPos - lightPos;
    //float closestDepth = texture(depthMap, lightToFrag).r;
    //FragColor = vec4(5.0 * vec3(closestDepth / far_plane), 1.0);
}