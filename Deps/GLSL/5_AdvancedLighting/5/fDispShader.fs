#version 330 core
out vec4 FragColor;

in VS_OUT 
{
    vec2 TexCoords;
    vec3 FragPos;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float height_scale;
uniform int option;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, int op)
{
    
       vec2 resultTexCoords = texCoords;
    //��ȡ�߶�(ʵ��Ϊ���)
    float height =  texture(depthMap, texCoords).r;

    if(op == 0)       //�����Ӳ�ӳ��
    {
        //�������uvƫ����
        vec2 p = viewDir.xy / viewDir.z * (height * height_scale);

        resultTexCoords = texCoords - p;
    }
    else if(op == 1 || op == 2)  //���Ӳ�ӳ��(1) || �Ӳ��ڱ�ӳ��(2)
    {
        //�������� (�۲����ĽǶ�Խ�󣬲�����Խ��)
        const float minLayers = 8;
        const float maxLayers = 32;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
        //ÿ�����
        float layerDepth = 1.0 / numLayers;
        //��ǰ���ڲ�����
        float currentLayerDepth = 0.0;
        //�ڹ۲췽���ϣ�ÿ������������ƶ���
        vec2 P = viewDir.xy * height_scale; 
        vec2 deltaTexCoords = P / numLayers;

        //��ʼֵ
        vec2  currentTexCoords     = texCoords;   //��ǰ��������
        float currentDepthMapValue = height;      //��ǰ������������ֵ

        while(currentLayerDepth < currentDepthMapValue)//ֱ���ҵ���һ�����С�ڵ�ǰ���������Ӧ���ֵ�˳�ѭ��
        {
            //���Ź۲췽���ƶ���������
            currentTexCoords -= deltaTexCoords;
            //��ȡ��ǰ������������ֵ
            currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
            //������һ�㣬�����µ�ǰ���ڲ�����
            currentLayerDepth += layerDepth;  
        }
        resultTexCoords = currentTexCoords;

        if(op == 2)   //�Ӳ��ڱ�ӳ�䣨�޸��ϲ����⣩
        {
            //��ȡ��һ����������
            vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

            //���Բ�ֵǰ����������
            float afterDepth  = currentDepthMapValue - currentLayerDepth;
            float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

            //��������Ĳ�ֵ
            float weight = afterDepth / (afterDepth - beforeDepth); 
            resultTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
        }
    }
       
    return resultTexCoords; 
}

void main()
{           
    //�Ӳ���ͼ
    //ͨ���Ӳ���ͼ�������������ƫ��
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir, option);
    //��������Χ��Ƭ�β�����
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;

    //������ͼ
    //�ӷ�����ͼ��ȡ��ΧΪ[0,1]�ķ�������
    vec3 normal = texture(normalMap, texCoords).rgb;
    //����������ת��Ϊ��Χ[-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    //���ַ����ģ��
    //diffuse color
    vec3 color = texture(diffuseMap, texCoords).rgb;
    //Ambient
    vec3 ambient = 0.1 * color;
    //Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    //Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}