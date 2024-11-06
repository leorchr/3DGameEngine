#version 430

layout (binding = 0) readonly uniform image2D tex;
layout (binding = 1) writeonly uniform image2D outputTex;

layout(local_size_x = 16, local_size_y = 16) in

void main() {
    
    // Récupérer l'index du pixel dans l'image
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);

    // Lire la couleur du pixel dans la texture d'entrée
    vec4 color = imageLoad(tex, pixelCoords);

    // Appliquer une opération sur la couleur (par exemple, inversion des couleurs)
    color = vec4(1.0) - color;

    // Écrire la couleur modifiée dans la texture de sortie
    imageStore(outputTex, pixelCoords, color);
}