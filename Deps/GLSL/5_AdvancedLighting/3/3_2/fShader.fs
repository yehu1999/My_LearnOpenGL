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

//用于PCF的偏移量数组
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

//阴影计算函数
float ShadowCalculation(vec3 fragPos)
{
    float shadow = 0.0;       //阴影强度

    int samples = 20;         //样本数

    //获取光源到片段的向量    //应用PCF后，该向量将在偏移后多次采样
	vec3 lightToFrag = fragPos - lightPos;

    //当前深度就是光源到片段的距离
	float currentDepth = length(lightToFrag);

    //偏移半径：根据观察者的距离来增加偏移半径了，当距离更远的时候阴影更柔和，更近了就更锐利
    float diskRadius = (1.0 + (currentDepth / far_plane)) / 25.0;  

    //PCF
    for(int i = 0; i < samples; ++i)
	{
	    //通过方向向量从深度立方体贴图中获取该片段的最近深度   
	    float closestDepth = texture(depthMap, lightToFrag + sampleOffsetDirections[i] * diskRadius).r;
	    //将深度值其从[0,1]范围转换到原始值
	    closestDepth *= far_plane;
	    //深度测试
	    float bias = 0.05;    //偏移值:根据表面朝向光线的角度更改（阴影失真校正）
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
    float shadow = shadows ? ShadowCalculation(fs_in.FragPos) : 0.0;  
    //shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);

    //debug：显示深度缓冲灰度图
    //vec3 lightToFrag = fs_in.FragPos - lightPos;
    //float closestDepth = texture(depthMap, lightToFrag).r;
    //FragColor = vec4(5.0 * vec3(closestDepth / far_plane), 1.0);
}