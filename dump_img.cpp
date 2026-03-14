#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

int main() {
    int width, height, channels;
    unsigned char *data = stbi_load("resource/enemy.png", &width, &height, &channels, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
        return 1;
    }
    
    int newW = 11;
    int newH = 8;
    for (int y = 0; y < newH; ++y) {
        for (int x = 0; x < newW; ++x) {
            int srcX = x * width / newW + (width / newW / 2);
            int srcY = y * height / newH + (height / newH / 2);
            int i = (srcY * width + srcX) * channels;
            int a = (channels == 4) ? data[i+3] : 255;
            if (a > 128) std::cout << "X";
            else std::cout << " ";
        }
        std::cout << std::endl;
    }
    stbi_image_free(data);
    return 0;
}
