#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cstring>


#include "Debug.h"
#include "Block.h"




constexpr uint32_t MAGIC = 0x59434650;           // 文件校准码
constexpr uint16_t VERSION = 1;                  // 版本


// ====================== 世界容器 ======================
class WorldMap {
public:
    std::string MapName;    //地图名字
    std::string MapWay;     //地图路径
    std::string MapSaveWay; //地图保存路径


    float g; //重力加速度


    std::unordered_map<int64_t, Block> blocks;  //区块数据
    int blockShift = 4;  // 2^4 = 16

    // 根据区块坐标生成唯一键
    static int64_t makeKey(int32_t cx, int32_t cz);

    // 获取或创建区块
    Block& getOrCreateChunk(int32_t cx, int32_t cz);
    // 获取区块列表
    std::unordered_map<int64_t, Block>& GetBlockAll();

    // 设置方块（自动定位区块）
    void setBlock(int32_t worldX, int32_t worldY, int32_t worldZ, BlockID id);
    // 获取方块（自动定位区块）
    BlockID GetBlock(int32_t worldX, int32_t worldY, int32_t worldZ);

    // 保存到二进制文件
    bool saveToFile(const std::string& filename);
    // 从二进制文件加载
    bool loadFromFile(const std::string& filename);

    // 关闭 
    void clear();
};
