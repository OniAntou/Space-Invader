#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// Cần GLEW để sử dụng các hàm OpenGL hiện đại
#include <GL/glew.h> 
#include <GL/glut.h>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID; // ID của chương trình Shader

    // Constructor đọc và xây dựng shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Copy constructor
    Shader(const Shader& other);

    // Copy assignment
    Shader& operator=(const Shader& other);

    // Destructor
    ~Shader();

    // Kích hoạt shader
    void use();

    // Các hàm bổ trợ để truyền dữ liệu Uniform
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    // Kiểm tra lỗi biên dịch
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif