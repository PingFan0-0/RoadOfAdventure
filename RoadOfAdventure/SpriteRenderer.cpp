#include "SpriteRenderer.h"
#include <iostream>
#include "Debug.h"   // 假设您有 Error 宏

static const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;
    uniform mat4 model;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
        TexCoord = aTexCoord;
    }
)";

static const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D image;
    uniform vec3 spriteColor;
    void main() {
        FragColor = vec4(spriteColor, 1.0) * texture(image, TexCoord);
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
        Error("着色器编译失败: " + std::string(info), "W");
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
        Error("着色器链接失败: " + std::string(info), "W");
        glDeleteProgram(prog);
        return 0;
    }
    return prog;
}

SpriteRenderer::SpriteRenderer(unsigned int screenWidth, unsigned int screenHeight) {
    // 编译着色器
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!vs || !fs) {
        m_shaderID = 0;
        return;
    }
    m_shaderID = linkProgram(vs, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    if (!m_shaderID) return;

    // 设置正交投影矩阵（屏幕坐标，原点左上角，Y向下）
    m_projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);
    glUseProgram(m_shaderID);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteBuffers(1, &m_texVBO);
    if (m_shaderID) glDeleteProgram(m_shaderID);
}

void SpriteRenderer::initRenderData() {
    // 四个顶点位置（左下、右下、左上、右上），用于 Triangle Strip
    float vertices[] = {
        0.0f, 0.0f,   // 左下
        1.0f, 0.0f,   // 右下
        0.0f, 1.0f,   // 左上
        1.0f, 1.0f    // 右上
    };
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 纹理坐标 VBO（动态更新）
    glGenBuffers(1, &m_texVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(GLuint atlasTexture, const AtlasRegion& region,
    glm::vec2 position, glm::vec2 size,
    float rotate, glm::vec3 color) {
    if (!m_shaderID) return;

    glUseProgram(m_shaderID);
    glUniform3f(glGetUniformLocation(m_shaderID, "spriteColor"), color.r, color.g, color.b);

    // 构建模型矩阵（以精灵中心旋转）

    bool needrotate = (rotate != 0.0f);//是否需要变换

    glm::mat4 model = glm::mat4(1.0f);

    glm::vec2 center = position + size * 0.5f;// 计算中心点
    model = glm::translate(model, glm::vec3(center, 0.0f));// 平移到中心
    if(needrotate) model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));// 旋转
    model = glm::scale(model, glm::vec3(size, 1.0f));// 缩放到目标大小
    model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));// 因为顶点坐标是从 (0,0) 到 (1,1)，所以先平移到中心，旋转，缩放，再平移回去
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));


    // 绑定图集纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glUniform1i(glGetUniformLocation(m_shaderID, "image"), 0);

    // 计算纹理坐标（假设 region.uvMin/uvMax 基于图集左上角原点，需要翻转 V）
    float u1 = region.uvMin.x;
    float u2 = region.uvMax.x;
    float v1 = region.uvMin.y;
    float v2 = region.uvMax.y;

    // 纹理坐标顺序与顶点位置匹配（左下、右下、左上、右上）
    float texCoords[] = {
        u1, v1,   // 左下
        u2, v1,   // 右下
        u1, v2,   // 左上
        u2, v2    // 右上
    };
    glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);//绑定纹理坐标 VBO
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);// 更新纹理坐标数据

    // 绘制（Triangle Strip）
    glBindVertexArray(m_quadVAO);//绑定 VAO
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);// 绘制四个顶点
    glBindVertexArray(0);//解绑
}

// 更新投影矩阵（窗口大小改变时调用）
void SpriteRenderer::UpdateProjection(unsigned int screenWidth, unsigned int screenHeight) {//更新投影矩阵
    m_projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);//重新计算投影矩阵
    glUseProgram(m_shaderID);//使用着色器
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));//更新 uniform
}





//========================================


// ---------- 静态批次 ----------
StaticSpriteBatch::StaticSpriteBatch(GLuint atlasTexture)
    : m_atlasTexture(atlasTexture), m_VAO(0), m_VBO(0), m_built(false) {
}

StaticSpriteBatch::~StaticSpriteBatch() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
}

void StaticSpriteBatch::AddSprite(float x, float y, float w, float h,
    const AtlasRegion& region, const glm::vec4& color) {
    if (m_built) return;
    float x1 = x, y1 = y + h;
    float x2 = x + w, y2 = y;
    float u1 = region.uvMin.x, v1 = region.uvMin.y;
    float u2 = region.uvMax.x, v2 = region.uvMax.y;

    // 三角形1
    m_vertices.push_back({ {x1, y1}, {u1, v2}, color });
    m_vertices.push_back({ {x2, y1}, {u2, v2}, color });
    m_vertices.push_back({ {x1, y2}, {u1, v1}, color });
    // 三角形2
    m_vertices.push_back({ {x2, y1}, {u2, v2}, color });
    m_vertices.push_back({ {x2, y2}, {u2, v1}, color });
    m_vertices.push_back({ {x1, y2}, {u1, v1}, color });
}

void StaticSpriteBatch::Build() {
    if (m_vertices.empty()) return;
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(SpriteVertex),
        m_vertices.data(), GL_STATIC_DRAW);
    // 位置属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
        (void*)offsetof(SpriteVertex, pos));
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
        (void*)offsetof(SpriteVertex, texCoord));
    glEnableVertexAttribArray(1);
    // 颜色属性
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
        (void*)offsetof(SpriteVertex, color));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    m_built = true;
}

void StaticSpriteBatch::Draw() const {
    if (!m_built) return;
    glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());
    glBindVertexArray(0);
}

void StaticSpriteBatch::Clear() {
    m_vertices.clear();
    if (m_built) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_VAO = m_VBO = 0;
        m_built = false;
    }
}



//=======================================


// ---------- 动态批次 ----------
DynamicSpriteBatch::DynamicSpriteBatch(GLuint atlasTexture)
    : m_atlasTexture(atlasTexture), m_VAO(0), m_VBO(0), m_initialized(false) {
}

DynamicSpriteBatch::~DynamicSpriteBatch() {
    if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO) glDeleteBuffers(1, &m_VBO);
}

void DynamicSpriteBatch::initBuffers() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // 位置
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
        (void*)offsetof(SpriteVertex, pos));
    glEnableVertexAttribArray(0);
    // 纹理坐标
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
        (void*)offsetof(SpriteVertex, texCoord));
    glEnableVertexAttribArray(1);
    // 颜色
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex),
        (void*)offsetof(SpriteVertex, color));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    m_initialized = true;
}

void DynamicSpriteBatch::Begin() {
    m_vertices.clear();
}

void DynamicSpriteBatch::AddSprite(float x, float y, float w, float h,
    const AtlasRegion& region, const glm::vec4& color) {
    float x1 = x, y1 = y + h;
    float x2 = x + w, y2 = y;
    float u1 = region.uvMin.x, v1 = region.uvMin.y;
    float u2 = region.uvMax.x, v2 = region.uvMax.y;
    v1 = 1.0f - v1;
    v2 = 1.0f - v2;

    m_vertices.push_back({ {x1, y1}, {u1, v2}, color });
    m_vertices.push_back({ {x2, y1}, {u2, v2}, color });
    m_vertices.push_back({ {x1, y2}, {u1, v1}, color });
    m_vertices.push_back({ {x2, y1}, {u2, v2}, color });
    m_vertices.push_back({ {x2, y2}, {u2, v1}, color });
    m_vertices.push_back({ {x1, y2}, {u1, v1}, color });
}

void DynamicSpriteBatch::Draw() {
    if (m_vertices.empty()) return;
    if (!m_initialized) initBuffers();

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(SpriteVertex),
        m_vertices.data(), GL_DYNAMIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());
    glBindVertexArray(0);
}