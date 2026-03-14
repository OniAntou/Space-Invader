#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "resourceManager.h"

class SpriteRenderer {
public:
    // Constructor khởi tạo shader
    SpriteRenderer(Shader& shader);
    // Destructor giải phóng VAO/VBO
    ~SpriteRenderer();

    // Hàm vẽ chính
    void DrawSprite(const Texture &texture, glm::vec2 position, 
                    glm::vec2 size = glm::vec2(10.0f, 10.0f), 
                    float rotate = 0.0f, 
                    glm::vec3 color = glm::vec3(1.0f));

private:
    Shader& shader; 
    unsigned int quadVAO;
    // Khởi tạo dữ liệu hình vuông (quad)
    void initRenderData();
};

#endif