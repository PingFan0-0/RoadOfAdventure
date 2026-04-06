#include "OpenGL.h"

#include"GameData.h"
#include"Debug.h"
#include<fstream>
#include"TypeConversion.h"


bool BoolMod = false;//是否线框模式
GLFWwindow* window;//窗口对象



bool GLBegin() {//窗口初始化
	glfwInit();
	
	// 设置 OpenGL 版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	
	// 初始化窗口
	window = glfwCreateWindow(Win.WinX, Win.WinY, "RoadOfAdventure", nullptr, nullptr);
	if (!window) {
	    glfwTerminate();
	    return false;
	}
	glfwMakeContextCurrent(window);
	
	// 初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return false;
	}

	return true;
}

void GLEnd() {//关闭窗口
    glfwDestroyWindow(window);
    glfwTerminate();
}


void OpenGLMousePos() {//更新鼠标位置
	glfwGetCursorPos(window, &MouseX, &MouseY);
}
//打开着色器文件
#pragma region
std::string LoadShader(std::string name) {
	Debug("读取着色器 " + name);
	std::string ret = "";
	std::ifstream PF(wstring_string(DataParent + L"/" + ShaderParent) + "/" + name);//打开文件
	if (PF.is_open()) {//文件是否打开
		std::stringstream buf;
		buf << PF.rdbuf();//写入文件
		ret = buf.str();//读取字符
	}
	else {
		Error("着色器打开失败 " + name, "W");
	}
	PF.close();//关闭
	return ret;
}
#pragma endregion


//void GLDrawText(float x, float y, const std::string& text, void* font, float r, float g, float b) {
//	// 保存当前颜色和矩阵状态
//	glPushMatrix();
//	glPushAttrib(GL_CURRENT_BIT);
//
//	// 设置颜色
//	glColor3f(r, g, b);
//
//	// 设置光栅位置（世界坐标）
//	glRasterPos2f(x, y);
//
//	// 逐个字符绘制
//	for (char c : text) {
//		glutBitmapCharacter(font, c);
//	}
//
//	// 恢复状态
//	glPopAttrib();
//	glPopMatrix();
//}