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

//阴影计算函数
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    //阴影强度
    float shadow = 0.0;

    //透视除法  (将光空间坐标转换为标准化设备坐标NDC[-1,1])
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    //将坐标变换到[0,1] （因为要和深度贴图的深度比较，统一比较范围）
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)   //如果深度值大于最大值1.0(说明超出远平面)，则没有阴影
        shadow = 0.0;
    else
    {
        //得到光源视角下最近的深度
        //float closestDepth = texture(shadowMap, projCoords.xy).r; 

        //得到片段的当前深度
        float currentDepth = projCoords.z;

        //判断片段是否在阴影中
        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); //偏移值:根据表面朝向光线的角度更改（阴影失真校正）
        //shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;     
   
        //PCF阴影柔化  :(从纹理像素四周对深度贴图采样，然后把结果平均起来)
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0); //一个像素的大小 {textureSize(shadowMap, 0)返回深度贴图的0级mipmap的宽高}
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                //采样当前片段周围9个像素的阴影值
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;  //求平均
    }
                   
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);

    //布林冯模型
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

    //计算阴影
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);  
    shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}