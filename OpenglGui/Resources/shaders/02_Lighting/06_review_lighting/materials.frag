#version 330 core
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

uniform bool useAmbient;
uniform bool useDiffuse;
uniform bool useSpecular;
void main()
{
    //环境光
    vec3 ambient = material.ambient * light.ambient;
    //漫反射光
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;
    //镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，是从片段指向光源（由先前我们计算lightDir向量时，减法的顺序决定）。
    //为了保证我们得到正确的reflect向量，我们通过对lightDir向量取反来获得相反的方向。
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec;
    if(!useAmbient){
        ambient = vec3(0.0);
    }
    if(!useDiffuse){
        diffuse = vec3(0.0);
    }
    if(!useSpecular){
        specular = vec3(0.0);
    }
    //没有漫反射和镜面时，调高环境光的作用
    //if(!useDiffuse&&!useSpecular){ 
    //    ambient = material.ambient;
    //}
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
