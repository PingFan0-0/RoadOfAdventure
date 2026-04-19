#include "MapData.h"
/*
*/

// 全局世界地图实例
WorldMap World;







// 根据区块坐标生成唯一键
int64_t WorldMap::makeKey(int32_t cx, int32_t cz) {
    return (static_cast<int64_t>(cx) << 30) |
        (static_cast<int64_t>(cz) & 0xFFFFF);
}

// 获取或创建区块
Block& WorldMap::getOrCreateChunk(int32_t cx, int32_t cz) {
    int64_t key = makeKey(cx, cz);
    auto it = blocks.find(key);
    if (it != blocks.end()) return it->second;
    blocks.emplace(key, Block(cx, cz));
    return blocks[key];
}

// 获取区块列表
std::unordered_map<int64_t, Block>& WorldMap::GetBlockAll() {
    return blocks;
}


// 设置方块（自动定位区块）
void WorldMap::setBlock(int32_t worldX, int32_t worldY, int32_t worldZ, BlockID id) {
    int32_t cx = worldX >> blockShift;
    //int32_t cy = worldY >> chunkShift;
    int32_t cz = worldZ >> blockShift;
    int lx = worldX & (CHUNK_SIZE - 1);
    //int ly = worldY & (CHUNK_SIZE - 1);
    int lz = worldZ & (CHUNK_SIZE - 1);
    Block& chunk = getOrCreateChunk(cx, cz);
    chunk.setBlock(lx, worldY, lz, id);
}
// 获取方块
BlockID WorldMap::GetBlock(int32_t worldX, int32_t worldY, int32_t worldZ) {
    int32_t cx = worldX >> blockShift;
    int32_t cz = worldZ >> blockShift;
    int lx = worldX & (CHUNK_SIZE - 1);
    int lz = worldZ & (CHUNK_SIZE - 1);
    return getOrCreateChunk(cx, cz).getBlock(lx, worldY, lz);
     
}




// 保存到二进制文件
bool WorldMap::saveToFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (!file) return false;

    // 文件头
    file.write(reinterpret_cast<const char*>(&MAGIC), 4);
    file.write(reinterpret_cast<const char*>(&VERSION), 2);
    uint32_t chunkCount = static_cast<uint32_t>(blocks.size());
    file.write(reinterpret_cast<const char*>(&chunkCount), 4);

    // 每个区块先编码再写入
    for (auto& pair : blocks) {
        Block& block = pair.second;
        block.encodeRLE();  // 生成 palette 与 rleData

        // 写入区块头：坐标 + 压缩数据大小
        file.write(reinterpret_cast<const char*>(&block.m_cx), 4);
        file.write(reinterpret_cast<const char*>(&block.m_cz), 4);

        // 调色板大小与数据
        uint16_t paletteSize = static_cast<uint16_t>(block.palette.size());
        file.write(reinterpret_cast<const char*>(&paletteSize), 2);
        if (paletteSize > 0) {
            file.write(reinterpret_cast<const char*>(block.palette.data()), static_cast<std::streamsize>(paletteSize) * static_cast<std::streamsize>(sizeof(BlockID)));
        }

        // RLE 数据大小与数据
        uint32_t rleSize = static_cast<uint32_t>(block.rleData.size());
        file.write(reinterpret_cast<const char*>(&rleSize), 4);
        file.write(reinterpret_cast<const char*>(block.rleData.data()), rleSize);
    }

    file.close();
    return true;
}

// 从二进制文件加载
bool WorldMap::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    // 读取并验证文件头
    uint32_t magic = 0;
    uint16_t version = 0;
    file.read(reinterpret_cast<char*>(&magic), 4);
    file.read(reinterpret_cast<char*>(&version), 2);
    if (magic != MAGIC || version != VERSION) {
        if (magic != MAGIC)     Error("无法加载地图 无效的文件格式", "W");
        if (version != VERSION) Warn("无法加载地图 无法解析地图版本", "W");
        return false;
    }

    uint32_t chunkCount = 0;
    file.read(reinterpret_cast<char*>(&chunkCount), 4);
    blocks.clear();

    for (uint32_t i = 0; i < chunkCount; ++i) {
        Block block;
        file.read(reinterpret_cast<char*>(&block.m_cx), 4);
        file.read(reinterpret_cast<char*>(&block.m_cz), 4);

        
        uint16_t paletteSize = 0;
        file.read(reinterpret_cast<char*>(&paletteSize), 2);
        block.palette.resize(paletteSize);
        if (paletteSize > 0) {
            file.read(reinterpret_cast<char*>(block.palette.data()), static_cast<std::streamsize>(paletteSize) * static_cast<std::streamsize>(sizeof(BlockID)));
        }

        uint32_t rleSize = 0;
        file.read(reinterpret_cast<char*>(&rleSize), 4);
        block.rleData.resize(rleSize);
        file.read(reinterpret_cast<char*>(block.rleData.data()), rleSize);

        // 解码还原 blocks
        block.decodeRLE();

        // 获取区块 精准地址
        int64_t key = makeKey(block.m_cx, block.m_cz);
        blocks[key] = std::move(block);

        // 刷新网格数据
        blocks[key].MeshData();
    }

    file.close();
    return true;
}



void WorldMap::clear() {
    blocks.clear();
}