#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "TextRenderer.h"
#include "GameData.h"
#include "SpriteRenderer.h"
#include "Shader.h"
#include "Vertex.h"


#include "MapData.h"
#include "Block.h"
#include "Image.h"
#include "GameUnit.h"


struct DynamicObject {
	glm::vec3 position = glm::vec3(0.0f);//位置
	glm::vec3 velocity = glm::vec3(0.0f);//速度
	glm::vec3 size = glm::vec3(0.8f, 0.8f, 0.8f);//尺寸
	float rotationY = 0.0f;//旋转角
	AtlasRegion* textureRegion = nullptr; // 指向图集区域（用于纹理）s
	bool active = true;//是否存在
};


class Game {
public:
	Game();
	~Game();

	// 初始化 、着色器、纹理等
	bool init();

	// 创建世界（指定渲染半径，区块数量为 (2*radius+1)^2）
	void createWorld(int radius);

	// 运行主循环
	void run();

	// 清理资源
	void cleanup();

	//std::vector <Unit> UnitData;//单位数据
	WorldMap MapData;//世界数据
	SpriteRenderer spriteRenderer;// 精灵渲染器
	TextRenderer textRenderer;    // 文本渲染器
private:
	//Shader SH;//创建着色器
	Vertex vertex;//创建顶点对象
	GLuint m_shaderProgram;// 着色器程序 ID
	//std::vector<std::unique_ptr<Block>> m_chunks;// 区块列表
	std::vector<DynamicObject> m_dynamicObjects;//单位


	// 内部辅助函数
	void processInput(float dt);
	bool isChunkVisibleTo(const Block* chunk, const glm::mat4& vp);
	bool createShaderProgram();




	//// ---------- 鼠标移动 ----------
	//void mouse_callback_G(GLFWwindow* window, double xpos, double ypos);
	//// ---------- 鼠标滚轮 ----------
	//void scroll_callback_G(GLFWwindow* window, double xoffset, double yoffset);
	//// ---------- 鼠标滚动点击 ------
	//void mouse_button_callback_G(GLFWwindow* window, int button, int action, int mods);
	//// ---------- 窗口回调 ----------
	//void framebuffer_size_callback_G(GLFWwindow* window, int width, int height);
};

//extern Game engine;