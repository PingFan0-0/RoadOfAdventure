#pragma once

#include"OpenGL.h"
#include"Debug.h"


class Shader {


public:
	// ----- 构造函数 ----------- 
	Shader(const char* vs, const char* fs) {//初始化
		Debug("创建 Shader  VS(" + std::string(vs) + ") FS(" + fs + ")");
		this->VS = vs;
		this->FS = fs;

		int Bool;

		// --- VS
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建
		std::string str1 = LoadShader(vs);//获取着色器
		const GLchar* vertexShaderSource = str1.c_str();//写入着色器
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//设置代码
		glCompileShader(vertexShader);// 编译 着色器
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Bool);//获取编译状态
		if (!Bool) {
			char Text[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, Text);
			Error("编译(" + std::string(vs) + ")时发生错误 " + Text, "W");
		}
		
		// --- FS
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);//创建
		std::string str2 = LoadShader(fs);//获取着色器
		const GLchar* fragmentShaderSource0 = str2.c_str();//写入着色器
		glShaderSource(fragmentShader, 1, &fragmentShaderSource0, NULL);
		glCompileShader(fragmentShader);// 编译 着色器
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Bool);//获取编译状态
		if (!Bool) {
			char Text[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, Text);
			Error("编译(" + std::string(vs) + ")时发生错误 " + Text, "W");
		}

		// --- 链接
		Program = glCreateProgram();//创建Program
		glAttachShader(Program, vertexShader);//链接VS
		glAttachShader(Program, fragmentShader);//链接FS
		glLinkProgram(Program);//链接
		glGetProgramiv(Program, GL_LINK_STATUS, &Bool);
		if (!Bool) {
			char Text[512];
			glGetProgramInfoLog(Program, 512, NULL, Text);
			Error("链接着色器程序时发生错误 " + std::string(Text), "W");
		}

		glDeleteShader(vertexShader);//删除
		glDeleteShader(fragmentShader);//删除
	}

	// ----- 使用着色器 ---------
	void Draw() {
		glUseProgram(Program);
	}

	// ----- 设置RGB颜色 --------
	void SetColor(float R, float G, float B) {
		setFloat("R", R);
		setFloat("G", G);
		setFloat("B", B);
	}

	// ----- 设置坐标 -----------
	void SetPos(float X, float Y, float Z) {
		setFloat("X", X);
		setFloat("Y", Y);
		setFloat("Z", Z);
	}

	// ----- 以名字设置 Bool ----
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(Program, name.c_str()), (int)value);
	}

	// ----- 以名字设置 Int -----
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(Program, name.c_str()), value);
	}

	// ----- 以名字设置 Float ---
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(Program, name.c_str()), value);
	}

	// ----- 以名字设置 Mat4 ----
	void setMat4(const std::string& name, glm::mat4 trans) const
	{
		glUniformMatrix4fv(glGetUniformLocation(Program, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
	}

	// ----- 以名字设置 Vec3f ---
	void setVector3f(const std::string& name, glm::vec3 trans) const
	{
		glUniform3f(glGetUniformLocation(Program, name.c_str()), trans.r, trans.g, trans.b);
	}

	unsigned int Program;
	const char* VS;
	const char* FS;

};
