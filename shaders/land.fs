#version 330 core

struct Material {
    vec3 color;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
    float inCutOff;
    float outCutOff;

    float constant;
    float linear;
    float quadratic;
};

uniform DirLight dlight;
uniform SpotLight slight;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    float dist = length(FragPos-viewPos);
    
    vec3 result = CalcDirLight(dlight,norm,viewDir);
    result += CalcSpotLight(slight,norm,FragPos,viewDir);
    result -= vec3(1.0f) * dist*dist/1000.0f;
    result = vec3(clamp(result.x,0.0f,1.0f),clamp(result.y,0.0f,1.0f),clamp(result.z,0.0f,1.0f));
    FragColor = vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    vec3 ambient =  light.ambient  * material.color;
    vec3 diffuse =  light.diffuse  * diff * material.color;
    vec3 specular = light.specular * spec * material.color;

    return ambient+diffuse;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance*distance));

    vec3 ambient  = light.ambient  * material.color;
    vec3 diffuse  = light.diffuse  * diff * material.color;
    vec3 specular = light.specular * spec * material.color;
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal,lightDir),0.0);
    float distance = length(light.position-fragPos);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outCutOff) / (light.inCutOff-light.outCutOff), 0, 1.0);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance*distance));

    vec3 ambient  = light.ambient  * material.color;
    vec3 diffuse  = light.diffuse  * diff * material.color;
    vec3 specular = light.specular * spec * material.color;

    return ambient + (diffuse + specular) * intensity;
}