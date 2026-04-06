#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Image.h"
#include <vector>

class SpriteRenderer {
public:
    SpriteRenderer(unsigned int screenWidth, unsigned int screenHeight);
    ~SpriteRenderer();

    // 绘制精灵（使用图集纹理 + 子图区域）
    void DrawSprite(GLuint atlasTexture, const AtlasRegion& region,
        glm::vec2 position, glm::vec2 size,
        float rotate = 0.0f,
        glm::vec3 color = glm::vec3(1.0f));

    // 可选：更新投影矩阵（窗口大小改变时调用）
    void UpdateProjection(unsigned int screenWidth, unsigned int screenHeight);

private:
    GLuint m_shaderID;
    GLuint m_quadVAO;      // 顶点数组对象（位置）
    GLuint m_quadVBO;      // 位置 VBO
    GLuint m_texVBO;       // 纹理坐标 VBO
    glm::mat4 m_projection;

    void initRenderData();
};



struct SpriteVertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
    glm::vec4 color;   // 支持顶点颜色（动态精灵可能需要）
};

// ------------------- 静态精灵批次：预先构建，不可修改
class StaticSpriteBatch {
public:
    StaticSpriteBatch(GLuint atlasTexture);
    ~StaticSpriteBatch();
    void AddSprite(float x, float y, float w, float h, const AtlasRegion& region, const glm::vec4& color = glm::vec4(1.0f));
    void Build();                     // 调用后不可再添加
    void Draw() const;
    void Clear();
private:
    GLuint m_atlasTexture;
    GLuint m_VAO, m_VBO;
    std::vector<SpriteVertex> m_vertices;
    bool m_built;
};

// -------------- 动态精灵批次：每帧重新构建，支持快速更新
class DynamicSpriteBatch {
public:
    DynamicSpriteBatch(GLuint atlasTexture);
    ~DynamicSpriteBatch();
    void Begin();                     // 清空内部列表
    void AddSprite(float x, float y, float w, float h, const AtlasRegion& region, const glm::vec4& color = glm::vec4(1.0f));
    void End();                       // 上传数据并绘制（或者提供单独的 Draw 方法）
    void Draw();                      // 如果不想每帧重建，可以预先 Build 然后每帧修改属性
private:
    GLuint m_atlasTexture;
    GLuint m_VAO, m_VBO;
    std::vector<SpriteVertex> m_vertices;
    bool m_initialized;
    void initBuffers();
};