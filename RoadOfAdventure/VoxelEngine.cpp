//#include "VoxelEngine.h"
//#include "Chunk.h"  
//#include <iostream>
//
//
//
//
//// 静态回调函数（因为 GLFW 需要 C 风格函数）
////static VoxelEngine* g_engine = nullptr;
////static void glfw_mouse_callback(GLFWwindow* win, double xpos, double ypos) {
////    if (g_engine) g_engine->mouseCallback(xpos, ypos);
////}
//
//VoxelEngine::VoxelEngine()
//    : m_window(nullptr), m_shaderProgram(0), m_atlasTexture(0),//	m_screenWidth(800), m_screenHeight(600),
//    m_cameraPos(0.0f, 20.0f, 20.0f), m_cameraFront(0.0f, -0.5f, -1.0f),
//    m_cameraUp(0.0f, 1.0f, 0.0f), m_yaw(-90.0f), m_pitch(-20.0f),
//    m_firstMouse(true), m_lastX(0), m_lastY(0) {
//}
//
//VoxelEngine::~VoxelEngine() { cleanup(); }
//
//bool VoxelEngine::init(int width, int height, const char* title) {
//    m_screenWidth = width;
//    m_screenHeight = height;
//    if (!initGLFW() || !initOpenGL()) return false;
//    if (!createShaderProgram()) return false;
//    m_atlasTexture = createTestAtlas();
//    //setupCallbacks();
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    return true;
//}
//
//bool VoxelEngine::initGLFW() {
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "Voxel Engine", nullptr, nullptr);
//    if (!m_window) return false;
//    glfwMakeContextCurrent(m_window);
//    return true;
//}
//
//bool VoxelEngine::initOpenGL() {
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return false;// 初始化 GLAD
//    return true;
//}
//
//bool VoxelEngine::createShaderProgram() {
//    // 这里嵌入上一轮的顶点/片段着色器源码，编译链接
//    // ... 与之前相同的 compileShader 和 linkProgram 代码 ...
//    // 返回是否成功
//    return true;
//}
//
//GLuint VoxelEngine::createTestAtlas() {
//    // 生成 2x2 颜色纹理的代码（同上）
//    // ...
//}
//
////void VoxelEngine::setupCallbacks() {//
////    glfwSetCursorPosCallback(m_window, glfw_mouse_callback);
////    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
////    g_engine = this;
////}
//
////void VoxelEngine::mouseCallback(double xpos, double ypos) {
////    if (m_firstMouse) {
////        m_lastX = xpos;
////        m_lastY = ypos;
////        m_firstMouse = false;
////    }
////    float xoffset = xpos - m_lastX;
////    float yoffset = m_lastY - ypos;
////    m_lastX = xpos;
////    m_lastY = ypos;
////    float sensitivity = 0.1f;
////    xoffset *= sensitivity;
////    yoffset *= sensitivity;
////    m_yaw += xoffset;
////    m_pitch += yoffset;
////    if (m_pitch > 89.0f) m_pitch = 89.0f;
////    if (m_pitch < -89.0f) m_pitch = -89.0f;
////    updateCameraVectors();
////}
//
//void VoxelEngine::updateCameraVectors() {
//    glm::vec3 front;
//    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
//    front.y = sin(glm::radians(m_pitch));
//    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
//    m_cameraFront = glm::normalize(front);
//}
//
//void VoxelEngine::processInput(float dt) {
//    float speed = 10.0f * dt;
//    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
//        m_cameraPos += speed * m_cameraFront;// 沿前方向移动
//    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
//        m_cameraPos -= speed * m_cameraFront;// 沿前方向反向移动
//    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
//        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;// 沿右方向反向移动
//    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
//        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;// 沿右方向移动
//}
//
//void VoxelEngine::createWorld(int radius) {
//    m_chunks.clear();// 清除旧的区块
//    for (int cx = -radius; cx <= radius; ++cx) {// 以玩家为中心创建区块
//        for (int cz = -radius; cz <= radius; ++cz) {// 创建区块并添加到列表
//            m_chunks.push_back(std::make_unique<Chunk>(cx, cz));// 这里的 Chunk 构造函数会生成方块数据并上传到 GPU
//        }
//    }
//}
//
//bool VoxelEngine::isChunkVisible(const Chunk* chunk, const glm::mat4& vp) const {
//    glm::vec3 min, max;
//    chunk->getAABB(min, max);
//    glm::vec4 center = vp * glm::vec4((min + max) * 0.5f, 1.0f);
//    float radius = glm::length(max - min) * 0.5f;
//    float w = center.w;
//    return !(center.x < -w || center.x > w ||
//        center.y < -w || center.y > w ||
//        center.z < 0.0f || center.z > w);
//}
//
//void VoxelEngine::run() {
//    float lastFrame = 0.0f;// 用于计算帧时间
//    while (!glfwWindowShouldClose(m_window)) {
//        float currentFrame = glfwGetTime();// 获取当前时间
//        float dt = currentFrame - lastFrame;// 计算帧时间
//        lastFrame = currentFrame;// 更新上一帧时间
//
//        processInput(dt);// 处理输入
//
//        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);// 设置背景色
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除颜色和深度缓冲
//
//        glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);// 视图矩阵
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f),// 45度视野，宽高比，近远裁剪面
//            (float)m_screenWidth / m_screenHeight, 0.1f, 1000.0f);// 投影矩阵
//        glm::mat4 vp = projection * view;// 视图投影矩阵
//
//        glUseProgram(m_shaderProgram);// 使用着色器程序
//        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));// 传递视图矩阵
//        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//        glUniform3fv(glGetUniformLocation(m_shaderProgram, "viewPos"), 1, glm::value_ptr(m_cameraPos));
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, m_atlasTexture);
//
//        for (auto& chunk : m_chunks) {
//            if (isChunkVisible(chunk.get(), vp)) {
//                glm::mat4 model = glm::mat4(1.0f);
//                glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
//                chunk->render();
//            }
//        }
//
//        glfwSwapBuffers(m_window);
//        glfwPollEvents();
//    }
//}
//
//void VoxelEngine::cleanup() {
//    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);// 删除着色器程序
//    if (m_atlasTexture) glDeleteTextures(1, &m_atlasTexture);
//    if (m_window) glfwDestroyWindow(m_window);
//    glfwTerminate();
//}