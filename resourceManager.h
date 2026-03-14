#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <GL/glew.h> // Thay đổi: Cần GLEW cho Shader
#include <GL/glut.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "shader.h" // Thêm: Include file shader.h bạn đã tạo

class Texture {
public:
    GLuint ID;
    int Width, Height, Channels;
    Texture() : ID(0), Width(0), Height(0), Channels(0) {}
};

class ResourceManager {
public:
    // Quản lý tài nguyên
    static std::unordered_map<std::string, std::shared_ptr<Shader>>  Shaders;
    static std::unordered_map<std::string, Texture> Textures;

    // Shader: Tải và truy xuất
    static std::shared_ptr<Shader> LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name);
    static std::shared_ptr<Shader> GetShader(std::string name);

    // Giải phóng bộ nhớ
    static void Clear();

private:
    ResourceManager() {}
    static Texture loadTextureFromFile(const char* file, bool alpha);
};

#endif