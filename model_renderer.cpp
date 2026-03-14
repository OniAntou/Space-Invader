#include "model_renderer.h"

ModelRenderer::ModelRenderer(Shader &shader) : shader(shader) {
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    this->shader.use();
    this->shader.setMat4("projection", projection);
    
    // Generate arrays
    glGenVertexArrays(3, this->VAO);
    glGenBuffers(3, this->VBO);

    this->initSpaceshipData();
    this->initBulletData();
    this->initEnemyData();
}

ModelRenderer::~ModelRenderer() {
    glDeleteVertexArrays(3, this->VAO);
    glDeleteBuffers(3, this->VBO);
}

void ModelRenderer::DrawModel(ModelType type, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
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

    // 3. Lấy chỉ mục mảng dựa trên Enum
    int index = 0;
    if (type == ModelType::SPACESHIP) index = 0;
    else if (type == ModelType::BULLET) index = 1;
    else if (type == ModelType::ENEMY) index = 2;

    // 4. Vẽ Model
    glBindVertexArray(this->VAO[index]);
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount[index]);
    glBindVertexArray(0);
}

void ModelRenderer::initSpaceshipData() {
    float vertices[] = { 
        0.5f, 1.0f,  0.3f, 0.2f,  0.7f, 0.2f, // Thân chính
        0.3f, 0.2f,  0.0f, 0.0f,  0.5f, 0.4f, // Cánh trái
        0.7f, 0.2f,  0.5f, 0.4f,  1.0f, 0.0f  // Cánh phải
    };
    this->vertexCount[0] = 9; 

    glBindVertexArray(this->VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ModelRenderer::initBulletData() {
    // Vẽ đạn bằng một hình vuông (gồm 2 hình tam giác)
    // Tọa độ làm sao cho mỏng và thẳng đứng
    float vertices[] = { 
        0.4f, 1.0f,  0.4f, 0.0f,  0.6f, 0.0f, // Tam giác 1
        0.4f, 1.0f,  0.6f, 0.0f,  0.6f, 1.0f  // Tam giác 2
    };
    this->vertexCount[1] = 6;

    glBindVertexArray(this->VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

#include <vector>
void ModelRenderer::initEnemyData() {
    // Bản đồ pixel 11x8 của Crab Alien
    const char* pattern[] = {
        "  X     X  ",
        "   X   X   ",
        "  XXXXXXX  ",
        "XXXXXXXXXXX",
        "XXX XXX XXX",
        "X XXXXXXX X",
        "X X     X X",
        "   XX XX   "
    };

    int cols = 11;
    int rows = 8;
    std::vector<float> vertices;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (pattern[y][x] == 'X') {
                // OpenGL lật Y (0 ở dưới cùng, nhưng pattern thì 0 ở trên cùng)
                // Ta cần đổi trục Y: Y_GL = 1.0 - (y / rows)
                float x_left = (float)x / cols;
                float x_right = (float)(x + 1) / cols;
                float y_top = 1.0f - (float)y / rows;
                float y_bottom = 1.0f - (float)(y + 1) / rows;

                // Triangle 1
                vertices.push_back(x_left); vertices.push_back(y_top);
                vertices.push_back(x_left); vertices.push_back(y_bottom);
                vertices.push_back(x_right); vertices.push_back(y_bottom);
                // Triangle 2
                vertices.push_back(x_left); vertices.push_back(y_top);
                vertices.push_back(x_right); vertices.push_back(y_bottom);
                vertices.push_back(x_right); vertices.push_back(y_top);
            }
        }
    }

    this->vertexCount[2] = vertices.size() / 2; // Số đỉnh

    glBindVertexArray(this->VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
