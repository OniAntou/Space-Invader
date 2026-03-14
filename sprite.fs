#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{    
    // Lấy màu của pixel từ bức ảnh
    FragColor = texture(image, TexCoords);
    
    // Bỏ discard để test
    // if(FragColor.a < 0.1)
    //     discard;
}