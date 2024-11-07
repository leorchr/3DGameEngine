#version 430

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout (binding = 0, rgba8) uniform image2D inputImage;
layout (binding = 1, rgba8) uniform image2D outputImage;

void main()
{
    ivec2 coords = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(1.0,0.0,0.0,1.0);
    imageStore(outputImage, coords, color); // Écriture dans l'image de sortie
}