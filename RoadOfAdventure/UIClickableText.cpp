#include "UIClickableText.h"

#include "Debug.h"






void UIClickableTextManager::Init(TextRenderer* renderer, int screenWidth, int screenHeight) {
    m_textRenderer = renderer;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void UIClickableTextManager::AddText(const std::string& text, float x, float y, float scale,
    std::function<void()> onClick,
    glm::vec3 normalColor, glm::vec3 hoverColor) {
    if (!m_textRenderer) return;

    ClickableText ct;
    ct.text = text;
    ct.x = x;
    ct.y = y;
    ct.scale = scale;
    ct.bounds = m_textRenderer->GetTextBounds(text, x, y, scale);
    ct.onClick = onClick;
    ct.normalColor = normalColor;
    ct.hoverColor = hoverColor;
    ct.isHovered = false;
    m_texts.push_back(ct);
}

void UIClickableTextManager::HandleClick(float mouseX, float mouseY) {
    int i = 0;
    for (auto& ct : m_texts) {
        if (ct.isHovered) {
            if (ct.onClick) {
                Debug("点击了的按钮索引: " + std::to_string(i));
                ct.onClick();
            }
            break; // 只触发第一个命中的
        }
        i++;
    }
}

void UIClickableTextManager::UpdateHover(float mouseX, float mouseY) {
    for (auto& ct : m_texts) {
        ct.isHovered = (mouseX >= ct.bounds.x && mouseX <= ct.bounds.x + ct.bounds.width &&
            mouseY >= ct.bounds.y && mouseY <= ct.bounds.y + ct.bounds.height);
    }
}

void UIClickableTextManager::Render() {
    if (!m_textRenderer) return;
    for (auto& ct : m_texts) {
        glm::vec3 color = ct.isHovered ? ct.hoverColor : ct.normalColor;
        m_textRenderer->RenderText(ct.text, ct.x, ct.y, ct.scale, color);
    }
}

void UIClickableTextManager::Clear() {
    m_texts.clear();
}