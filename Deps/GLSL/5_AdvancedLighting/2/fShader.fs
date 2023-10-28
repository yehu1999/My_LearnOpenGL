#version 330 core
out vec4 FragColor;

in VS_OUT 
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D floorTexture;
uniform vec3 lightPos[4];
uniform vec3 viewPos;
uniform bool Blinn;
uniform bool gamma;

void main()
{           
    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.01 * color;
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lightPos[i] - fs_in.FragPos);
        vec3 normal = normalize(fs_in.Normal);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * vec3(0.5);
        // specular
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        if(Blinn)
        {
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        }
        else
        {
            vec3 reflectDir = reflect(-lightDir, normal);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
        }
        vec3 specular = vec3(1) * spec; // assuming bright white light color

        // simple attenuation
        float max_distance = 1.5;
        float distance = length(lightPos[i] - fs_in.FragPos);
        float attenuation = 1.0 / (gamma ? distance * distance : distance);
        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
    }
    lighting += ambient;
    color *= lighting;

    if(gamma)
        color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);

   
}