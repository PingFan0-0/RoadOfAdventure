#pragma once

#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H



/// 字形结构：存储纹理ID和度量信息
struct Character {
    GLuint     TextureID;   // 字形纹理ID
    glm::ivec2 Size;        // 字形位图宽高
    glm::ivec2 Bearing;     // 从基线到字形左上角的偏移
    GLuint     Advance;     // 到下一个字形的距离 (1/64像素)
};
/// 文本边界结构：用于计算文本的实际占用区域
struct TextBounds {
    float x, y;          // 左上角坐标
    float width, height; // 宽高
};

/// 文本渲染器类
class TextRenderer {
public:
    const GLint FontSize = 48;

    // 禁用拷贝构造和拷贝赋值
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    // 支持移动构造和移动赋值
    TextRenderer(TextRenderer&&) noexcept = default;
    TextRenderer& operator=(TextRenderer&&) noexcept = default;

    // 构造函数：需要屏幕宽高（用于正交投影）
    TextRenderer(GLuint screenWidth, GLuint screenHeight);
    ~TextRenderer();

    // 加载字体文件，fontSize为像素高度
    bool Load(const std::string& fontPath, GLuint fontSize);

    // 渲染文本，位置(x,y)为屏幕像素坐标（原点左上角），scale为缩放，color为RGB颜色
    void RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));

    // 计算文本边界：返回文本的实际占用区域（用于布局）
    TextBounds GetTextBounds(const std::string& text, float x, float y, float scale);

private:
    // 着色器程序ID
    GLuint m_shaderID;
    // VAO, VBO
    GLuint m_VAO, m_VBO;
    // 投影矩阵
    glm::mat4 m_projection;
    // 字符映射表
    //std::map<unsigned long, Character> m_characters;
    std::unordered_map<uint32_t, Character> m_characters;
    // 编译着色器的辅助函数
    GLuint compileShader(GLenum type, const char* source);
    GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader);

    // UTF-8 转 Unicode 码点列表
    std::vector<unsigned long> utf8ToUnicode(const std::string& utf8Str);
    // 动态加载单个字符的辅助函数
    bool loadCharacter(unsigned long codePoint);

    // FreeType相关成员（用于动态加载新字符）
    FT_Library m_ftLibrary;// FreeType库对象
    FT_Face    m_ftFace;// 当前加载的字体面
    bool       m_ftInitialized;// 是否成功初始化FreeType
    GLuint     m_fontSize;// 当前字体大小（像素）
};