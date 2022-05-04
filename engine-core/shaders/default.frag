#version 330 core

out vec4 FragColor;

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;

    int illumType;
    float opacity;
}; 

in vec2 AmbientTexCoord;
in vec2 DiffuseTexCoord;
in vec2 SpecularTexCoord;

in vec3 Normal;
in vec3 FragPos;

uniform bool hasAmbientTex;
uniform bool hasDiffuseTex;
uniform bool hasSpecularTex;

uniform bool hasLight;

uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;

void main()
{

    vec4 ambient = material.ambient * vec4(lightColor, 1.0);
    if (hasAmbientTex == true)
    {
        ambient *= texture(ambientTexture, AmbientTexCoord);
    }

    float diff;
    vec3 lightDir;
    vec3 norm = normalize(Normal);

    if (hasLight)
    {
        lightDir = normalize(lightPos - FragPos);
        diff = max(dot(norm, lightDir), 0.0);
    }
    else
    {
        diff = 1.0;
    }
    
    vec4 diffuse = vec4(lightColor, 1.0) * (diff * material.diffuse);
    if (hasDiffuseTex == true)
    {
        diffuse *= texture(diffuseTexture, DiffuseTexCoord);
    }

    float spec;
    vec3 viewDir = normalize(viewPos - FragPos);
    
    if (hasLight)
    {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    }
    else
    {
        spec = 1.0;
    }

    vec4 specular = vec4(lightColor, 1.0) * (spec * material.specular);
    if (hasSpecularTex == true)
    {
        specular *= texture(specularTexture, SpecularTexCoord);
    }

    vec4 result;
    switch (material.illumType)
    {
        case 1:
            result = diffuse;
            break;
        case 2:
            result = ambient + diffuse;
            break;
        case 3:
            result = ambient + diffuse + specular;
            break;
    }
    FragColor = result;
}