#include "GameInput.h"


#include"GameData.h"
#include"GamePlayerMove.h"
#include"Debug.h"


#include"OpenGL.h"
#include"UIClickableText.h"

#include"GameUnit.h"

#include"Camera.h"

extern UIClickableTextManager& uiManager;


// ---------- 键盘输入 ----------------
void processInput_Game(GLFWwindow* window, float JGTime) {
	/*
	
	// ---------- 退出 ----------
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {// ESC 关闭
		YM = "begin";//返回开始菜单
	}


	// ---------- 移动 ----------
	int xx = 0, yy = 0;//定义坐标变化量
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		yy--;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		yy++;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		xx--;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		xx++;
	}
	if (xx != 0 || yy != 0) {//如果有移动输入
		if (UnitData[Player.Player].BoolDEL) {//如果玩家有单位 则使用单位的加速度移动
			UnitData[Player.Player].ToF(xx, yy);//玩家加速度
		}
		else {//如果玩家没有单位 则直接移动摄像机
			cameraPos.x += xx * 200 / Win.Size * Time.JGTime;//直接移动摄像机
			cameraPos.y += yy * 200 / Win.Size * Time.JGTime;//直接移动摄像机
		}
	}



	
	// ---------- 切换模式 ----------
	glfwSetKeyCallback(window, [](GLFWwindow* w, int k, int, int a, int) {//键盘事件
		if (k == GLFW_KEY_SPACE && a == GLFW_PRESS) {// 空格 切换显示模式
			BoolMod = !BoolMod;//切换状态
			if (BoolMod) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
			else  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//默认模式
		}
		});
		*/
}
// ---------- 鼠标移动事件 ------------
void mouse_callback_Game(GLFWwindow* window, double xpos, double ypos) {

	MouseX = xpos;//更新鼠标坐标
	MouseY = ypos;//更新鼠标坐标
	//static float lastX = 400, lastY = 300;
	//static bool firstMouse = true;
	//if (firstMouse) {
	//	lastX = xpos;
	//	lastY = ypos;
	//	firstMouse = false;
	//}
	//float xoffset = xpos - lastX;
	//float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	//lastX = xpos;
	//lastY = ypos;
	//float sensitivity = 0.1f; // change this value to your liking
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;
	//static float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
	//static float pitch = 0.0f;
	//yaw += xoffset;
	//pitch += yoffset;
	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
	//	pitch = -89.0f;
	//glm::vec3 front;
	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//cameraFront = glm::normalize(front);
}
// ---------- 鼠标滚轮事件 ------------
void scroll_callback_Game(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		Win.Size += (int)(Win.Size * 0.2);
	}
	else {
		Win.Size -= (int)(Win.Size * 0.2);
	}
	if (Win.Size > Win.WinY / 3) Win.Size = Win.WinY / 3;
	if (Win.Size < Win.WinY / 120) Win.Size = Win.WinY / 120;
	//camerafov -= (float)yoffset;
	//if (camerafov < 1.0f) camerafov = 1.0f;
	//if (camerafov > 90.0f) camerafov = 90.0f;
	//// 更新投影矩阵
	//Debug("滚轮事件: " + std::to_string(camerafov));//测试日志
	//projection = glm::perspective(glm::radians(camerafov), (float)Win.WinX / (float)Win.WinY, 0.1f, 100.0f);
}
// ---------- 鼠标点击事件 ------------
void mouse_button_callback_Game(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		uiManager.HandleClick(MouseX, MouseY);//处理点击事件
	}
}





void playerinput() {//<---------------------------------------------------玩家输入 

	processInput_Game(window, Time.JGTime);//处理键盘输入
	glfwSetCursorPosCallback(window, mouse_callback_Game);//设置鼠标位置回调函数
	glfwSetScrollCallback(window, scroll_callback_Game);//设置鼠标滚轮回调函数
	glfwSetMouseButtonCallback(window, mouse_button_callback_Game);//设置鼠标按钮回调函数
	uiManager.UpdateHover(MouseX, MouseY);//更新鼠标悬停状态


	

	//if (GetAsyncKeyState(PlayerInput.left) & 0x8000)xx--;//左
	//if (GetAsyncKeyState(PlayerInput.right) & 0x8000)xx++;//右
	//if (GetAsyncKeyState(PlayerInput.up) & 0x8000)yy--;//上
	//if (GetAsyncKeyState(PlayerInput.down) & 0x8000)yy++;//下
	//if (xx != 0 || yy != 0) {
	//	if (UnitData[Player.Player].BoolDEL) UnitData[Player.Player].ToF(xx, yy);//玩家加速度
	//	else {
	//		Player.CameraX += xx * 200 / Win.Size * Time.JGTime / 1000;
	//		Player.CameraY += yy * 200 / Win.Size * Time.JGTime / 1000;
	//	}
	//}

	//if (peekmessage(&Mouse, EX_MOUSE)) {//获取鼠标消息
	//	if (Mouse.message == WM_MOUSEWHEEL) {//鼠标滚轮
	//		if (Mouse.wheel > 0) {
	//			Win.Size += (int)(Win.Size * 0.2);
	//		}
	//		else {
	//			Win.Size -= (int)(Win.Size * 0.2);
	//		}
	//		if (Win.Size > Win.WinY / 3) Win.Size = Win.WinY / 3;
	//		if (Win.Size < Win.WinY / 120) Win.Size = Win.WinY / 120;
	//	}
	//}
}