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
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);// 打开文件（二进制模式，覆盖写入）
    if (!file) return false;// 打开失败

    // 文件头
    file.write(reinterpret_cast<const char*>(&MAGIC), 4);        // 文件校准码
    file.write(reinterpret_cast<const char*>(&VERSION), 2);      // 版本


    // --- 全局字典 ---
    uint32_t DicSize = static_cast<uint32_t>(CentralData.BlockData.size());
    file.write(reinterpret_cast<const char*>(&DicSize), 4);//字典 数据大小
    for (uint16_t i = 0; i < CentralData.BlockData.size(); i++) {
        std::string str = CentralData.BlockData[i].Name;      // 获取字符串
        uint32_t len = static_cast<uint32_t>(str.size());     // 获取字符串长度
        file.write(reinterpret_cast<const char*>(&len), 4);   // 写入长度
        file.write(str.data(), len);                          // 写入 字符串

        file.write(reinterpret_cast<const char*>(&i), 4);     // 字典信息
    }


    uint32_t chunkCount = static_cast<uint32_t>(blocks.size());  
    file.write(reinterpret_cast<const char*>(&chunkCount), 4);   // 区块数量

    // 每个区块先编码再写入
    for (auto& pair : blocks) {
        Block& block = pair.second;// 获取区块
        block.encodeRLE();         // 生成 palette 与 rleData

        // 写入区块头：坐标 + 压缩数据大小
        file.write(reinterpret_cast<const char*>(&block.m_cx), 4);// 区块坐标
        file.write(reinterpret_cast<const char*>(&block.m_cz), 4);// 区块坐标

        // 调色板大小与数据
        uint16_t paletteSize = static_cast<uint16_t>(block.palette.size());// 调色板大小（最多 256 种方块类型）
        file.write(reinterpret_cast<const char*>(&paletteSize), 2);// 写入调色板大小
        if (paletteSize > 0) {// 写入调色板数据（BlockID 列表）
            file.write(reinterpret_cast<const char*>(block.palette.data()), static_cast<std::streamsize>(paletteSize) * static_cast<std::streamsize>(sizeof(BlockID)));// 写入调色板数据（BlockID 列表）
        }

        // RLE 数据大小与数据
        uint32_t rleSize = static_cast<uint32_t>(block.rleData.size());
        file.write(reinterpret_cast<const char*>(&rleSize), 4);// 写入 RLE 数据大小
        file.write(reinterpret_cast<const char*>(block.rleData.data()), rleSize);// 写入 RLE 数据
    }

    file.close();// 关闭文件
    return true;//  成功保存
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



    // --- 全局字典 ---
    std::unordered_map<std::string, uint16_t> Dictionary;
    const uint32_t MAX_LEN = 10 * 1024 * 1024; // 10 MB
    uint32_t DicSize = 0;
    file.read(reinterpret_cast<char*>(&DicSize), 4);//字典 数据大小
    for (uint16_t i = 0; i < DicSize; i++) {
        uint32_t len = 0;
        file.read(reinterpret_cast<char*>(&len), 4);
        if (!file) Error("读取长度失败", "W");
        // 安全检查：限制最大长度，避免恶意或损坏的文件导致内存爆炸
        if (len > MAX_LEN) Error("字符串长度超限", "W");
        std::string str(len, '\0');          // 预分配空间
        file.read(&str[0], len);              // C++11 起 &s[0] 指向连续内存
        if (!file) Error("读取字符串数据失败", "W");

        uint16_t dic = 0;
        file.read(reinterpret_cast<char*>(&dic), 4);// 字典信息

        Dictionary.emplace(str, dic);// 存储到字典中
    }
    for (int i = 0; i < CentralData.BlockData.size(); i++) {//分析字典 与原来的关系 比较 并建立新的关系
        auto it = Dictionary.find(CentralData.BlockData[i].Name);// 获取文件中的字典索引
        if (it != Dictionary.end()) {
            BlockDictionary.emplace(it->second, i);// 建立新的关系
        }
        else {
            Warn("地图文件中的字典项未找到: " + CentralData.BlockData[i].Name, "W");
        }
        Dictionary.erase(CentralData.BlockData[i].Name);// 从文件字典中移除已匹配项
    }
    uint16_t missingCount = 0;
    for (const auto& [key, val] : Dictionary) {
        Error("地图文件中的字典项未匹配到游戏数据: " + key, "W");
        missingCount++;
    }
    if(missingCount > 0) {
        Error("共有 " + std::to_string(missingCount) + " 个字典项未匹配到游戏数据", "W");
    }

    uint32_t chunkCount = 0;
    file.read(reinterpret_cast<char*>(&chunkCount), 4);// 读取区块数量
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