#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




class DrawRectShape {
public:
    static DrawRectShape& GetInstance();

    void Init(unsigned int screenWidth, unsigned int screenHeight);

    // 通用绘制函数
    void DrawRect(float x, float y, float width, float height,
        const glm::vec4& color,
        bool filled = true,      // true=实心, false=空心
        float lineWidth = 1.0f); // 仅当 filled=false 时有效

    void UpdateProjection(unsigned int screenWidth, unsigned int screenHeight);

private:
    DrawRectShape() = default;
    ~DrawRectShape();

    GLuint m_shaderID = 0;
    GLuint m_VAO = 0, m_VBO = 0;
    GLsizei m_vertexCount = 0;   // 当前使用的顶点数量
    glm::mat4 m_projection;
    bool m_initialized = false;

    void initShaders();
    void initBuffers();
    //void updateBuffers(float x, float y, float width, float height, const glm::vec4& color, bool filled);
};