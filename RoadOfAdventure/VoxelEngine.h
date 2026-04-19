//#pragma once
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <vector>
//#include <memory>
//
//
//class VoxelEngine {
//public:
//    VoxelEngine();
//    ~VoxelEngine();
//
//    // 初始化窗口、OpenGL、着色器、纹理等
//    bool init(int screenWidth, int screenHeight, const char* title);
//
//    // 创建世界（指定渲染半径，区块数量为 (2*radius+1)^2）
//    void createWorld(int radius);
//
//    // 运行主循环
//    void run();
//
//    // 清理资源
//    void cleanup();
//
//private:
//    GLFWwindow* m_window;
//    GLuint m_shaderProgram;
//    GLuint m_atlasTexture;
//    std::vector<std::unique_ptr<Chunk>> m_chunks;
//    int m_screenWidth, m_screenHeight;
//
//    // 相机控制
//    glm::vec3 m_cameraPos;
//    glm::vec3 m_cameraFront;
//    glm::vec3 m_cameraUp;
//    float m_yaw, m_pitch;
//    bool m_firstMouse;
//    float m_lastX, m_lastY;
//
//    // 内部辅助函数
//    bool initGLFW();
//    bool initOpenGL();
//    bool createShaderProgram();
//    GLuint createTestAtlas();   // 生成测试纹理（可替换为加载真实图集）
//    //void setupCallbacks();// 设置 GLFW 回调函数
//    void processInput(float dt);
//    void updateCameraVectors();
//    bool isChunkVisible(const Chunk* chunk, const glm::mat4& vp) const;
//};