#include "TextRenderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Debug.h"
#include "OpenGL.h"
#include "TypeConversion.h"
#include "GameData.h"



// ---------- 编译单个着色器 ----------
GLuint TextRenderer::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        Error("着色器编译失败: " + std::string(infoLog), "W");
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

// ---------- 链接着色器程序 ----------
GLuint TextRenderer::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        Error("着色器链接失败: " + std::string(infoLog), "W");
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

// ---------- 构造函数：加载着色器并设置投影 ----------
TextRenderer::TextRenderer() {
    // 1. 加载并编译着色器
    std::string vertCode = LoadShader("text_vert.glsl");
    std::string fragCode = LoadShader("text_frag.glsl");
    if (vertCode.empty() || fragCode.empty()) {
        Error("加载着色器文件失败", "W");
        m_shaderID = 0;
        return;
    }

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragCode.c_str());
    if (!vertexShader || !fragmentShader) {
        m_shaderID = 0;
        return;
    }

    m_shaderID = linkProgram(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (m_shaderID == 0) {
        Error("创建着色器程序失败", "W");
        return;
    }

    // 2. 设置正交投影矩阵
    m_projection = glm::ortho(0.0f, (float)Win.WinX, (float)Win.WinY, 0.0f, -1.0f, 1.0f);
    glUseProgram(m_shaderID);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));

    // 3. 配置VAO/VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (FT_Init_FreeType(&m_ftLibrary)) {
        Error("错误::FREETYPE: 无法初始化FreeType库", "W");
        m_ftInitialized = false;
    }
    else {
        m_ftInitialized = true;
    }
}

// ---------- 析构函数 ----------
TextRenderer::~TextRenderer() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    for (auto& pair : m_characters) {
        glDeleteTextures(1, &pair.second.TextureID);
    }
    if (m_shaderID) glDeleteProgram(m_shaderID);
    if (m_ftInitialized && m_ftFace) {
        FT_Done_Face(m_ftFace);
    }
    if (m_ftInitialized) {
        FT_Done_FreeType(m_ftLibrary);
    }
}

// ---------- 加载字体并预生成字形纹理 ----------
bool TextRenderer::Load(const std::string& fontPath, GLuint fontSize) {
    m_characters.clear();// 清空之前的字符数据
    if (!m_ftInitialized) return false;// 确保FreeType已初始化

    if (FT_New_Face(m_ftLibrary, fontPath.c_str(), 0, &m_ftFace)) {
        Error("FREETYPE: 加载字体失败" + fontPath, "W");
        return false;
    }
    m_fontSize = fontSize;
    FT_Set_Pixel_Sizes(m_ftFace, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned long c = 32; c < 127; ++c) {
        loadCharacter(c);
        if (m_characters[c].TextureID == 0) {
            //Error("加载文字失败" + std::to_string(c), "W");
        }
    }

    return true;
}

// ---------- 动态加载单个字符的辅助函数 ----------
bool TextRenderer::loadCharacter(unsigned long codePoint) {
    if (!m_ftInitialized || !m_ftFace) return false;// 已经加载过该字符，直接返回
    if (m_characters.find(codePoint) != m_characters.end()) return true;// 加载字符到 FreeType

    if (FT_Load_Char(m_ftFace, codePoint, FT_LOAD_RENDER)) {// 加载失败
        Error("FREETYPE: 加载字符失败: " + std::to_string(codePoint), "W");
        return false;
    }

    GLuint texture;// 创建 OpenGL 纹理
    glGenTextures(1, &texture);// 创建纹理对象
    glBindTexture(GL_TEXTURE_2D, texture);// 上传字形位图数据到纹理

    int width = m_ftFace->glyph->bitmap.width;// 获取字形位图的宽高
    int height = m_ftFace->glyph->bitmap.rows;// 处理空格等不可见字符

    if (width == 0 || height == 0) {// 字形没有可见像素
        // 对于空格等不可见字符，创建 1x1 透明纹理，避免纹理 ID 为 0
        unsigned char dummyPixel[4] = { 0, 0, 0, 0 };// RGBA 全透明
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, dummyPixel);// 创建一个1x1的透明纹理
        width = height = 1; // 标记为透明占位，渲染时跳过绘制
    }
    else {// 正常加载字形位图
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE,
            m_ftFace->glyph->bitmap.buffer);// 字形位图是单通道灰度图，使用 GL_RED 格式上传
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// 字形纹理不需要重复，使用边缘像素填充
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 使用线性过滤，保持字形边缘平滑
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 同上

    Character character = {// 创建 Character 结构体并存储到映射表
        texture,// 字形纹理ID
        glm::ivec2(width, height),// 字形位图宽高
        glm::ivec2(m_ftFace->glyph->bitmap_left, m_ftFace->glyph->bitmap_top),// 从基线到字形左上角的偏移
        (GLuint)m_ftFace->glyph->advance.x
    };
    m_characters[codePoint] = character;// 存储到字符映射表
    return true;// 加载成功
}

// ---------- UTF-8 转 Unicode ----------
std::vector<unsigned long> TextRenderer::utf8ToUnicode(const std::string& utf8Str) {
    std::vector<unsigned long> result;
    size_t i = 0;
    while (i < utf8Str.size()) {
        unsigned char c = utf8Str[i];
        unsigned long cp = 0;
        int bytes = 0;
        if (c < 0x80) {                     // 1-byte
            cp = c;
            bytes = 1;
        }
        else if ((c >> 5) == 0x06) {      // 2-byte (110xxxxx)
            cp = c & 0x1F;
            bytes = 2;
        }
        else if ((c >> 4) == 0x0E) {      // 3-byte (1110xxxx)
            cp = c & 0x0F;
            bytes = 3;
        }
        else if ((c >> 3) == 0x1E) {      // 4-byte (11110xxx)
            cp = c & 0x07;
            bytes = 4;
        }
        else {
            // 无效起始字节，跳过
            ++i;
            continue;
        }

        if (i + bytes > utf8Str.size()) break;

        bool valid = true;
        for (int j = 1; j < bytes; ++j) {
            unsigned char next = utf8Str[i + j];
            if ((next & 0xC0) != 0x80) {
                valid = false;
                break;
            }
            cp = (cp << 6) | (next & 0x3F);
        }

        if (valid) {
            result.push_back(cp);
        }
        i += bytes;
    }
    return result;// 返回 Unicode 码点列表
}

// ---------- 渲染字符串 ---------- 
void TextRenderer::RenderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    if (m_shaderID == 0) {
        Error("着色器程序无效", "W");
        return;
    }
    if (!m_ftInitialized || !m_ftFace) {
        Error("FreeType 未初始化或字体未加载", "W");
        return;
    }

    std::vector<unsigned long> unicodes = utf8ToUnicode(to_utf8(text));
    if (unicodes.empty()) return;

    glUseProgram(m_shaderID);// 设置文本颜色
    glUniform3f(glGetUniformLocation(m_shaderID, "textColor"), color.r, color.g, color.b);// 激活纹理单元并绑定 VAO
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);
    float originX = x;// 从指定的 x 坐标开始渲染文本

    for (unsigned long codePoint : unicodes) {// 遍历每个 Unicode 码点
        auto it = m_characters.find(codePoint);// 查找字符信息
        if (it == m_characters.end()) {// 字符未预加载，尝试动态加载
            if (!loadCharacter(codePoint)) {// 尝试动态加载字符失败
                originX += m_fontSize * 0.3f * scale;
                continue;// 加载失败，使用一个默认宽度（例如字体大小的 1/3）继续
            }
            it = m_characters.find(codePoint);
            if (it == m_characters.end()) {// 再次检查是否成功加载
                originX += m_fontSize * 0.3f * scale;
                continue;// 加载失败，使用一个默认宽度（例如字体大小的 1/3）继续
            }
        }
        Character& ch = it->second;// 获取字符信息

        if (ch.Size.x > 1 && ch.Size.y > 1) {// 计算字形四边形的顶点位置和纹理坐标
            float xpos = originX + ch.Bearing.x * scale;// 计算字形四边形的顶点位置，调整 x 坐标以考虑字形左侧空白
            float ypos = y - (ch.Bearing.y - m_fontSize * 0.8f) * scale;// 计算字形四边形的顶点位置，调整 y 坐标以更好地对齐基线
            float w = ch.Size.x * scale;// 计算字形四边形的顶点数据
            float h = ch.Size.y * scale;// 计算字形四边形的顶点数据

            float vertices[6][4] = {
                { xpos,     ypos + h, 0.0f, 1.0f },
                { xpos,     ypos,     0.0f, 0.0f },
                { xpos + w, ypos,     1.0f, 0.0f },
                { xpos,     ypos + h, 0.0f, 1.0f },
                { xpos + w, ypos,     1.0f, 0.0f },
                { xpos + w, ypos + h, 1.0f, 1.0f }
            };
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);// 绑定字形纹理
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);// 更新 VBO 数据
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);// 更新 VBO 数据
            glBindBuffer(GL_ARRAY_BUFFER, 0);// 更新 VBO 数据
            glDrawArrays(GL_TRIANGLES, 0, 6);// 绘制字形四边形
        }
        originX += (ch.Advance >> 6) * scale;// Advance 是 1/64 像素单位，需要右移 6 位转换为像素
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);// 解绑 VAO 和纹理
}

// ---------- 计算文本边界 ----------
TextBounds TextRenderer::GetTextBounds(const std::string& text, float x, float y, float scale) {
    TextBounds bounds;
    bounds.x = x;
    bounds.y = y;
    bounds.width = 0;
    bounds.height = 0;

    std::vector<unsigned long> unicodes = utf8ToUnicode(to_utf8(text));
    float curX = x;
    float minY = y;
    float maxY = y;

    for (unsigned long cp : unicodes) {
        // 确保字符已加载
        if (m_characters.find(cp) == m_characters.end()) {
            if (!loadCharacter(cp)) continue;
        }
        Character& ch = m_characters[cp];

        float xpos = curX + ch.Bearing.x * scale;
        float ypos = y - (ch.Bearing.y - m_fontSize * 0.8f) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        // 更新总宽度（当前字符的右边界）
        float right = xpos + w;
        if (right - bounds.x > bounds.width) {
            bounds.width = right - bounds.x;
        }

        // 更新最小/最大Y
        if (ypos < minY) minY = ypos;
        if (ypos + h > maxY) maxY = ypos + h;

        curX += (ch.Advance >> 6) * scale;
    }

    bounds.height = maxY - minY;
    bounds.y = minY;  // 实际绘制时的顶部Y
    return bounds;
}


// 更新投影矩阵（窗口大小改变时调用）
void TextRenderer::UpdateProjection(unsigned int screenWidth, unsigned int screenHeight) {//更新投影矩阵
    m_projection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f);//重新计算投影矩阵
    glUseProgram(m_shaderID);//使用着色器
    glUniformMatrix4fv(glGetUniformLocation(m_shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));//更新 uniform
}