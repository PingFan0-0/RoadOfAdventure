#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "stb_image_resize.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H 


//#include "Image.h"
//#include "Model.h"
//#include "Shader.h"
//#include "TextRenderer.h"
//#include "Texture.h"
//#include "Vertex.h"


#include <string>
#include <map>


extern bool BoolMod;
extern GLFWwindow* window;

struct XYInt {
	int X, Y;
};

//窗口

bool GLBegin();//窗口初始化
void GLEnd();//关闭窗口


void OpenGLMousePos();//更新鼠标位置

std::string LoadShader(std::string name);//打开着色器文件