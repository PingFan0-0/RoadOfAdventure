#include "Game.h"


#include "Debug.h"
#include <string>
#include "Camera.h"
#include "TypeConversion.h"
#include "Time.h"
#include "GameUnit.h"
#include "DrawShape.h"
#include "Model.h"


//Game engine;




void ImageDisplay_G(float x, float y, float size, int img, SpriteRenderer& spriteRenderer) {//<-------------------------------------------------------------图片显示
    spriteRenderer.DrawSprite(atlas, regions[img], glm::vec2(x, y), glm::vec2(size, size));
}
void ImageDisplay_G(int x, int y, int size, SXY Size, int img, SpriteRenderer& spriteRenderer) {//<-------------------------------------------------------------图片显示
    spriteRenderer.DrawSprite(atlas, regions[img], glm::vec2((x - Size.X * size / 2), (y - Size.Y * size / 2)), glm::vec2((Size.X * size), (Size.Y * size)));
}






Game::Game()
    : m_shaderProgram(0),/* SH("vertexShader.glsl", "fragmentShader0.glsl"),*/
    vertex(verticesZ, verticesZ_size, indicesZ, verticesZ_size, propZ, 3){
    std::string FontsName = wstring_string(DataParent + L"/" + FontsParent) + "/" + FontMain;
    if (!textRenderer.Load(FontsName, textRenderer.FontSize)) {

    }

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Game::~Game() { cleanup(); }

// 顶点着色器和片段着色器源码
#pragma region 着色器 ===============
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    gl_Position = projection * view * worldPos;
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;
uniform sampler2D atlas;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 viewPos;
void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(lightDir);
    float diff = max(dot(norm, lightDirNorm), 0.2);
    vec3 ambient = vec3(0.3);
    vec3 diffuse = diff * lightColor;
    vec4 texColor = texture(atlas, TexCoord);
    vec3 result = (ambient + diffuse) * texColor.rgb;
    FragColor = vec4(result, 1.0);
}
)";

// 着色器
GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        Error("着色器编译错误; " + std::string(info), "W");
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

bool Game::createShaderProgram() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vs);// 附加顶点和片段着色器
    glAttachShader(m_shaderProgram, fs);// 链接程序
    glLinkProgram(m_shaderProgram);// 删除着色器对象（链接后不再需要）
    glDeleteShader(vs);// 检查链接错误
    glDeleteShader(fs);
    GLint success;// 获取链接状态
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);// 如果链接失败，获取错误日志并输出
    if (!success) {// 获取错误日志
        char info[512];// 输出错误日志
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, info);// 输出错误日志
        Error("程序链接错误; " + std::string(info), "W");
        glDeleteProgram(m_shaderProgram);// 删除程序对象
        return false;
    }

    return true;
}
#pragma endregion

// 初始化游戏，着色器等
bool Game::init() {
    if (!createShaderProgram()) return false;//编译链接
    //m_atlasTexture = createTestAtlas();// 生成 2x2 颜色纹理的代码（同上）
    //setupCallbacks();
    glEnable(GL_DEPTH_TEST);// 开启深度测试
    glEnable(GL_CULL_FACE);// 开启面剔除
    glCullFace(GL_BACK);// 剔除背面

    return true;
}


// 创建世界（指定渲染半径，区块数量为 (2*radius+1)^2）
void Game::createWorld(int radius) {
    //for (int i = 0; i < 4; i++) {
    //    for (int j = 0; j < 2; j++) {
    //        MapData.getOrCreateChunk(i, j);
    //    }
    //}
    if (FindFile(to_wstring(MapData.MapSaveWay), MapData.MapName) != "null") {//如果游玩过此地图
        std::string way = MapData.MapSaveWay + "/" + MapData.MapName;
        if (MapData.loadFromFile(way)) {
            Debug("加载地图 " + way);
        }
        else {
            Error("加载地图 " + way + " 失败", "W");
            return;
        }
    }
    else {//如果没有游玩过此地图
        std::string way = MapData.MapWay + "/" + MapData.MapName;
        if (MapData.loadFromFile(way)) {
            Debug("加载地图 " + way);
        }
        else {
            Error("加载地图 " + way + " 失败", "W");
            return;
        }
    }
}




#pragma region Input =================

glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
// ---------- 键盘输入 ----------
void Game::processInput(float dt) {
    float speed = 10.0f * dt;
    glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
    glm::vec3 frontHorizontal = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 move = glm::vec3(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        move += frontHorizontal;
        //cameraPos += frontHorizontal * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        move -= frontHorizontal;
        //cameraPos -= frontHorizontal * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        move -= right;
        //cameraPos -= right * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        move += right;
        //cameraPos += right * speed;
    }
    if (glm::length(move) > 0.0f)//归一化对角线移动
        move = glm::normalize(move);

    if (UnitData[Player.Player].BoolDEL) UnitData[Player.Player].ToF(move.x, move.z);
    else center += move * speed;

    // --------------- ESC 关闭 ---------------
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		YM = "begin";//切换到起始界面
    }
}


float radius = 0;
float yaw = -90.0f, pitch = -89.0f;
float lastX = (float)Win.WinX / 2, lastY = (float)Win.WinY / 2;
bool MouseToView = false;
// ---------- 鼠标移动 ----------
void mouse_callback_G(GLFWwindow* window, double xpos, double ypos) {
    //Debug(std::to_string(xpos) + " " + std::to_string(ypos));
    MouseX = xpos; MouseY = ypos;
    if (MouseToView) {
        if (UnitData[Player.Player].BoolDEL) {
            center = UnitData[Player.Player].Getglmvec3XYZ(); // 旋转中心
        }
        float radius = glm::distance(cameraPos, center);// 摄像机到中心的距离
        float xoffset = (float)xpos - lastX;
        float yoffset = (float)ypos - lastY;

        float sensitivity = 0.2f * 1.0f;//鼠标灵敏度

        yaw   += xoffset * sensitivity;// X
        pitch += yoffset * sensitivity;// Y

        // 限制俯仰角，避免万向锁和翻转
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }
    lastX = (float)xpos;//上次的坐标
    lastY = (float)ypos;
}
// ---------- 鼠标滚轮 ----------
void scroll_callback_G(GLFWwindow* window, double xoffset, double yoffset) {
    radius -= float(yoffset * Time.JGTime * 50.0f);
    if (radius < 0.5f) radius = 0.5f;
}
// ---------- 鼠标滚动点击 ------
void mouse_button_callback_G(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {// 检查按下的是否是滚轮（中键）
        MouseToView = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//切换鼠标模式
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {//松开
        MouseToView = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//切换鼠标模式
    }
}
// ---------- 窗口回调 ----------
void framebuffer_size_callback_G(GLFWwindow* window, int width, int height) {
    // 1. 更新视口
    glViewport(0, 0, width, height);

    DrawRectShape::GetInstance().UpdateProjection(width, height);//图形渲染器
    //Game::spriteRenderer.UpdateProjection(Win.WinX, Win.WinY);
    void* p = glfwGetWindowUserPointer(window);
    if (p) {
        Game* game = static_cast<Game*>(p);
        game->spriteRenderer.UpdateProjection(width, height);// 精灵渲染器
        game->textRenderer.UpdateProjection(width, height);  // 文本渲染器
    }
    // 2. 更新投影矩阵（正交投影，覆盖屏幕范围）
    //GLuint shader = *(GLuint*)glfwGetWindowUserPointer(window);
    //glUseProgram(shader);//使用着色器
    //glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
    //glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

    Win.WinX = width; Win.WinY = height;
}



#pragma endregion

// 视锥剔除：判断区块是否在视锥内
bool Game::isChunkVisibleTo(const Block* chunk, const glm::mat4& vp) {
    return true;
    glm::vec3 min = glm::vec3(0.0f);// 获取区块的 AABB 包围盒
    glm::vec3 max = glm::vec3(0.0f);// 获取区块的 AABB 包围盒
    chunk->getAABB(min, max);// 将 AABB 的中心点和半径转换到裁剪空间
    glm::vec4 center = vp * glm::vec4((min + max) * 0.5f, 1.0f);// 计算包围球半径（AABB 对角线的一半）
    float radius = glm::length(max - min) * 0.75f;// 在裁剪空间中，判断包围球是否与视锥相交（简单的球-平面测试）
    float w = center.w;// 如果包围球完全在近平面后面，或者在左右/上下平面外，则不可见
    return !(center.x < -w - radius || center.x > w + radius ||// 判断左右平面
        center.y < -w - radius || center.y > w + radius ||// 判断上下平面
        center.z < 0.0f - radius || center.z > w + radius);// 判断近平面和远平面
}

void Game::run() {
#pragma region 窗口 ===============
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_G);
#pragma endregion
#pragma region 相机 ===============
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));//模型矩阵
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));//视图矩阵
    projection = glm::perspective(glm::radians(60.0f), (float)Win.WinX / (float)Win.WinY, 0.1f, 100.0f);//投影矩阵
    cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);//摄像机朝向
    cameraPos = glm::vec3(0.0f, 10.0f, 0.0f);//Position
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//摄像机上方向
    yaw = 90.0f;//  仰角 X
    pitch = 89.0f;//俯角 Y
    center = UnitData[Player.Player].Getglmvec3XYZ(); // 旋转中心
    radius = glm::distance(cameraPos, center);// 摄像机到中心的距离
    glm::vec3 offset;
    offset.x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    offset.y = radius * sin(glm::radians(pitch));
    offset.z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraPos = center + offset;        // 摄像机位于球面上
    cameraFront = center - cameraPos;   // 视线指向中心
#pragma endregion

    float Tsize = 1.0f;//总的缩放比值

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    while (YM == "gameon") {
        TimeMath();// 计算时间

        processInput(Time.JGTime);// 处理输入
        glfwSetCursorPosCallback(window, mouse_callback_G);// 设置鼠标回调
        glfwSetMouseButtonCallback(window, mouse_button_callback_G);//设置鼠标滚轮点击回调函数
        glfwSetScrollCallback(window, scroll_callback_G);//设置鼠标滚轮回调函数


        for (int i = 0; i < UnitData.size(); i++)UnitData[i].UnitTo();//单位移动计算

        // ---------- 相机跟随 ----------
        if (UnitData[Player.Player].BoolDEL) {
            center = UnitData[Player.Player].Getglmvec3XYZ(); // 旋转中心
        }
        offset.x = radius * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        offset.y = radius * sin(glm::radians(pitch));
        offset.z = radius * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraPos = center + offset;        // 摄像机位于球面上
        cameraFront = center - cameraPos;   // 视线指向中心

        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除颜色和深度缓冲区

#pragma region 地图 =================

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//视图矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)Win.WinX / (float)Win.WinY, 0.1f, 1000.0f);// 投影矩阵
        glm::mat4 vp = projection * view;// 视图-投影矩阵

        glUseProgram(m_shaderProgram);// 使用着色器程序
        glUniform3f(glGetUniformLocation(m_shaderProgram, "lightDir"), 0.2f, 1.0f, 0.1f); // 光源方向（从上方斜照）
        glUniform3f(glGetUniformLocation(m_shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);// 光源颜色（白色）
        glUniform3f(glGetUniformLocation(m_shaderProgram, "ambientColor"), 0.2f, 0.2f, 0.2f);// 环境光颜色（灰色）

        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));// 传递视图矩阵
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));// 传递投影矩阵
        glUniform3fv(glGetUniformLocation(m_shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));// 传递摄像机位置
        glActiveTexture(GL_TEXTURE0);// 激活纹理单元 0
        glBindTexture(GL_TEXTURE_2D, atlas);

        for (auto& [id, chunk] : MapData.GetBlockAll()){// 遍历区块列表，进行视锥剔除和渲染
            if (isChunkVisibleTo(&chunk, vp)){// 如果区块在视锥内，设置模型矩阵并渲染
                glm::mat4 model = glm::mat4(1.0f);// 模型矩阵（区块位置）
                glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));// 传递模型矩阵
                chunk.render();// 渲染区块
            }
        }
#pragma endregion
#pragma region 单位 =================

        //for (const auto& obj : m_dynamicObjects) {
        //    renderDynamicObject(obj);
        //}

#pragma endregion
        glDisable(GL_DEPTH_TEST);// 2D UI 渲染通常不需要深度测试
        glEnable(GL_BLEND);// 启用混合
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 设置混合函数
#pragma region 左上 =================
        {
            GLfloat ULY = 0;
            GLfloat h = 5.0f;
            float size = std::min(0.4f, std::min(Win.WinX / 1200.0f, Win.WinY / 900.0f)) * Tsize;
            if (BoolFPS) {//=====FPS显示
                textRenderer.RenderText(
                    "FPS: " + std::to_string(FPS) + " " + std::to_string(Time.JGTime)
                    , 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
                ULY++;
            }
            if (BoolZbxs) {
                textRenderer.RenderText(//中心XY
                    "centerPOS: " + std::to_string((int)center.x) + " " + std::to_string((int)center.y) + " " + std::to_string((int)center.z)
                    , 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
                ULY++;
            }
            if (BoolZbxs) {
                textRenderer.RenderText(//相机XY
                    "cameraPOS: " + std::to_string((int)cameraPos.x) + " " + std::to_string((int)cameraPos.y) + " " + std::to_string((int)cameraPos.z)
                    , 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
                ULY++;
            }
            if (BoolVxs) {
                SXY a = UnitData[Player.Player].GetV();//V
                textRenderer.RenderText(
                    "V:   " + std::to_string(a.X) + " " + std::to_string(a.Y)
                    , 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
                ULY++;
            }
            {
                SXY a = UnitData[Player.Player].GetV();//V
                textRenderer.RenderText(
                    "滚轮按键:  " + std::string((MouseToView) ? "true" : "false")
                    , 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size
                    , (MouseToView) ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f));
                ULY++;
            }
            {
                textRenderer.RenderText(
                    "鼠标坐标:  " + std::to_string((int)MouseX) + " " + std::to_string((int)MouseY)
                    , 10.0f, textRenderer.FontSize * ULY * size + h, 1.0f * size, glm::vec3(1.0f));
                ULY++;
            }
        }
#pragma endregion
#pragma region 左下 =================
        {
            GLfloat DLY = 0.0f;
            float size = 0.4f;
            float X = std::min((float)Win.WinX / 5, (float)(Tsize * 133));         //宽
            float Y = std::min((float)Win.WinY / 5, (float)(Tsize * 100));         //高
            X = std::min(Y * 4 / 3, X);
            Y = std::min(X * 3 / 4, Y);


            float L = (float)std::min(Win.WinX / 50, 10);            //左
            float R = L + X;                                         //右
            float D = Win.WinY - (float)std::min(Win.WinY / 50, 10); //下
            float U = D - Y;                                         //上
            
            DrawRectShape::GetInstance().DrawRect(L, U, R, D,
                glm::vec4(0.25f, 0.25f, 0.25f, 0.5f), true);//背景 - 外
            DrawRectShape::GetInstance().DrawRect(L, U, R, D,
                glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, 2.0f);//边框 - 外

            SNMfloat A = UnitData[Player.Player].GetLife();//获取生命

            {
                float L1 = L + X / 15;
                float R1 = R - X / 15;
                float U1 = U + Y / 8;
                float D1 = U + Y / 4;
                float X1 = R1 - L1;
                float Y1 = D1 - U1;
                //血条
                SNMfloat A = UnitData[Player.Player].GetLife();
                DrawRectShape::GetInstance().DrawRect(L1, U1, L1 + X1 * A.Now / A.Max, D1,
                    glm::vec4(0.0f, 0.8f, 0.0f, 0.5f), true);//实心矩形.
                DrawRectShape::GetInstance().DrawRect(L1, U1, R1, D1,
                    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, 2.0f);//空心矩形
                //玩家贴图
                float Size1 = (Y / 5);
                U1 = D1 + Y / 8;
                if (UnitData[Player.Player].BoolDEL) {
                    ImageDisplay_G(L1, U1, Size1, CentralData.UnitData[UnitData[Player.Player].GetHand()].ImageHand, spriteRenderer);
                }
                DrawRectShape::GetInstance().DrawRect(L1, U1, L1 + Size1, U1 + Size1,
                    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), false, 2.0f);//空心矩形
            }
        }
#pragma endregion

        glEnable(GL_DEPTH_TEST);// 恢复深度测试状态

        glfwSwapBuffers(window);//渲染
        glfwPollEvents();
    }

    std::string way = MapData.MapSaveWay + "/" + MapData.MapName;
    if (MapData.saveToFile(way)) {
        Debug("保存地图 " + way);
    }
    else {
        Error("保存地图 " + way + "失败", "W");
        return;
    }
}



// 清理资源
void Game::cleanup() {
    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);//删除着色器
}
