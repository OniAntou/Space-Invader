#ifndef MODEL_RENDERER_H
#define MODEL_RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

enum class ModelType {
    SPACESHIP,
    BULLET,
    ENEMY
};

class ModelRenderer {
public:
    // Constructor khởi tạo shader
    ModelRenderer(Shader& shader);
    // Destructor giải phóng VAO/VBO
    ~ModelRenderer();

    // Hàm vẽ chính
    void DrawModel(ModelType type, glm::vec2 position, 
                    glm::vec2 size = glm::vec2(10.0f, 10.0f), 
                    float rotate = 0.0f, 
                    glm::vec3 color = glm::vec3(1.0f));

private:
    Shader& shader; 
    unsigned int VAO[3];
    unsigned int VBO[3];
    int vertexCount[3];

    // Khởi tạo dữ liệu các đỉnh
    void initSpaceshipData();
    void initBulletData();
    void initEnemyData();
};

#endif
