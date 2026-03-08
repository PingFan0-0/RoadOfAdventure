#include "PageDisplay.h"

#include"GameData.h"
#include"PlayerInput.h"
#include<graphics.h>
#include"GameRun.h"



void NewWindows(int X, int Y) {//<---------------------------------------创建窗口
	initgraph(X, Y);//初始化图形界面
}


void ymxs() {//<---------------------------------------------------------页面选择
	if (YM == "begin")  YMBegin();//初始界面
	if (YM == "gameon") YMGameRun();//游戏页面
	if (YM == "set")    YMSet();//设置页面
	if (YM == "about")  YMAbout();//关于页面.


	//if(cz == 0){//起始页面====================(0) 
 //   	while(1) {//input
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
}

void YMBegin() {//<--------------------------------------------------------------------初始界面
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
		outtextxy(TitleX, TitleY, _T("冒险之路"));//输出文字
		// ===== Button
		//设置字体 =-= (开始游戏)
		BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 0, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 0, _T("开始游戏"));
		//设置字体 =-= (设置)
		BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 1, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 1, _T("设置"));
		//设置字体 =-= (关于)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 2, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 2, _T("关于"));
		//设置字体 =-= (退出)
		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("退出"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButton1) {//按钮1 开始游戏
				if (gamestart()) {
					YM = "gameon";//切换到游戏界面
					return;
				}
				else {
					MessageBox(GetHWnd(), _T("地图文件打开失败！"), _T("错误"), MB_OK | MB_ICONERROR);
					Mouse.x = -1;
					Mouse.y = -1;
					//YM = "begin";//保持在初始界面
					return;
				}
			}
			if (BoolButton2) {//按钮2 设置
				YM = "set";//切换到设置界面
				return;
			}
			if (BoolButton3) {//按钮3 关于
				YM = "about";//切换到关于界面
				return;
			}
			if (BoolButtonEnd) {//按钮4 退出
				BoolTheGame = false;//结束游戏主循环
				return;
			}
		}

		Sleep(1); //延时
		cleardevice();// 清屏
	}
}

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
		//设置字体 =-= (开始游戏)
		BoolButton1 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 0, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 0 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 0, _T("设置"));
		//设置字体 =-= (设置)
		BoolButton2 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 1, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 1 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 1, _T("设置"));
		//设置字体 =-= (关于)
		BoolButton3 = Object.ButtonStyle1(ButtonT, ButtonHighT, ButtonF, ButtonHighF, MousePlace(MouseX, MouseY, ButtonX, ButtonY + ButtonSpaceY * 2, ButtonX + ButtonWidth, ButtonY + ButtonSpaceY * 2 + ButtonHighF));
		outtextxy(ButtonX, ButtonY + ButtonSpaceY * 2, _T("设置"));
		//设置字体 =-= (返回)
		BoolButtonEnd = Object.ButtonStyle1(ButtonT, EndHighT, ButtonF, EndHighF, MousePlace(MouseX, MouseY, EndX, EndY, EndX + EndWidth, EndY + EndHighF));
		outtextxy(EndX, EndY, _T("返回"));//输出文字

		settextstyle(15, 0, _T("宋体"));//打印鼠标坐标
		outtextxy(0, 0, (std::to_wstring(MouseX) + L" " + std::to_wstring(MouseY)).c_str());
		FlushBatchDraw();//将绘图从缓存中绘制到屏幕====================


		if (Mouse.message == WM_LBUTTONDOWN && !BoolMouseLife) {//点击鼠标左键
			if (BoolButton1) {//按钮1 

			}
			if (BoolButton2) {//按钮2 
				YM = "set";//切换到设置界面
				return;
			}
			if (BoolButton3) {//按钮3
				YM = "about";//切换到关于界面
				return;
			}

			if (BoolButtonEnd) {//按钮 返回
				YM = "begin";//切换到初始界面
				return;
			}
		}
		Sleep(1); //延时
		cleardevice();// 清屏
	}
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

void YMGameRun() {//<------------------------------------------------------------------游戏界面
	GameOnRun();
}