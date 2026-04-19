//#pragma once
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//#include <array>
//#include <unordered_map>
//#include <cmath>
//
//
//
//const int CHUNK_SIZE = 16;          // 区块宽度和高度（x,z 方向）
//const int CHUNK_HEIGHT = 16;        // 区块高度（y 方向）
//const int WORLD_RADIUS = 2;         // 渲染半径（区块数），世界大小 = (2*R+1)^2 个区块
//const int VIEW_DISTANCE = 64;
//
//
//
//
//
//enum BlockType {
//    BLOCK_AIR = 0,
//    BLOCK_GRASS,
//    BLOCK_DIRT,
//    BLOCK_STONE
//};
//
//// 顶点数据
//struct Vertex {
//    glm::vec3 pos;
//    glm::vec2 uv;
//    glm::vec3 normal;
//};
//
//class Chunk {
//public:
//    Chunk(int cx, int cz) : m_cx(cx), m_cz(cz), m_vao(0), m_vbo(0), m_ebo(0), m_indexCount(0) {
//        // 初始化方块数据（简单生成一个地形：草地 + 泥土 + 石头）
//        for (int x = 0; x < CHUNK_SIZE; ++x) {// 遍历区块内的每个方块位置
//            for (int z = 0; z < CHUNK_SIZE; ++z) {// 计算世界坐标（用于生成高度）
//                int worldX = cx * CHUNK_SIZE + x;// 根据区块坐标和方块坐标计算世界坐标
//                int worldZ = cz * CHUNK_SIZE + z;// 根据世界坐标生成高度（这里使用简单的正弦余弦函数模拟地形，实际可用 Perlin 噪声等更复杂算法）
//                // 简单的噪声高度（使用正弦余弦模拟，实际可用 Perlin）
//                int height = (int)((sin(worldX * 0.1f) + cos(worldZ * 0.1f)) * 3.0f + 8.0f);// 生成高度在 5-11 之间，形成起伏的地形
//                for (int y = 0; y < CHUNK_HEIGHT; ++y) {// 根据高度设置方块类型
//                    if (y > height) {// 高于地面部分设置为空气
//                        m_blocks[x][y][z] = BLOCK_AIR;// 空气
//                    }
//                    else if (y == height) {// 地面部分设置为草地
//                        m_blocks[x][y][z] = BLOCK_GRASS;// 草地
//                    }
//                    else if (y > height - 3) {// 地面下方3层设置为泥土
//                        m_blocks[x][y][z] = BLOCK_DIRT;// 泥土
//                    }
//                    else {
//                        m_blocks[x][y][z] = BLOCK_STONE;// 更深层设置为石头
//                    }
//                }
//            }
//        }
//        generateMesh();// 生成网格数据
//        uploadMesh();// 上传到 GPU
//    }
//
//    ~Chunk() {
//        if (m_vao) glDeleteVertexArrays(1, &m_vao);
//        if (m_vbo) glDeleteBuffers(1, &m_vbo);
//        if (m_ebo) glDeleteBuffers(1, &m_ebo);
//    }
//
//    void render() const {
//        if (m_indexCount == 0) return;
//        glBindVertexArray(m_vao);
//        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//    }
//
//    // 获取区块的 AABB（用于视锥剔除）
//    void getAABB(glm::vec3& min, glm::vec3& max) const {
//        min = glm::vec3(m_cx * CHUNK_SIZE, 0, m_cz * CHUNK_SIZE);
//        max = min + glm::vec3(CHUNK_SIZE, CHUNK_HEIGHT, CHUNK_SIZE);
//    }
//
//private:
//    int m_cx, m_cz;
//    GLuint m_vao, m_vbo, m_ebo;
//    GLsizei m_indexCount;
//    std::array<std::array<std::array<BlockType, CHUNK_SIZE>, CHUNK_HEIGHT>, CHUNK_SIZE> m_blocks;
//
//    // 根据方块类型获取纹理坐标（UV范围）
//    void getBlockUV(BlockType type, glm::vec2& uvMin, glm::vec2& uvMax) {
//        // 假设纹理图集为 2x2 网格，0:草,1:土,2:石
//        float texWidth = 2.0f, texHeight = 2.0f;
//        int col = 0, row = 0;
//        switch (type) {
//        case BLOCK_GRASS: col = 0; row = 0; break;
//        case BLOCK_DIRT:  col = 1; row = 0; break;
//        case BLOCK_STONE: col = 0; row = 1; break;
//        default: col = 0; row = 0; break;
//        }
//        float u0 = col / texWidth;
//        float v0 = row / texHeight;
//        float u1 = (col + 1) / texWidth;
//        float v1 = (row + 1) / texHeight;
//        uvMin = glm::vec2(u0, v0);
//        uvMax = glm::vec2(u1, v1);
//    }
//
//    // 添加一个面的顶点和索引
//    void addFace(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,
//        const glm::vec3& pos, const glm::vec3& normal,
//        const glm::vec2& uvMin, const glm::vec2& uvMax) {
//        // 根据法线方向确定四个顶点的位置
//        float x = pos.x, y = pos.y, z = pos.z;// 方块尺寸为1单位
//        float w = 1.0f, h = 1.0f;// 四个顶点位置
//        glm::vec3 v0, v1, v2, v3;// 四个顶点位置
//        glm::vec2 uv0, uv1, uv2, uv3;
//
//        if (normal.x == 1) { // 右面 (+x)
//            v0 = { x + w, y,   z };   v1 = { x + w, y,   z + w };
//            v2 = { x + w, y + h, z + w }; v3 = { x + w, y + h, z };
//            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
//            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
//        }
//        else if (normal.x == -1) { // 左面 (-x)
//            v0 = { x, y,   z + w }; v1 = { x, y,   z };
//            v2 = { x, y + h, z }; v3 = { x, y + h, z + w };
//            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
//            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
//        }
//        else if (normal.y == 1) { // 上面 (+y)
//            v0 = { x,   y + h, z };   v1 = { x + w, y + h, z };
//            v2 = { x + w, y + h, z + w }; v3 = { x,   y + h, z + w };
//            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
//            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
//        }
//        else if (normal.y == -1) { // 下面 (-y)
//            v0 = { x,   y, z + w }; v1 = { x + w, y, z + w };
//            v2 = { x + w, y, z };   v3 = { x,   y, z };
//            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
//            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
//        }
//        else if (normal.z == 1) { // 前面 (+z)
//            v0 = { x + w, y,   z + w }; v1 = { x,   y,   z + w };
//            v2 = { x,   y + h, z + w }; v3 = { x + w, y + h, z + w };
//            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
//            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
//        }
//        else if (normal.z == -1) { // 后面 (-z)
//            v0 = { x,   y,   z }; v1 = { x + w, y,   z };
//            v2 = { x + w, y + h, z }; v3 = { x,   y + h, z };
//            uv0 = { uvMin.x, uvMin.y }; uv1 = { uvMax.x, uvMin.y };
//            uv2 = { uvMax.x, uvMax.y }; uv3 = { uvMin.x, uvMax.y };
//        }
//        else {
//            return;
//        }
//
//        GLuint base = vertices.size();
//        vertices.push_back({ v0, uv0, normal });
//        vertices.push_back({ v1, uv1, normal });
//        vertices.push_back({ v2, uv2, normal });
//        vertices.push_back({ v3, uv3, normal });
//
//        indices.push_back(base);
//        indices.push_back(base + 1);
//        indices.push_back(base + 2);
//        indices.push_back(base);
//        indices.push_back(base + 2);
//        indices.push_back(base + 3);
//    }
//
//    void generateMesh() {
//        std::vector<Vertex> vertices;// 存储顶点数据
//        std::vector<GLuint> indices;// 存储索引数据
//
//        for (int x = 0; x < CHUNK_SIZE; ++x) {// 遍历区块内的每个方块位置
//            for (int y = 0; y < CHUNK_HEIGHT; ++y) {// 遍历区块内的每个方块位置
//                for (int z = 0; z < CHUNK_SIZE; ++z) {// 获取方块类型
//                    BlockType type = m_blocks[x][y][z];// 如果是空气，跳过
//                    if (type == BLOCK_AIR) continue;// 计算世界坐标（用于生成顶点位置）
//
//                    glm::vec3 worldPos = glm::vec3(m_cx * CHUNK_SIZE + x, y, m_cz * CHUNK_SIZE + z);// 获取纹理坐标
//                    glm::vec2 uvMin, uvMax;// 根据方块类型获取纹理坐标
//                    getBlockUV(type, uvMin, uvMax);// 根据方块类型获取纹理坐标
//
//                    // 检查六个方向是否有相邻方块，没有则添加面
//                    // 右 (x+1)
//                    if (x + 1 >= CHUNK_SIZE || m_blocks[x + 1][y][z] == BLOCK_AIR)
//                        addFace(vertices, indices, worldPos, glm::vec3(1, 0, 0), uvMin, uvMax);
//                    // 左 (x-1)
//                    if (x - 1 < 0 || m_blocks[x - 1][y][z] == BLOCK_AIR)
//                        addFace(vertices, indices, worldPos, glm::vec3(-1, 0, 0), uvMin, uvMax);
//                    // 上 (y+1)
//                    if (y + 1 >= CHUNK_HEIGHT || m_blocks[x][y + 1][z] == BLOCK_AIR)
//                        addFace(vertices, indices, worldPos, glm::vec3(0, 1, 0), uvMin, uvMax);
//                    // 下 (y-1)
//                    if (y - 1 < 0 || m_blocks[x][y - 1][z] == BLOCK_AIR)
//                        addFace(vertices, indices, worldPos, glm::vec3(0, -1, 0), uvMin, uvMax);
//                    // 前 (z+1)
//                    if (z + 1 >= CHUNK_SIZE || m_blocks[x][y][z + 1] == BLOCK_AIR)
//                        addFace(vertices, indices, worldPos, glm::vec3(0, 0, 1), uvMin, uvMax);
//                    // 后 (z-1)
//                    if (z - 1 < 0 || m_blocks[x][y][z - 1] == BLOCK_AIR)
//                        addFace(vertices, indices, worldPos, glm::vec3(0, 0, -1), uvMin, uvMax);
//                }
//            }
//        }
//
//        // 存储顶点和索引数据到成员变量，用于上传
//        m_vertices = vertices;// 存储顶点数据
//        m_indices = indices;// 存储索引数据
//        m_indexCount = indices.size();// 存储索引数量
//    }
//
//    void uploadMesh() {
//        if (m_vertices.empty() || m_indices.empty()) return;
//
//        glGenVertexArrays(1, &m_vao);
//        glGenBuffers(1, &m_vbo);
//        glGenBuffers(1, &m_ebo);
//
//        glBindVertexArray(m_vao);
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
//
//        // 位置
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
//        glEnableVertexAttribArray(0);
//        // 纹理坐标
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
//        glEnableVertexAttribArray(1);
//        // 法线
//        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//        glEnableVertexAttribArray(2);
//
//        glBindVertexArray(0);
//        // 清空CPU侧数据，释放内存
//        m_vertices.clear();
//        m_indices.clear();
//    }
//
//    std::vector<Vertex> m_vertices;
//    std::vector<GLuint> m_indices;
//};