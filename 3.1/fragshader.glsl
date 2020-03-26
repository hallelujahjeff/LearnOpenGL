#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;
uniform vec3 viewPos;

struct Light
{
    float ambient;
    float diffuse;
    float specular;
    vec3 color;
    vec3 pos;
};

uniform Light light;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

//uniform float ambientStrength;	//环境光强度
//uniform float specularStength;	//镜面强度
//uniform int Shininess; //反光度

void main()
{
    //环境光
	vec3 ambient = light.ambient * light.color * material.ambient;

    // 漫反射 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * light.color * (diff * material.diffuse);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * light.color * (spec * material.specular);  

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}