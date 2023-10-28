#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    vec4 texColor = texture(texture1, TexCoords);   
    
    //设定alpha值处理
    //if(texColor.a < 0.1)//低于阈值的片段丢弃
        //discard;

    FragColor = texColor;
}