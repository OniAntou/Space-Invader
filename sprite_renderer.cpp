#include "sprite_renderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader) : shader(shader) {
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    this->shader.use();
    this->shader.setMat4("projection", projection);
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(const Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    // 1. Chuẩn bị các ma trận biến đổi
    this->shader.use();
    glm::mat4 model = glm::mat4(1.0f);

    // Di chuyển (Translation) - Xử lý position là tâm đối tượng
    model = glm::translate(model, glm::vec3(position - 0.5f * size, 0.0f));  

    // Xoay (Rotation) - Xoay quanh tâm của đối tượng
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    // Co giãn (Scaling)
    model = glm::scale(model, glm::vec3(size, 1.0f)); 

    // 2. Gửi ma trận và màu sắc vào Shader
    this->shader.setMat4("model", model);
    this->shader.setVec3("spriteColor", color);
    this->shader.setInt("image", 0);

    // 3. Vẽ Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.ID);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
    // Định nghĩa các đỉnh của một hình vuông (quad) sử dụng TRIANGLE_STRIP
    unsigned int VBO;
    float vertices[] = { 
        // Vị trí     // Texture Coords
        0.0f, 1.0f,   0.0f, 1.0f,  // top-left
        1.0f, 1.0f,   1.0f, 1.0f,  // top-right
        0.0f, 0.0f,   0.0f, 0.0f,  // bottom-left
        1.0f, 0.0f,   1.0f, 0.0f   // bottom-right
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}