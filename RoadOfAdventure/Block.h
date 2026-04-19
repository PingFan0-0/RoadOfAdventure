#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <cmath>
#include "GameData.h"
#include "Debug.h"

#include "Image.h"



const int CHUNK_SIZE = 16;          // 区块宽度（x,z 方向）
const int CHUNK_HEIGHT = 16;        // 区块高度（y 方向）
const int WORLD_RADIUS = 2;         // 渲染半径（区块数），世界大小 = (2*R+1)^2 个区块
const int VIEW_DISTANCE = 64;       // 视距（单位：方块），超过这个距离的区块不渲染


// 顶点数据
struct VertexTo {// 顶点数据结构，包含位置、纹理坐标和法线
    glm::vec3 pos;//顶点位置
    glm::vec2 uv;//顶点纹理坐标
    glm::vec3 normal;//顶点法线
};




using BlockID = uint16_t;
const BlockID AIR = 0;//空气 air

class Block {
public:
    Block(int cx = 0, int cz = 0) : m_cx(cx), m_cz(cz), m_vao(0), m_vbo(0), m_ebo(0), m_indexCount(0) {// 构造函数，初始化区块坐标
        //for (int x = 0; x < CHUNK_SIZE; ++x) {// 遍历区块内的每个方块位置
        //    for (int z = 0; z < CHUNK_SIZE; ++z) {// 计算世界坐标（用于生成高度）
        //        int worldX = cx * CHUNK_SIZE + x;// 根据区块坐标和方块坐标计算世界坐标
        //        int worldZ = cz * CHUNK_SIZE + z;// 根据世界坐标生成高度（这里使用简单的正弦余弦函数模拟地形，实际可用 Perlin 噪声等更复杂算法）
        //        // 简单的噪声高度（使用正弦余弦模拟，实际可用 Perlin）
        //        int height = (int)((sin(worldX * 0.1f) + cos(worldZ * 0.1f)) * 3.0f + 8.0f);// 生成高度在 5-11 之间，形成起伏的地形
        //        for (int y = 0; y < CHUNK_HEIGHT; ++y) {// 根据高度设置方块类型
        //            if (y > height) {// 高于地面部分设置为空气
        //                m_blocks[x][y][z] = 0;// 空气
        //            }
        //            else if (y == height) {// 地面部分设置为草地
        //                m_blocks[x][y][z] = 1;// 草地
        //            }
        //            else if (y > height - 3) {// 地面下方3层设置为泥土
        //                m_blocks[x][y][z] = 2;// 泥土
        //            }
        //            else {
        //                m_blocks[x][y][z] = 3;// 更深层设置为石头
        //            }
        //        }
        //    }
        //}
        //m_blocks.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT);
        m_blocks.assign(CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT, AIR);//申请空间
        //for (int x = 0; x < CHUNK_SIZE; ++x) {// 遍历区块内的每个方块位置
        //    for (int z = 0; z < CHUNK_SIZE; ++z) {// 计算世界坐标（用于生成高度）
        //        int worldX = cx * CHUNK_SIZE + x;// 根据区块坐标和方块坐标计算世界坐标
        //        int worldZ = cz * CHUNK_SIZE + z;// 根据世界坐标生成高度（这里使用简单的正弦余弦函数模拟地形，实际可用 Perlin 噪声等更复杂算法）
        //        int idx_floor = to_MapHand(worldX, worldZ);//获取地址
        //        if (idx_floor >= 0 && static_cast<size_t>(idx_floor) < Map.Data.size() && (worldX >= 0 && worldX < Map.maxx)) {
        //            m_blocks[index(x, 0, z)] = 1;
        //            if (m_blocks[index(x, 1, z)] == 6)Map.Data[idx_floor].floor = 5 ;
        //        }
                //else {
                //    
                //}
                //for (int y = 2; y < CHUNK_SIZE; ++y) {
                //    m_blocks[index(x, y, z)] = AIR;// 空气
                //}
        //    }
        //}
        //generateMesh();// 生成网格数据
        //uploadMesh();// 上传到 GPU
    }
    ~Block() {
        if (m_vao) glDeleteVertexArrays(1, &m_vao);// 析构函数，删除 OpenGL 相关资源
        if (m_vbo) glDeleteBuffers(1, &m_vbo);// 删除顶点缓冲对象
        if (m_ebo) glDeleteBuffers(1, &m_ebo);// 删除索引缓冲对象
    }

    // Disable copy to avoid double-free of GL resources
    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;

    // Move semantics: transfer GL handles and internal data
    Block(Block&& other) noexcept
        : m_cx(other.m_cx), m_cz(other.m_cz), rleData(std::move(other.rleData)), palette(std::move(other.palette)),
          m_blocks(std::move(other.m_blocks)), m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)),
          m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo), m_indexCount(other.m_indexCount) {
        other.m_vao = 0; other.m_vbo = 0; other.m_ebo = 0; other.m_indexCount = 0;
    }

    Block& operator=(Block&& other) noexcept {
        if (this != &other) {
            if (m_vao) glDeleteVertexArrays(1, &m_vao);
            if (m_vbo) glDeleteBuffers(1, &m_vbo);
            if (m_ebo) glDeleteBuffers(1, &m_ebo);

            m_cx = other.m_cx; m_cz = other.m_cz;
            rleData = std::move(other.rleData);
            palette = std::move(other.palette);
            m_blocks = std::move(other.m_blocks);
            m_vertices = std::move(other.m_vertices);
            m_indices = std::move(other.m_indices);

            m_vao = other.m_vao; m_vbo = other.m_vbo; m_ebo = other.m_ebo; m_indexCount = other.m_indexCount;

            other.m_vao = 0; other.m_vbo = 0; other.m_ebo = 0; other.m_indexCount = 0;
        }
        return *this;
    }

    // 刷新 GPU网格数据
    void MeshData(){
        generateMesh();// 生成网格数据
        uploadMesh();// 上传到 GPU
    }

    // 渲染函数，绑定 VAO 并调用 glDrawElements 绘制区块
    void render() const {
        if (m_indexCount == 0) return;
        glBindVertexArray(m_vao);// 绑定顶点数组对象
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);// 绘制区块
        glBindVertexArray(0);// 解绑顶点数组对象
    }
    // 获取区块的 AABB（用于视锥剔除）
    void getAABB(glm::vec3& min, glm::vec3& max) const {
        min = glm::vec3(m_cx * CHUNK_SIZE, 0, m_cz * CHUNK_SIZE);
        max = min + glm::vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE);
    }


    // 方块索引转换 (x,y,z 为区块内局部坐标 0..15)
    size_t index(int x, int y, int z) const {
        return x + CHUNK_SIZE * (y + CHUNK_SIZE * z);
    }
    // 设置方块
    void setBlock(int x, int y, int z, BlockID id) {
        m_blocks[index(x, y, z)] = id;
    }
    // 获取方块
    BlockID getBlock(int x, int y, int z) const {
        return m_blocks[index(x, y, z)];
    }


    // ========== 编码：生成 palette 与 RLE ==========
    void encodeRLE() {
        // 1. 构建调色板（去重 BlockID）
        std::unordered_map<BlockID, uint8_t> idToIndex;
        palette.clear();
        for (BlockID id : m_blocks) {
            if (idToIndex.find(id) == idToIndex.end()) {
                idToIndex[id] = static_cast<uint8_t>(palette.size());
                palette.push_back(id);
            }
        }

        // 2. 将 blocks 转换为调色板索引序列
        std::vector<uint8_t> indices;
        indices.reserve(m_blocks.size());
        for (BlockID id : m_blocks) {
            indices.push_back(idToIndex[id]);
        }

        // 3. 对索引序列进行 RLE 编码
        rleData.clear();
        if (indices.empty()) return;

        uint8_t prev = indices[0];
        uint8_t count = 1;
        for (size_t i = 1; i < indices.size(); ++i) {
            uint8_t curr = indices[i];
            if (curr == prev && count < 255) {
                ++count;
            }
            else {
                rleData.push_back(prev);
                rleData.push_back(count);
                prev = curr;
                count = 1;
            }
        }
        rleData.push_back(prev);
        rleData.push_back(count);
    }
    // ========== 解码：从 RLE 还原 blocks ==========
    void decodeRLE() {
        m_blocks.assign(CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT, AIR);
        if (rleData.empty() || palette.empty()) return;

        std::vector<uint8_t> indices;
        indices.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_HEIGHT);

        // 解压 RLE 为索引序列
        for (size_t i = 0; i < rleData.size(); i += 2) {
            uint8_t idx = rleData[i];
            uint8_t len = rleData[i + 1];
            for (int j = 0; j < len; ++j) {
                indices.push_back(idx);
            }
        }

        // 如果索引数量不等于 4096，说明数据损坏
        if (indices.size() != m_blocks.size()) {
            Error("解码失败：块计数不正确", "W");
        }

        // 通过调色板映射回 BlockID
        for (size_t i = 0; i < indices.size(); ++i) {
            m_blocks[i] = palette[indices[i]];
        }
    }



    int m_cx, m_cz;// 区块坐标
    std::vector<uint8_t> rleData;//  编码后的字节流
    std::vector<BlockID> palette;//  字典：本区块用到的 字典 列表
    std::vector<BlockID> m_blocks;// 方块数据，线性数组表示区块内每个位置的方块类型
private:
    GLuint m_vao, m_vbo, m_ebo;// OpenGL 相关变量：顶点数组对象、顶点缓冲对象、索引缓冲对象
    GLsizei m_indexCount;// 索引数量
    //std::array<std::array<std::array<BlockID, CHUNK_SIZE>, CHUNK_HEIGHT>, CHUNK_SIZE> m_blocks;// 方块数据，三维数组表示区块内每个位置的方块类型
    //std::vector<std::vector<std::vector<uint8_t> > > m_blocks;// 方块数据，三维数组表示区块内每个位置的方块类型

    // 添加一个面的顶点和索引
    void addFace(std::vector<VertexTo>& vertices, std::vector<GLuint>& indices,
        const glm::vec3& pos, const glm::vec3& normal,
        const glm::vec2& uvMin, const glm::vec2& uvMax) {// 根据法线方向确定四个顶点的位置
        // 根据法线方向确定四个顶点的位置
        float x = pos.x, y = pos.y, z = pos.z;// 方块尺寸为1单位
        float w = 1.0f, h = 1.0f;// 四个顶点位置
        glm::vec3 v0, v1, v2, v3;// 四个顶点位置
        glm::vec2 uv0, uv1, uv2, uv3;// 四个顶点的纹理坐标

        if (normal.x == 1) { // 右面 (+x)
            v0 = { x + w, y,   z };   v1 = { x + w, y,   z + w };// 右面顶点位置
            v2 = { x + w, y + h, z + w }; v3 = { x + w, y + h, z };// 右面顶点位置
            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };// 右面纹理坐标
            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };// 右面纹理坐标
        }
        else if (normal.x == -1) { // 左面 (-x)
            v0 = { x, y,   z + w }; v1 = { x, y,   z };
            v2 = { x, y + h, z }; v3 = { x, y + h, z + w };
            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
        }
        else if (normal.y == 1) { // 上面 (+y)
            v0 = { x,   y + h, z };   v1 = { x + w, y + h, z };
            v2 = { x + w, y + h, z + w }; v3 = { x,   y + h, z + w };
            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
        }
        else if (normal.y == -1) { // 下面 (-y)
            v0 = { x,   y, z + w }; v1 = { x + w, y, z + w };
            v2 = { x + w, y, z };   v3 = { x,   y, z };
            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
        }
        else if (normal.z == 1) { // 前面 (+z)
            v0 = { x + w, y,   z + w }; v1 = { x,   y,   z + w };
            v2 = { x,   y + h, z + w }; v3 = { x + w, y + h, z + w };
            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
        }
        else if (normal.z == -1) { // 后面 (-z)
            v0 = { x,   y,   z }; v1 = { x + w, y,   z };
            v2 = { x + w, y + h, z }; v3 = { x,   y + h, z };
            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
        }
        else {
            return;
        }

        GLuint base = (GLint)vertices.size();// 当前顶点的起始索引
        vertices.push_back({ v0, uv0, normal });// 添加四个顶点数据
        vertices.push_back({ v1, uv1, normal });// 添加四个顶点数据
        vertices.push_back({ v2, uv2, normal });// 添加四个顶点数据
        vertices.push_back({ v3, uv3, normal });// 添加四个顶点数据

        indices.push_back(base);// 添加两个三角形的索引数据
        indices.push_back(base + 1);// 添加两个三角形的索引数据
        indices.push_back(base + 2);// 添加两个三角形的索引数据
        indices.push_back(base);// 添加两个三角形的索引数据
        indices.push_back(base + 2);// 添加两个三角形的索引数据
        indices.push_back(base + 3);// 添加两个三角形的索引数据
    }

    void generateMesh() {// 生成区块的顶点和索引数据

        std::vector<VertexTo> vertices;// 存储顶点数据
        std::vector<GLuint> indices;// 存储索引数据

        glFrontFace(GL_CW);
        for (int x = 0; x < CHUNK_SIZE; ++x) {// 遍历区块内的每个方块位置
            for (int y = 0; y < CHUNK_HEIGHT; ++y) {// 遍历区块内的每个方块位置
                for (int z = 0; z < CHUNK_SIZE; ++z) {// 获取方块类型
                    BlockID type = m_blocks[index(x, y, z)];// 如果是空气，跳过
                    BlockID BLOCK_AIR = 0;
                    if (type == BLOCK_AIR) continue;//空气

                    glm::vec3 worldPos = glm::vec3(m_cx * CHUNK_SIZE + x, y, m_cz * CHUNK_SIZE + z);// 计算世界坐标（用于生成顶点位置）
                    glm::vec2 uvMin = regions[type].uvMin;// 获取纹理坐标
                    glm::vec2 uvMax = regions[type].uvMax;// 获取纹理坐标

                    // 检查六个方向是否有相邻方块，没有则添加面
                    // 右 (x+1)
                    if (x + 1 >= CHUNK_SIZE || m_blocks[index(x + 1, y, z)] == BLOCK_AIR)
                        addFace(vertices, indices, worldPos, glm::vec3(1, 0, 0), uvMin, uvMax);
                    // 左 (x-1)
                    if (x - 1 < 0 || m_blocks[index(x - 1, y, z)] == BLOCK_AIR)
                        addFace(vertices, indices, worldPos, glm::vec3(-1, 0, 0), uvMin, uvMax);
                    // 上 (y+1)
                    if (y + 1 >= CHUNK_HEIGHT || m_blocks[index(x, y + 1, z)] == BLOCK_AIR)
                        addFace(vertices, indices, worldPos, glm::vec3(0, 1, 0), uvMin, uvMax);
                    // 下 (y-1)
                    if (y - 1 < 0 || m_blocks[index(x, y - 1, z)] == BLOCK_AIR)
                        addFace(vertices, indices, worldPos, glm::vec3(0, -1, 0), uvMin, uvMax);
                    // 前 (z+1)
                    if (z + 1 >= CHUNK_SIZE || m_blocks[index(x, y, z + 1)] == BLOCK_AIR)
                        addFace(vertices, indices, worldPos, glm::vec3(0, 0, 1), uvMin, uvMax);
                    // 后 (z-1)
                    if (z - 1 < 0 || m_blocks[index(x, y, z - 1)] == BLOCK_AIR)
                        addFace(vertices, indices, worldPos, glm::vec3(0, 0, -1), uvMin, uvMax);
                }
            }
        }

        // 存储顶点和索引数据到成员变量，用于上传
        m_vertices   = vertices;               // 存储顶点数据
        m_indices    = indices;                // 存储索引数据
        m_indexCount = (GLsizei)indices.size();// 存储索引数量
    }

    void uploadMesh() {
        if (m_vertices.empty() || m_indices.empty()) return;

        glGenVertexArrays(1, &m_vao);// 生成顶点数组对象
        glGenBuffers(1, &m_vbo);// 生成顶点缓冲对象
        glGenBuffers(1, &m_ebo);// 生成索引缓冲对象

        glBindVertexArray(m_vao);// 绑定顶点数组对象
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);// 绑定顶点缓冲对象
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexTo), m_vertices.data(), GL_STATIC_DRAW);// 上传顶点数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);// 绑定索引缓冲对象
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);// 上传索引数据

        // 位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTo), (void*)offsetof(VertexTo, pos));// 定义位置属性指针
        glEnableVertexAttribArray(0);// 启用位置属性
        // 纹理坐标
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTo), (void*)offsetof(VertexTo, uv));
        glEnableVertexAttribArray(1);
        // 法线
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTo), (void*)offsetof(VertexTo, normal));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);// 解绑顶点数组对象
        // 清空CPU侧数据，释放内存
        m_vertices.clear();// 清空顶点数据，释放内存
        m_indices.clear();// 清空索引数据，释放内存
    }

    std::vector<VertexTo> m_vertices;
    std::vector<GLuint> m_indices;
};