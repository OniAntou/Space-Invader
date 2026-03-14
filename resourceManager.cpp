#include "resourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Khởi tạo các biến tĩnh
std::unordered_map<std::string, Texture> ResourceManager::Textures;
std::unordered_map<std::string, std::shared_ptr<Shader>>  ResourceManager::Shaders;

std::shared_ptr<Shader> ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name) {
    Shaders[name] = std::make_shared<Shader>(vShaderFile, fShaderFile);
    return Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture ResourceManager::LoadTexture(const char* file, bool alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

void ResourceManager::Clear() {
    // Xóa tất cả Shader (destructor sẽ xử lý glDeleteProgram)
    Shaders.clear();
    // Xóa tất cả Texture
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
    Textures.clear();
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
    Texture texture;
    int width, height, nrChannels;
    
    // Flip textures vertically on load so that the origin matches OpenGL's bottom-left convention.
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

    if (data) {
        texture.Width = width;
        texture.Height = height;
        
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glGenTextures(1, &texture.ID);
        glBindTexture(GL_TEXTURE_2D, texture.ID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Cấu hình tham số Texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture at: " << file << std::endl;
    }
    return texture;
}