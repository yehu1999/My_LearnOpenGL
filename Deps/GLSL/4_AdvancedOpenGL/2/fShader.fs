#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1; 
float far  = 100.0; 

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{    
    float depth = LinearizeDepth(gl_FragCoord.z) / far;

    //no1 深度测试函数 / no4 深度冲突
    FragColor = texture(texture1, TexCoords);

    //no2 深度缓冲的非线性可视化
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);  
    
    //no3 深度缓冲的线性可视化
    //FragColor = vec4(vec3(depth), 1.0);              

}