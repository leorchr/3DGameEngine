#version 430

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;

void main(){
    vec4 color = texture(screenTexture, texCoords);
    FragColor = color;
}