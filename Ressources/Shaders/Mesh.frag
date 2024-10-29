#version 330

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 fragColor;

struct DirectionalLight
{
    vec3 direction;
    vec3 diffuseColor;
    vec3 specColor;
};

uniform sampler2D gSampler;
uniform DirectionalLight uDirLight;
uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecPower;


void main()
{
    
    // ambient
    vec3 phong = uAmbientLight;
    
    // diffuse
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(uDirLight.direction);
    
    float diffuseStrength = max(0.0, dot(normal, -lightDirection));
    vec3 diffuse = diffuseStrength * uDirLight.diffuseColor;
    
    
    // specular
    vec3 cameraDir = normalize(uCameraPos - fragWorldPos);
    vec3 reflectSource = normalize(reflect(lightDirection,normal));
    float specularStrength = pow(max(0.0, dot(reflectSource, cameraDir)), uSpecPower);
    vec3 specular = specularStrength * uDirLight.specColor;
    
    // phong results
    phong += diffuse + specular;
    
    fragColor = texture(gSampler, fragTexCoord) * vec4(phong, 1.0f);
}