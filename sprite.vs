#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    // vertex.zw chứa tọa độ Texture (Texture Coordinates)
    TexCoords = vertex.zw;
    
    // vertex.xy chứa tọa độ không gian (Position)
    // Nhân với ma trận model và projection
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}