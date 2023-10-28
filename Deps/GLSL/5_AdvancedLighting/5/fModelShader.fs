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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;


void main()
{    
	//从法线贴图获取范围为[0,1]的法线向量
    vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
    //将法线向量转换为范围[-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

    //布林冯光照模型
    //diffuse color
    vec3 color = texture(texture_diffuse1, fs_in.TexCoords).rgb;
    //Ambient
    vec3 ambient = 0.1 * color;
    //Diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    //Specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.5) * spec;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}