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
    //获取高度(实际为深度)
    float height =  texture(depthMap, texCoords).r;

    if(op == 0)       //近似视差映射
    {
        //近似求得uv偏移量
        vec2 p = viewDir.xy / viewDir.z * (height * height_scale);

        resultTexCoords = texCoords - p;
    }
    else if(op == 1 || op == 2)  //陡视差映射(1) || 视差遮蔽映射(2)
    {
        //采样层数 (观察表面的角度越大，采样率越大)
        const float minLayers = 8;
        const float maxLayers = 32;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
        //每层深度
        float layerDepth = 1.0 / numLayers;
        //当前所在层的深度
        float currentLayerDepth = 0.0;
        //在观察方向上，每层纹理坐标的移动量
        vec2 P = viewDir.xy * height_scale; 
        vec2 deltaTexCoords = P / numLayers;

        //初始值
        vec2  currentTexCoords     = texCoords;   //当前纹理坐标
        float currentDepthMapValue = height;      //当前纹理坐标的深度值

        while(currentLayerDepth < currentDepthMapValue)//直到找到第一个深度小于当前纹理坐标对应深度值退出循环
        {
            //沿着观察方向移动纹理坐标
            currentTexCoords -= deltaTexCoords;
            //获取当前纹理坐标的深度值
            currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
            //进入下一层，并更新当前所在层的深度
            currentLayerDepth += layerDepth;  
        }
        resultTexCoords = currentTexCoords;

        if(op == 2)   //视差遮蔽映射（修复断层问题）
        {
            //获取上一个纹理坐标
            vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

            //线性插值前后两层的深度
            float afterDepth  = currentDepthMapValue - currentLayerDepth;
            float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

            //纹理坐标的插值
            float weight = afterDepth / (afterDepth - beforeDepth); 
            resultTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
        }
    }
       
    return resultTexCoords; 
}

void main()
{           
    //视差贴图
    //通过视差贴图对纹理坐标进行偏移
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords, viewDir, option);
    //超出纹理范围的片段不采样
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
		discard;

    //法线贴图
    //从法线贴图获取范围为[0,1]的法线向量
    vec3 normal = texture(normalMap, texCoords).rgb;
    //将法线向量转换为范围[-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    //布林冯光照模型
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