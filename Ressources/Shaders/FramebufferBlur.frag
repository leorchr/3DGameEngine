#version 430

out vec4 FragColor;
in vec2 texCoords;

uniform sampler2D screenTexture;
uniform vec2 texelSize;
const float coeff[5][5] = {
{1, 4, 6, 4, 1},
{4, 16, 24, 16, 4},
{6, 24, 36, 24, 6},
{4, 16, 24, 16, 4},
{1, 4, 6, 4, 1}
};

void main(){

    vec4 color = vec4(0.0);

    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            color += texture(screenTexture, texCoords + texelSize * vec2(i, j)) * coeff[i + 2][j + 2];
        }
    }

    color = color/256;
    
    FragColor = color;
}