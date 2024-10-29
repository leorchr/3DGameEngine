#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 uViewProjection;
uniform mat4 uWorldTransform;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main()
{
    vec4 pos = vec4(inPosition, 1.0);
    fragWorldPos = pos.xyz;
    gl_Position = uViewProjection * uWorldTransform * pos;
    
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;
    fragTexCoord = inTexCoord;
}