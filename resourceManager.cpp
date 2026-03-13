#include "resourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glut.h>
#include <iostream>

std::unordered_map<std::string, Texture> ResourceManager::Textures;

Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

void ResourceManager::Clear() {
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
    Texture texture;
    if (alpha) {
        texture.Channels = 4; // Assuming RGBA
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(file, &texture.Width, &texture.Height, &texture.Channels, 0);
        if (data) {
            glGenTextures(1, &texture.ID);
            glBindTexture(GL_TEXTURE_2D, texture.ID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
    } else {
        texture.Channels = 3; // RGB
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(file, &texture.Width, &texture.Height, &texture.Channels, 0);
        if (data) {
            glGenTextures(1, &texture.ID);
            glBindTexture(GL_TEXTURE_2D, texture.ID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.Width, texture.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
    }
    return texture;
}
