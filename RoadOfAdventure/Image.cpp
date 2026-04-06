#include "Image.h"



std::vector<AtlasRegion> regions;//图集区域数据
GLuint atlas;


/*
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
    int targetSize,
    int maxAtlasSize) {
    if (imagePaths.empty()) return 0;

    // 1. 加载并缩放所有图片，记录缩放后的像素数据
    struct ImageEntry {
        int width, height;
        unsigned char* data;  // RGBA 格式
    };
    std::vector<ImageEntry> images;//存储加载和缩放后的图像数据

    for (const auto& path : imagePaths) {//加载图片
        int w, h, channels;
        unsigned char* original = stbi_load(path.c_str(), &w, &h, &channels, 4); // 强制 RGBA
        if (!original) {
            // 加载失败，跳过（或可报错）
            continue;
        }
        // 缩放至 targetSize x targetSize
        unsigned char* resized = (unsigned char*)malloc(targetSize * targetSize * 4);
        stbir_resize_uint8(original, w, h, 0,
            resized, targetSize, targetSize, 0,
            4);
        stbi_image_free(original);// 释放原始图像内存

        images.push_back({ targetSize, targetSize, resized });// 存储缩放后的图像数据
    }

    if (images.empty()) return 0;

    // 2. 计算图集布局（网格排列）
    int numImages = (int)images.size();// 图片数量
    int cols = (int)sqrt(numImages);// 计算列数（尽量接近正方形）
    int rows = (numImages + cols - 1) / cols;// 计算行数
    int atlasWidth = cols * targetSize;// 图集宽度
    int atlasHeight = rows * targetSize;// 图集高度

    // 如果超出最大尺寸，尝试调整列数（例如改为更宽但不超过 maxAtlasSize）
    if (atlasWidth > maxAtlasSize || atlasHeight > maxAtlasSize) {// 超出最大尺寸
        // 简单策略：计算需要的行和列，确保宽高都不超过 maxAtlasSize
        cols = maxAtlasSize / targetSize;// 计算最大列数
        if (cols == 0) cols = 1;// 至少一列
        rows = (numImages + cols - 1) / cols;// 重新计算行数
        atlasWidth = cols * targetSize;// 重新计算图集宽度
        atlasHeight = rows * targetSize;// 重新计算图集高度
        if (atlasHeight > maxAtlasSize) {// 仍超出最大尺寸
            // 仍超出，说明图片太多，需要更大的图集或降低目标尺寸
            for (auto& img : images) free(img.data);// 释放缩放后的图像内存
            return 0;// 创建失败
        }
    }

    // 3. 创建 OpenGL 纹理（空）
    GLuint atlasTex;
    glGenTextures(1, &atlasTex);
    glBindTexture(GL_TEXTURE_2D, atlasTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 4. 逐图拷贝到图集，并记录区域信息
    outRegions.clear();
    int idx = 0;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (idx >= numImages) break;
            const auto& img = images[idx];
            int x = col * targetSize;
            int y = row * targetSize;
            // 拷贝数据
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, targetSize, targetSize,
                GL_RGBA, GL_UNSIGNED_BYTE, img.data);
            // 记录区域
            AtlasRegion region;
            region.name = imagePaths[idx];
            region.x = x;
            region.y = y;
            region.width = targetSize;
            region.height = targetSize;
            region.uvMin = glm::vec2((float)x / atlasWidth, (float)y / atlasHeight);
            region.uvMax = glm::vec2((float)(x + targetSize) / atlasWidth,
                (float)(y + targetSize) / atlasHeight);
            outRegions.push_back(region);
            ++idx;
        }
        if (idx >= numImages) break;
    }

    // 可选：生成 mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放缩放后的图像内存
    for (auto& img : images) {
        free(img.data);
    }

    return atlasTex;
}



// 从图集中提取子图像数据（RGBA），返回是否成功
bool ExtractSubImageFromAtlas(GLuint atlasTexture, const AtlasRegion& region,
    std::vector<unsigned char>& outData) {
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    int atlasWidth, atlasHeight;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &atlasWidth);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &atlasHeight);

    std::vector<unsigned char> atlasData(atlasWidth * atlasHeight * 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());

    outData.resize(region.width * region.height * 4);
    for (int row = 0; row < region.height; ++row) {
        int srcRow = region.y + row;
        int dstRow = row;
        memcpy(outData.data() + dstRow * region.width * 4,
            atlasData.data() + srcRow * atlasWidth * 4 + region.x * 4,
            region.width * 4);
    }
    return true;
}