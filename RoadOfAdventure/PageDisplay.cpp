#include "PageDisplay.h"

#include"GameData.h"
#include"PlayerInput.h"
//#include<graphics.h>
#include"GameRun.h"

#include"Shader.h"
#include"Image.h"
#include"Model.h"
#include"OpenGL.h"
#include"SpriteRenderer.h"
#include"TextRenderer.h"
#include"Texture.h"
#include"UIClickableText.h"
#include"Vertex.h"
#include"DrawShape.h"


#include"Debug.h"

#include"Camera.h"

auto& uiManager = UIClickableTextManager::GetInstance();




//void NewWindows(int X, int Y) {//<---------------------------------------创建窗口
//	initgraph(X, Y);//初始化图形界面
//}


void ymxs() {//<---------------------------------------------------------页面选择
	if (YM == "begin")  YMBegin();//初始界面
	else if (YM == "gameon") YMGameRun();//游戏页面
	/*
	else if (YM == "set")    YMSet();//设置页面
	else if (YM == "setgame")YMSetGame();//设置 - game界面
	else if (YM == "about")  YMAbout();//关于页面.
	*/
	uiManager.Clear();//每次页面切换都清除可点击文本

	//非常旧的========
#pragma region
	//if(cz == 0){//起始页面====================(0) 
	//  	while(1) {//input
	//		std::string str;
	//		str+= "冒险之路\n1.继续游戏\n2.设置\n3.关于\n\nEsc.退出游戏";
	//		cls();//清屏
	//		ct(0,0,str);//打印文本
	//        char input=_getch();//获取玩家输入
	//		if(input == '1') {//选项 继续游戏-----(1)
	//			if (gamestart())cz = 1;//如果初始化成功 切换到游戏界面
	//            break;
	//        }else if(input == '2') {//选项 设置-----(2)
	//			cz=2;//切换到设置界面
	//			GameData("SDR",-1);//读取设置数据
	//            break;
	//        }else if(input == '3') {//选项 关于-----(3)
	//			cz=3;//切换到关于界面
	//            break;
	//        }else if(input == PlayerInput.returnn) {//选项 退出游戏-----(r)
	//        	BoolTheGame = false;//结束游戏主循环
	//            return;
	//        }
	//	}
	//}else if(cz == 2){//设置页面====================(2) 
	//	while(1){
	//		std::string str;
	//		str += "设置\n";
	//		str += "1.日志记录          当前:";if(BoolDebug)       str+="开\n";else str+="关\n";
	//		str += "2.FPS显示           当前:";if(BoolFPS)         str+="开\n";else str+="关\n";
	//		str += "3.FPS刷新间隔       当前:";                    str+="每"+std::to_string(FPSWeek)+"游戏时刻更新\n";
	//		str += "4.游戏时刻显示      当前:";if(BoolGameRunTime) str+="开\n";else str+="关\n";
	//		str += "5.坐标显示          当前:";if(BoolZbxs)        str+="开\n";else str+="关\n";
	//		str += "6.地图信息显示      当前:";if(BoolMapMessage)  str+="开\n";else str+="关\n";
	//		str += "7.警告显示          当前:";if(BoolWarn)        str+="开\n";else str+="关\n";
	//		str += "8.重置游戏数据      当前:无\n";
	//		str+="\n\n\n";
	//		str+="Esc.返回";
	//		cls();//清屏
	//		ct(0,0,str);//打印文本
	//        char input=_getch();//获取玩家输入
	//        if(input == '1'){//选项 日志记录-----(1) 
	//			BoolDebug = BoolDebug ? false : true;//切换状态
	//		}else if(input == '2'){//选项 FPS显示-----(2)
	//        	BoolFPS = BoolFPS ? false : true;//切换状态
	//		}else if(input == '3'){//选项 FPS显示-----(3)
	//        	while(1){
	//        		str+="FPS刷新间隔\n"; 
	//        		str+="1.每1时刻更新\n";
	//        		str+="2.每2时刻更新\n";
	//        		str+="3.每5时刻更新\n";
	//        		str+="4.每10时刻更新\n";
	//        		str+="5.每20时刻更新\n";
	//        		str+="6.自定义\n";
	//        		str+="\n";
	//        		str+="当前:每"+std::to_string(FPSWeek)+"游戏时刻更新";
	//        		str+="\n\n\n";
	//				str+="Esc.返回";
	//				cls();
	//        		ct(0,0,str);
	//        		char input1=_getch();
	//        		if(input1 == '1'){//选项  每1-(1) 
	//        			FPSWeek = 1;
	//				}else if(input1 == '2'){//选项  每2-(2) 
	//        			FPSWeek = 2;
	//				}else if(input1 == '3'){//选项  每5-(3) 
	//        			FPSWeek = 5;
	//				}else if(input1 == '4'){//选项  每10-(4) 
	//        			FPSWeek = 10;
	//				}else if(input1 == '5'){//选项  每20-(5) 
	//        			FPSWeek = 20;
	//				}else if(input1 == '6'){//选项  自定义-(6) 
	//        			while(1){
	//        				str+="FPS刷新间隔 自定义\n";
	//        				str+="请输入 数值\n";
	//        				str+="范围 数值 >= 1\n";
	//        				cls();
	//        				ct(0,0,str);
	//        				int num;
	//        				std::cin>>num;
	//        				if(num>=1){
	//        					FPSWeek = num;
	//							break; 
	//						}else{
	//							str+="\n范围错误 按 Esc 键以重新输入\n";
	//							ct(0,0,str);
	//							while(1){
	//								char input2 = _getch();
	//								if(input2 == PlayerInput.returnn){
	//									break;
	//								}
	//							}
	//						}
	//					}
	//				}else if(input1 == PlayerInput.returnn){
	//					GameData("SDW",-1);
	//		            break;
	//				}
	//			} 
	//		}
	//		else if(input == '4'){//选项 游戏时刻显示-----(4)
	//        	BoolGameRunTime = BoolGameRunTime ? false : true;//切换状态
	//		}
	//		else if(input == '5'){//选项 坐标显示-----(5)
	//        	BoolZbxs = BoolZbxs ? false : true;//切换状态
	//		}
	//		else if(input == '6'){//选项 地图信息显示-----(6)
	//        	BoolMapMessage = BoolMapMessage ? false : true;//切换状态
	//		}
	//		else if (input == '7') {//选项 警告显示-----(7)
	//			BoolWarn = BoolWarn ? false : true;//切换状态
	//		}
	//		else if (input == PlayerInput.returnn) {//选项 返回-----(r) 
	//			GameData("SDW",-1);//保存设置数据
	//        	cz=0;//切换到初始界面
	//            break;//退出循环
	//        }
	//	}
	//}else if(cz == 3){//关于====================(3) 
	//	while(1){
	//		std::string str;
	//		str+="关于\n";
	//		str+="\n";
	//		str+="Null\n";
	//        str+="\n\n\n";
	//		str+="Esc.返回";
	//		cls();//清屏
	//		ct(0,0,str);//打印文本
	//		char input=_getch();//获取玩家输入
	//        if(input == PlayerInput.returnn) {//选项 返回-----(r)
	//        	cz=0;//切换到初始界面
	//            break;//退出循环
	//        }
	//	}	
	//}else if(cz == -1){//死亡页面====================(-1) 
	//	pmsx();//屏幕刷新
	//	GameData("GDW",0);//保存游戏数据
	//	int X=5,Y=6;//定义文本显示位置
	//	std::string str;
	//	str=" ________________________ ";
	//	ct(X,Y+0,str);
	//	str="|                        |";
	//	ct(X,Y+1,str);
	//	str="|        GAME_OVER       |";
	//	ct(X,Y+2,str);
	//	str="|                        |";
	//	ct(X,Y+3,str);
	//	str="|                        |";
	//	ct(X,Y+4,str);
	//	str="|                        |";
	//	ct(X,Y+5,str);
	//	str="|                        |";
	//	ct(X,Y+6,str);
	//	str="|    ---按Esc键返回---   |";
	//	ct(X,Y+7,str);
	//	str="|________________________|";
	//	ct(X,Y+8,str);
	//	while(1){
	//		char input=_getch();//获取玩家输入
	//		if(input == PlayerInput.returnn){
	//			cz=0;//切换到起始界面
	//			break;//退出循环
	//		}
	//	}
	//}
#pragma endregion
}

//加载着色器
#pragma region




#pragma endregion



bool BoolKeyEsc = false;
// ---------- 键盘输入 ----------------
void processInput(GLFWwindow* window,float JGTime){
	// ---------- 移动 ----------
	float cameraSpeed = 3.0f;
	float S = JGTime * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraUp * S;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraUp * S;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * S;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * S;
	}
	// ---------- 退出 ----------
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {// ESC按下  关闭
		if(BoolKeyEsc) BoolTheGame = false;//结束游戏主循环
	}
	else if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {// ESC松开
		BoolKeyEsc = true;
	}
	glfwSetKeyCallback(window, [](GLFWwindow* w, int k, int, int a, int) {//键盘事件
		if (a == GLFW_PRESS) {
			// ---------- 切换模式 ----------
			if (k == GLFW_KEY_SPACE) {// 空格 切换显示模式
				BoolMod = !BoolMod;//切换状态
				if (BoolMod) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
				else  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//默认模式
			}
		}
	});
}
// ---------- 鼠标移动事件 ------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camerafov -= (float)yoffset;
	if (camerafov < 1.0f) camerafov = 1.0f;
	if (camerafov > 90.0f) camerafov = 90.0f;
	// 更新投影矩阵
	//Debug("滚轮事件: " + std::to_string(camerafov));//测试日志
	projection = glm::perspective(glm::radians(camerafov), (float)Win.WinX / (float)Win.WinY, 0.1f, 100.0f);
}
// ---------- 鼠标点击事件 ------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		uiManager.HandleClick((float)MouseX, (float)MouseY);//处理点击事件
	}
}
// ---------- 窗口回调 ----------
bool BoolW = false;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// 1. 更新视口
	glViewport(0, 0, width, height);

	DrawRectShape::GetInstance().UpdateProjection(width, height);//图形渲染器
	//spriteRenderer.UpdateProjection(width, height);// 精灵渲染器
	//textRenderer.UpdateProjection(width, height);  // 文本渲染器

	projection = glm::perspective(glm::radians(camerafov), (float)Win.WinX / (float)Win.WinY, 0.1f, 100.0f);
	Win.WinX = width; Win.WinY = height;
	BoolW = true;
}




void YMBegin() {//<--------------------------------------------------------------------初始界面
#pragma region 旧的
	//int TitleHigh = Win.WinY / 15;// ===== Title
	//int TitleX = Win.WinX / 2 - Win.WinX / 4;
	//int TitleY = Win.WinY / 20;
	//int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	//int ButtonHighF = Win.WinY / 20;
	//int ButtonWidth = 120;
	//int ButtonX = Win.WinX / 8;
	//int ButtonY = Win.WinY / 5;
	//int ButtonSpaceY = Win.WinY / 10;
	//int EndHighT = ButtonHighT;// ===== End
	//int EndHighF = ButtonHighF;
	//int EndWidth = ButtonWidth;
	//int EndX = ButtonX;
	//int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	//COLORREF ButtonT = RGB(255, 55, 55);
	//COLORREF ButtonF = RGB(255, 255, 255);

	//bool BoolButtonEnd = false;
	//bool BoolButton1 = false;
	//bool BoolButton2 = false;
	//bool BoolButton3 = false;

	//bool BoolMouseLife = false;

	//class C {
	//public:
	//	void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
	//		settextstyle(ButtonSize, 0, _T("宋体"));
	//		settextcolor(ButtonColor);//设置文字颜色
	//	}
	//	bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
	//		if (BoolClick) TextStyle1(ColorT, SizeT);
	//		else TextStyle1(ColorF, SizeF);
	//		return BoolClick;
	//	}
	//}Object;

	//while (1) {
	//	BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);

	//	peekmessage(&Mouse, EX_MOUSE);

	//	//获取鼠标数据
	//	MouseX = Mouse.x;//更新鼠标坐标
	//	MouseY = Mouse.y;

	//	BeginBatchDraw();//将绘图保存在缓存中====================
	//	std::wstring text;
	//	settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
	//	settextcolor(RGB(255, 255, 0));//设置文字颜色
	//	outtextxy(TitleX, TitleY, _T("冒险之路"));//输出文字
	//	// ===== Button
	//	//设置字体 =-= (开始游戏)
	//	BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 0, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
	//	outtextxy(ButtonX, ButtonY + ButtonSpaceY * 0, _T("开始游戏"));
	//	//设置字体 =-= (设置)
	//	BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 1, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
	//	outtextxy(ButtonX, ButtonY + ButtonSpaceY * 1, _T("设置"));
	//	//设置字体 =-= (关于)
	//	BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 2, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
	//	outtextxy(ButtonX, ButtonY + ButtonSpaceY * 2, _T("关于"));
	//	//设置字体 =-= (退出)
	//	BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
	//	outtextxy(EndX, EndY, _T("退出"));//输出文字

	//	settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
	//	outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
	//	FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


	//	if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
	//		if (BoolButton1) {//按钮1 开始游戏
	//			if (gamestart()) {
	//				YM = "gameon";//切换到游戏界面
	//				return;
	//			}
	//			else {
	//				MessageBox(GetHWnd(), _T("地图文件打开失败！"), _T("错误"), MB_OK | MB_ICONERROR);
	//				Mouse.x = -1;
	//				Mouse.y = -1;
	//				//YM = "begin";//保持在初始界面
	//				return;
	//			}
	//		}
	//		if (BoolButton2) {//按钮2 设置
	//			YM = "set";//切换到设置界面
	//			return;
	//		}
	//		if (BoolButton3) {//按钮3 关于
	//			YM = "about";//切换到关于界面
	//			return;
	//		}
	//		if (BoolButtonEnd) {//按钮4 退出
	//			BoolTheGame = false;//结束游戏主循环
	//			return;
	//		}
	//	}

	//	Sleep(1); //延时
	//	cleardevice();// 清屏
	//}
#pragma endregion
#pragma region 文本 ===================
	TextRenderer textRenderer;
	std::string FontsName = wstring_string(DataParent + L"/" + FontsParent) + "/" + FontMain;
	if (!textRenderer.Load(FontsName, textRenderer.FontSize)) {

	}
	// 添加可点击按钮
	uiManager.Init(&textRenderer, Win.WinX, Win.WinY);

	// 注册可点击文本
	uiManager.AddText("开始游戏", 100, 200, 1.0f, []() {
		Debug("开始游戏");
		YM = "gameon";
		}, glm::vec3(1.0f), glm::vec3(1.0f, 8.0f, 0.0f));
	uiManager.AddText("设置", 100, 260, 1.0f, []() {
		Debug("打开设置 不不不设置还没好");
		//YM = "set";//切换到设置界面
		}, glm::vec3(1.0f), glm::vec3(1.0f, 8.0f, 0.0f));
	uiManager.AddText("退出游戏", 100, 320, 1.0f, []() {
		Debug("退出游戏");
		BoolTheGame = false;
		}, glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

#pragma endregion
#pragma region 顶点数据 ===============
	Vertex vertex(verticesZ, verticesZ_size, indicesZ, verticesZ_size, propZ, 3);//创建顶点对象
#pragma endregion
#pragma region 加载贴图 ===============
	//static unsigned char* data;//图像数据
	//unsigned int Null = Texture("Data/Image/null.jpg", 0, true, data);//创建纹理对象
#pragma endregion
#pragma region 矩阵 ===================

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);//定义一个4D向量
	glm::mat4 trans = glm::mat4(1.0f);//初始化为单位矩阵
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));//缩放
	vec = trans * vec;//对向量进行变换
	

	//相机========
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));//模型矩阵
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));//视图矩阵
	projection = glm::perspective(glm::radians(45.0f), (float)Win.WinX / (float)Win.WinY, 0.1f, 100.0f);//投影矩阵

	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//摄像机位置
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//摄像机朝向
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//摄像机上方向

#pragma endregion
#pragma region 着色器 =================

	Shader SH("vertexShader.glsl", "fragmentShader0.glsl");//创建着色器
	SpriteRenderer spriteRenderer;
#pragma endregion
#pragma region OpenGL设置 =============

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标
	glEnable(GL_DEPTH_TEST);//开启深度测试
	glDisable(GL_CULL_FACE);// 关闭面剔除
	glEnable(GL_BLEND);// 启用混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 设置混合函数
	if(BoolMod) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式
	else  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//默认模式

#pragma endregion
#pragma region 窗口 ===================
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
#pragma endregion
	BoolKeyEsc = !(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
#pragma region 绘制循环 =============== 
	Debug("进入绘制---> 初始页面");//测试日志
	while (BoolTheGame && YM == "begin") {//绘制循环--------------------------------
		TimeMath();//计算时间

		processInput(window, Time.JGTime);//处理键盘输入
		glfwSetCursorPosCallback(window, mouse_callback);//设置鼠标位置回调函数
		glfwSetScrollCallback(window, scroll_callback);//设置鼠标滚轮回调函数
		glfwSetMouseButtonCallback(window, mouse_button_callback);//设置鼠标按钮回调函数
		uiManager.UpdateHover((float)MouseX, (float)MouseY);//更新鼠标悬停状态

		glClearColor(0.15f, 0.2f, 0.2f, 0);//设置背景颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓冲区
		glActiveTexture(GL_TEXTURE0);//激活纹理单元0
		glBindTexture(GL_TEXTURE_2D, atlas);//绑定纹理
		
		glBindVertexArray(vertex.GetVAO());//绑定顶点数组对象
		SH.setInt("ourTexture", 0);   // 确保 uniform 指向纹理单元0
		SH.Draw();//使用着色器



		trans = glm::rotate(trans, glm::radians(-40.0f * Time.JGTime), glm::vec3(0.0f, 0.0f, 1.0f));//旋转
		SH.setMat4("transform", trans);
		float r1 = (sin(Time.NowTime) + 1) / 2;//计算颜色
		float g1 = (sin(Time.NowTime + 2.0f) + 1) / 2;//计算颜色
		float b1 = (sin(Time.NowTime + 4.0f) + 1) / 2;//计算颜色
		SH.SetColor(r1, g1, b1);//设置颜色
		SH.setMat4("model", model);

		//透视
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//视图矩阵
		SH.setMat4("view", view);
		SH.setMat4("projection", projection);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			SH.setMat4("model", model);//设置模型矩阵

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		spriteRenderer.DrawSprite(atlas, regions[0], glm::vec2(100, 400), glm::vec2(100), sin(Time.NowTime));

		textRenderer.RenderText("FPS: " + std::to_string(FPS), 10.0f, 10.0f, 0.3f, glm::vec3(1.0f));
		textRenderer.RenderText(std::to_string((int)MouseX) + "," + std::to_string((int)MouseY), 10.0f, 25.0f, 0.3f, glm::vec3(1.0f));
		textRenderer.RenderText("冒险之路", 100.0f, 50.0f, 1.0f, glm::vec3(1.0f, 0.5f, 0.2f));
		textRenderer.RenderText("PF0_0", 600.0f, 500.0f, sin(Time.NowTime * 2), glm::vec3(0.3f, 0.7f, 0.9f));
		uiManager.Render();//渲染按钮
		//数据处理
		glfwSwapBuffers(window);//交换缓冲区
		glfwPollEvents();//获取事件
		if (BoolW) {
			spriteRenderer.UpdateProjection(Win.WinX, Win.WinY);// 精灵渲染器
			textRenderer.UpdateProjection(Win.WinX, Win.WinY);  // 文本渲染器
			glEnable(GL_DEPTH_TEST);//开启深度测试
			BoolW = !BoolW;
		}
	}
	vertex.DeleteBuffer();//删除缓冲区
	//glfwCreateCursor(nullptr, 0, 0);//恢复默认鼠标
	Debug("离开页面<--- 初始页面");//测试日志
#pragma endregion
}
/*
void YMSet() {//<--------------------------------------------------------------------设置界面
	int TitleHigh = Win.WinY / 15;// ===== Title
	int TitleX = Win.WinX / 2 - Win.WinX / 4;
	int TitleY = Win.WinY / 20;
	int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	int ButtonHighF = Win.WinY / 20;
	int ButtonWidth = 120;
	int ButtonX = Win.WinX / 8;
	int ButtonY = Win.WinY / 5;
	int ButtonSpaceY = Win.WinY / 10;
	int EndHighT = ButtonHighT;// ===== End
	int EndHighF = ButtonHighF;
	int EndWidth = ButtonWidth;
	int EndX = ButtonX;
	int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	COLORREF ButtonT = RGB(255, 55, 55);
	COLORREF ButtonF = RGB(255, 255, 255);

	bool BoolButtonEnd = false;
	bool BoolButton1 = false;
	bool BoolButton2 = false;
	bool BoolButton3 = false;

	bool BoolMouseLife = false;

	class C {
	public:
		void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
			settextstyle(ButtonSize, 0, _T("宋体"));
			settextcolor(ButtonColor);//设置文字颜色
		}
		bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
			if (BoolClick) TextStyle1(ColorT, SizeT);
			else TextStyle1(ColorF, SizeF);
			return BoolClick;
		}
	}Object;

	while (1) {
		BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);

		peekmessage(&Mouse, EX_MOUSE);

		//获取鼠标数据
		MouseX = Mouse.x;//更新鼠标坐标
		MouseY = Mouse.y;

		BeginBatchDraw();//将绘图保存在缓存中====================
		std::wstring text;
		settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
		settextcolor(RGB(255, 255, 0));//设置文字颜色
		outtextxy(TitleX, TitleY, _T("设置"));//输出文字
		// ===== Button
		//设置字体 =-= (游戏)
		BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 0, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 0, _T("游戏设置"));
		//设置字体 =-= (设置)
		BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 1, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 1, _T("null"));
		//设置字体 =-= (关于)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 2, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 2, _T("null"));
		//设置字体 =-= (返回)
		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("返回"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButton1) {//按钮1 
				YM = "setgame";//切换到设置 - game
				return;
			}
			if (BoolButton2) {//按钮2 
				//YM = "set";//切换到设置界面
				//return;
			}
			if (BoolButton3) {//按钮3
				//YM = "about";//切换到关于界面
				//return;
			}

			if (BoolButtonEnd) {//按钮 返回
				YM = "begin";//切换到初始界面
				GameData("SDW", 1);
				return;
			}
		}
		Sleep(1); //延时
		cleardevice();// 清屏
	}
}

void YMSetGame() {//<--------------------------------------------------------------------设置 - game界面
	int TitleHigh = Win.WinY / 15;// ===== Title
	int TitleX = Win.WinX / 2 - Win.WinX / 4;
	int TitleY = Win.WinY / 20;
	int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	int ButtonHighF = Win.WinY / 20;
	int ButtonWidth = 120;
	int ButtonXL = Win.WinX / 8;
	int ButtonXR = Win.WinX - Win.WinX / 5;
	int ButtonY = Win.WinY / 5;
	int ButtonSpaceY = Win.WinY / 10;
	int EndHighT = ButtonHighT;// ===== End
	int EndHighF = ButtonHighF;
	int EndWidth = ButtonWidth;
	int EndX = ButtonXL;
	int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	COLORREF ButtonT = RGB(255, 55, 55);
	COLORREF ButtonF = RGB(255, 255, 255);

	bool BoolButtonEnd = false;
	bool BoolButton1 = false;
	bool BoolButton2 = false;
	bool BoolButton3 = false;

	bool BoolMouseLife = false;

	class C {
	public:
		void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
			settextstyle(ButtonSize, 0, _T("宋体"));
			settextcolor(ButtonColor);//设置文字颜色
		}
		bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
			if (BoolClick) TextStyle1(ColorT, SizeT);
			else TextStyle1(ColorF, SizeF);
			return BoolClick;
		}
	}Object;

	while (1) {
		BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);

		peekmessage(&Mouse, EX_MOUSE);

		//获取鼠标数据
		MouseX = Mouse.x;//更新鼠标坐标
		MouseY = Mouse.y;

		BeginBatchDraw();//将绘图保存在缓存中====================
		std::wstring text;
		settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
		settextcolor(RGB(255, 255, 0));//设置文字颜色
		outtextxy(TitleX, TitleY, _T("设置 - 游戏"));//输出文字
		// ===== Button
		int i = 0;
		std::wstring wstr;
		//设置字体 =-= (FPS显示)
		BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 0, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("FPS显示"));//0
		wstr = BoolFPS ? L"当前:开" : L"当前:关";
		outtextxy(ButtonXR, ButtonY + ButtonSpaceY * i, wstr.c_str());
		i++;
		//设置字体 =-= (坐标显示)
		BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 1, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("坐标显示"));//1
		wstr = BoolZbxs ? L"当前:开" : L"当前:关";
		outtextxy(ButtonXR, ButtonY + ButtonSpaceY * i, wstr.c_str());
		i++;
		//设置字体 =-= (速度显示)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 2, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("速度显示"));//2
		wstr = BoolVxs ? L"当前:开" : L"当前:关";
		outtextxy(ButtonXR, ButtonY + ButtonSpaceY * i, wstr.c_str());
		i++;
		//设置字体 =-= (null)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 2, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("null"));//3
		i++;
		//设置字体 =-= (null)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 2, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("null"));//4
		i++;
		//设置字体 =-= (null)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 2, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("null"));//5
		i++;
		//设置字体 =-= (null)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonXL, ButtonY + ButtonSpaceY * 2, ButtonXL + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonXL, ButtonY + ButtonSpaceY * i, _T("null"));//6
		i++;
		//设置字体 =-= (返回)
		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("返回"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButton1) {//按钮1 
				BoolFPS = !BoolFPS;//FPS显示
			}
			if (BoolButton2) {//按钮2 
				BoolZbxs = !BoolZbxs;//坐标显示
			}
			if (BoolButton3) {//按钮3
				BoolVxs = !BoolVxs;
			}
			//
			if (BoolButtonEnd) {//按钮 返回
				YM = "set";//切换到初始界面
				GameData("SDW", 1);
				return;
			}
		}else if (Mouse.message == WM_MOUSEWHEEL) {//鼠标滚轮
			if (Mouse.wheel > 0) {
				ButtonY += (int)(Win.WinY * 0.05);
			}
			else {
				ButtonY -= (int)(Win.WinY * 0.05);
			}
			if (ButtonY > Win.WinY / 5) ButtonY = Win.WinY / 5;
			if (ButtonY < Win.WinY / 5 - ButtonSpaceY * 2) ButtonY = Win.WinY / 5 - ButtonSpaceY * 2;
		}
		Sleep(1); //延时
		cleardevice();// 清屏
	}
	GameData("SDW", 1);
}

void YMAbout() {//<------------------------------------------------------------------关于界面
	int TitleHigh = Win.WinY / 15;// ===== Title
	int TitleX = Win.WinX / 2 - Win.WinX / 4;
	int TitleY = Win.WinY / 20;
	int TextHigh = Win.WinY / 30 + 5;// ===== Text
	int TextX = Win.WinX / 8;
	int TextY = Win.WinY / 5;
	int TextSpaceY = TextHigh + Win.WinY / 50;
	int ButtonHighT = Win.WinY / 20 + 5;// ===== Button
	int ButtonHighF = Win.WinY / 20;
	int ButtonWidth = 120;
	int EndHighT = ButtonHighT;// ===== End
	int EndHighF = ButtonHighF;
	int EndWidth = ButtonWidth;
	int EndX = TextX;
	int EndY = Win.WinY - Win.WinY / 10 - EndHighF;

	COLORREF ButtonT = RGB(255, 55, 55);
	COLORREF ButtonF = RGB(255, 255, 255);
	COLORREF TextColor1 = RGB(255, 255, 255);

	bool BoolButtonEnd = false;

	bool BoolMouseLife = false;

	class C {
	public:
		void TextStyle1(COLORREF ButtonColor, int ButtonSize) {
			settextstyle(ButtonSize, 0, _T("宋体"));
			settextcolor(ButtonColor);//设置文字颜色
		}
		bool ButtonStyle1(COLORREF ColorT, int SizeT, COLORREF ColorF, int SizeF, bool BoolClick) {
			if (BoolClick) TextStyle1(ColorT, SizeT);
			else TextStyle1(ColorF, SizeF);
			return BoolClick;
		}
	}Object;

	while (1) {
		BoolMouseLife = (Mouse.message == WM_LBUTTONDOWN);
		peekmessage(&Mouse, EX_MOUSE);//获取鼠标数据
		MouseX = Mouse.x;//更新鼠标坐标
		MouseY = Mouse.y;

		BeginBatchDraw();//将绘图保存在缓存中====================
		std::wstring text;
		settextstyle(TitleHigh, 0, _T("宋体"));//设置字体 ===== Title
		settextcolor(RGB(255, 255, 0));//设置文字颜色
		outtextxy(TitleX, TitleY, _T("关于"));//输出文字
		// ===== Button
		//文本 1
		Object.TextStyle1(TextColor1, TextHigh);
		outtextxy(TextX, TextY + TextSpaceY * 0, _T("本项目由 \"平凡0_0\" 创作"));
		//文本 2
		Object.TextStyle1(TextColor1, TextHigh);
		outtextxy(TextX, TextY + TextSpaceY * 1, _T("想不到还有什么可以写了"));

		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("返回"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButtonEnd) {//按钮1 返回
				YM = "begin";//切换到初始界面
				return;
			}
		}

		Sleep(1); //延时
		cleardevice();// 清屏
	}
}
*/
void YMGameRun() {//<------------------------------------------------------------------游戏界面
	GameOnRun();
}