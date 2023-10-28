#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

//���ʲ���
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

//IBL
uniform samplerCube irradianceMap;

//���ղ���
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
//�����λ��
uniform vec3 camPos;
//��ѧ����
const float PI = 3.14159265359;

//���߷ֲ�����
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
//�����ڱκ���
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
//����������
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ----------------------------------------------------------------------------
void main()
{		
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    //������������ʣ���������ʱ�ķ����ʣ�
    vec3 F0 = vec3(0.04);             //�ǽ���ʹ��0.04����������(��ɫ)
    F0 = mix(F0, albedo, metallic);   //��������ݽ����ȶ�F0��albedo���в�ֵ�õ���������ɫ

    //�����ʷ���
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i) 
    {
        //����ÿ�����ߵķ�����
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        //Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        //kS(���淴����)����Fresnel
        vec3 kS = F;
        //kD(��������)����1 - kS �������غ㣩
        vec3 kD = vec3(1.0) - kS;
        //���ƽ�����   (������Ϊ1����������)
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        //���շ�����radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
        //��ע�⣬�����Ѿ��� BRDF ���Է����� ��kS����������ǲ����ٴγ��� kS
    }   
    
    //ambient lighting  (we now use IBL as the ambient term)
    vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    vec3 ambient = (kD * diffuse) * ao;

    vec3 color = ambient + Lo;

    //HDRɫ��ӳ��
    color = color / (color + vec3(1.0));
    //gammaУ��
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}