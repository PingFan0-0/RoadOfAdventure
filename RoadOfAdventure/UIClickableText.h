#pragma once

#include <string>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "TextRenderer.h"




struct ClickableText {
    std::string text;
    float x, y;
    float scale;
    TextBounds bounds;          // 包围盒（由管理器自动计算）
    std::function<void()> onClick;
    glm::vec3 normalColor;      // 正常颜色
    glm::vec3 hoverColor;       // 鼠标悬停颜色
    bool isHovered;             // 当前是否悬停（用于渲染）
};


class UIClickableTextManager {
public:
    static UIClickableTextManager& GetInstance() {
        static UIClickableTextManager instance;
        return instance;
    }

    // 初始化（需要传入 TextRenderer 和屏幕尺寸）
    void Init(TextRenderer* renderer, int screenWidth, int screenHeight);

    // 注册一个可点击文本
    void AddText(const std::string& text, float x, float y, float scale,
        std::function<void()> onClick,
        glm::vec3 normalColor = glm::vec3(1.0f),
        glm::vec3 hoverColor = glm::vec3(1.0f, 0.8f, 0.0f));

    // 处理鼠标点击（传入屏幕像素坐标，Y轴向下）
    void HandleClick(float mouseX, float mouseY);

    // 更新鼠标悬停状态（应在每帧渲染前调用）
    void UpdateHover(float mouseX, float mouseY);

    // 渲染所有可点击文本
    void Render();

    // 清除所有文本
    void Clear();

private:
    UIClickableTextManager() = default;
    ~UIClickableTextManager() = default;
    UIClickableTextManager(const UIClickableTextManager&) = delete;
    UIClickableTextManager& operator=(const UIClickableTextManager&) = delete;

    TextRenderer* m_textRenderer = nullptr;
    std::vector<ClickableText> m_texts;
    int m_screenWidth = 0;
    int m_screenHeight = 0;
};