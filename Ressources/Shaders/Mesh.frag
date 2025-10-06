#version 430

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 fragColor;

struct PositionalLight
{
    vec3 position;
    vec3 diffuseColor;
    vec3 specColor;
};

uniform sampler2D gSampler;
uniform vec3 uAmbientLight;
uniform vec3 uCameraPos;
uniform PositionalLight uPositionalLight;
uniform float uSpecPower;


void main()
{
    
    // ambient
    vec3 phong = uAmbientLight;
    
    // diffuse
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(uPositionalLight.position - fragWorldPos);
    float diffuseStrength = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseStrength * uPositionalLight.diffuseColor;

    // specular
    vec3 specular = vec3(0);
    if(dot(-lightDirection, normal) < 0.0){
        vec3 viewDirection = normalize(uCameraPos - fragWorldPos);
        vec3 reflectDirection = normalize(reflect(-lightDirection,normal));
       
        float specularStrength = pow(max(dot(viewDirection,reflectDirection), 0.0), uSpecPower);
        specular = specularStrength * uPositionalLight.specColor;
    }
    
    // phong results
    phong += diffuse + specular;
    
    fragColor = texture(gSampler, fragTexCoord);// * vec4(phong, 1.0f);
}