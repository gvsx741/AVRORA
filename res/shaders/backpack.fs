#version 460 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal; 
in vec2 TexCoords;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform Light light;
uniform vec3 viewPos;

void main()
{  
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
  	
	// diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;  
    
    // specular	
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;  
    
    vec3 result = ambient + diffuse + specular; 
	
    FragColor = vec4(result, texture(texture_specular1, TexCoords).a);
}