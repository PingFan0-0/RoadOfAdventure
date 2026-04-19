#include "DrawShape.h"
#include <iostream>

static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color;
uniform mat4 projection;
void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    Color = aColor;
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
in vec4 Color;
out vec4 FragColor;
void main() {
    FragColor = Color;
}
)";

static GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cerr << "Shader compile error: " << info << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint linkProgram(GLuint vs, GLuint fs) {
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    GLint success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(prog, 512, nullptr, info);
        std::cerr << "Program link error: " << info << std::endl;
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

DrawRectShape::~DrawRectShape() {
    if (m_initialized) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        if (m_shaderID) glDeleteProgram(m_shaderID);
    }
}

DrawRectShape& DrawRectShape::GetInstance() {
    static DrawRectShape instance;
    return instance;
}

void DrawRectShape::Init(unsigned int screenWidth, unsigned int screenHeight) {
    if (m_initialized) return;
    initShaders();
    initBuffers();
    UpdateProjection(screenWidth, screenHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_initialized = true;
}

void DrawRectShape::initShaders() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!vs || !fs) return;
    m_shaderID = linkProgram(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void DrawRectShape::initBuffers() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void DrawRectShape::UpdateProjection(unsigned int screenWidth, unsigned int screenHeight) {
    m_projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(m_shaderID);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
}

void DrawRectShape::DrawRect(float x1, float y1, float x2, float y2,
    const glm::vec4& color,bool filled, float lineWidth) {

    if (!m_initialized || m_shaderID == 0) return;
    glUseProgram(m_shaderID);

    // 设置线宽（仅空心时有效）
    if (!filled) glLineWidth(lineWidth);

    // 准备顶点数据
    std::vector<float> vertices;
    if (filled) {
        // 实心：6个顶点（两个三角形）
        vertices = {
            x1,  y2,    color.r, color.g, color.b, color.a,
            x1,  y1,    color.r, color.g, color.b, color.a,
            x2,  y2,    color.r, color.g, color.b, color.a,
            x2,  y2,    color.r, color.g, color.b, color.a,
            x1,  y1,    color.r, color.g, color.b, color.a,
            x2,  y1,    color.r, color.g, color.b, color.a
        };
        m_vertexCount = 6;
    }
    else {
        // 空心：4个顶点（边界顺序）
        vertices = {
            x1,  y2,    color.r, color.g, color.b, color.a,
            x2,  y2,    color.r, color.g, color.b, color.a,
            x2,  y1,    color.r, color.g, color.b, color.a,
            x1,  y1,    color.r, color.g, color.b, color.a
        };
        m_vertexCount = 4;
    }

    // 更新 VBO
    glBindVertexArray(m_VAO);// 绑定 VAO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);// 绑定 VBO
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());// 更新顶点数据

    // 绘制
    if (filled)
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);// 绘制实心矩形
    else
        glDrawArrays(GL_LINE_LOOP, 0, m_vertexCount);// 绘制空心矩形

    glBindVertexArray(0);// 解绑
}