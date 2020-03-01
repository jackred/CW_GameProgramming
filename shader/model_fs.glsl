#version 430 core

out vec4 FragColor;

struct Material_s {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct PointLight_s {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight_s {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec3 fragPos;
in vec4 fragPosLightSpace;
in vec3 vertexNormal;

uniform vec3 viewPos;
uniform sampler2D shadowMap;

uniform Material_s material;
uniform PointLight_s lights[NR_POINT_LIGHTS];
uniform DirLight_s dirLight;

vec3 CalcDirLight(DirLight_s light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight_s light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(vertexNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //   result += CalcPointLight(lights[i], norm, fragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    //float bias = 0.005f;

    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    const int halfkernelWidth = 3;
    for(int x = -halfkernelWidth; x <= halfkernelWidth; ++x)
    {
        for(int y = -halfkernelWidth; y <= halfkernelWidth; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= ((halfkernelWidth*2+1)*(halfkernelWidth*2+1));
    return shadow;
}

vec3 CalcDirLight(DirLight_s light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(lightDir, normal), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    // float shadow = ShadowCalculation(fragPosLightSpace, normal, lightDir);

    return (ambient + diffuse + specular);
    // return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight_s light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
