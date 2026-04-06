#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "OpenGL.h"




struct AtlasRegion {
    std::string name;        // 原始图片文件名（标识）
    glm::vec2 uvMin;         // 在大纹理中的 UV 最小值
    glm::vec2 uvMax;         // 在大纹理中的 UV 最大值
    int x, y;                // 在大纹理中的像素位置（左上角）
    int width, height;       // 实际存储的尺寸（即缩放后的目标尺寸）
};

extern std::vector<AtlasRegion> regions;//图集区域数据
extern GLuint atlas;

/**
 * 创建纹理图集：将所有输入图片缩放为统一尺寸，并合并到一张大纹理中
 *
 * @param imagePaths   输入图片路径列表
 * @param outRegions   输出：每个子图在大纹理中的区域信息（UV 和像素坐标）
 * @param targetSize   所有图片缩放后的目标尺寸（宽高相等，例如 64）
 * @param maxAtlasSize 图集的最大尺寸（宽或高，默认 2048，超出会报错）
 * @return 合并后的 OpenGL 纹理 ID，失败返回 0
 */
GLuint createTextureAtlas(const std::vector<std::string>& imagePaths,
    std::vector<AtlasRegion>& outRegions,
    int targetSize = 64,
    int maxAtlasSize = 2048);


// 从图集中提取子图像数据（RGBA），返回是否成功
bool ExtractSubImageFromAtlas(GLuint atlasTexture, const AtlasRegion& region,
    std::vector<unsigned char>& outData);