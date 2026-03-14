#include "resourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>

// Khởi tạo các biến tĩnh
std::unordered_map<std::string, std::shared_ptr<Shader>>  ResourceManager::Shaders;

std::shared_ptr<Shader> ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string name) {
    Shaders[name] = std::make_shared<Shader>(vShaderFile, fShaderFile);
    return Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

void ResourceManager::Clear() {
    // Xóa tất cả Shader (destructor sẽ xử lý glDeleteProgram)
    Shaders.clear();
}
