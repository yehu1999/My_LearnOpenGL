#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

//��Ӱ���㺯��
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    //��Ӱǿ��
    float shadow = 0.0;

    //͸�ӳ���  (����ռ�����ת��Ϊ��׼���豸����NDC[-1,1])
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    //������任��[0,1] ����ΪҪ�������ͼ����ȱȽϣ�ͳһ�ȽϷ�Χ��
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)   //������ֵ�������ֵ1.0(˵������Զƽ��)����û����Ӱ
        shadow = 0.0;
    else
    {
        //�õ���Դ�ӽ�����������
        //float closestDepth = texture(shadowMap, projCoords.xy).r; 

        //�õ�Ƭ�εĵ�ǰ���
        float currentDepth = projCoords.z;

        //�ж�Ƭ���Ƿ�����Ӱ��
        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); //ƫ��ֵ:���ݱ��泯����ߵĽǶȸ��ģ���Ӱʧ��У����
        //shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;     
   
        //PCF��Ӱ�ữ  :(�������������ܶ������ͼ������Ȼ��ѽ��ƽ������)
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0); //һ�����صĴ�С {textureSize(shadowMap, 0)���������ͼ��0��mipmap�Ŀ��}
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                //������ǰƬ����Χ9�����ص���Ӱֵ
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;  //��ƽ��
    }
                   
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
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);  
    shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}