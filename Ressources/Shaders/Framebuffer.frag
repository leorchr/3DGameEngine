#version 330 core

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

void main(){
    vec4 color = texture(screenTexture, texCoords);
    vec4 invert = vec4(1.0-color.x, 1.0-color.y, 1.0-color.z, color.a);
    FragColor = invert;
}