#pragma once

#include "OpenGL.h"
#include "Debug.h"


// vertices(顶点数据 Size  三角形数据 Size 属性大小 Size)
class Vertex {
public:
	// ----- 构造函数 -----------
	
	Vertex(float verticesa[], int vSize, unsigned int indices[], int iSize, unsigned int prop[], int pSize) {

		Debug("顶点数据长度 " + std::to_string(vSize));

		//创建 VAO VBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		//绑定 VAO VBO
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vSize, verticesa, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
		
		//设置属性
		int S = 0;//计算总属性大小
		for (int i = 0; i < pSize; i++) S += prop[i];//计算总属性大小
		int k = 0;//计算属性偏移
		for (int i = 0; i < pSize; i++) {//循环设置属性
			SetProperty(i, prop[i], S, k);//设置属性
			k += prop[i];//更新属性偏移
		}
		//SetProperty(0, 3, 8, 0);//设置顶点属性
		//SetProperty(1, 3, 8, 3);//设置颜色属性
		//SetProperty(2, 2, 8, 6);//贴图属性
		
	}
	// ----- 设置属性 -----------
	void SetProperty(int ID, int PSize, int VSize, int Psizeof) {//ID 属性ID PSize 属性大小 VSize 类型大小 Psizeof 步长
		glVertexAttribPointer(ID, PSize, GL_FLOAT, GL_FALSE, VSize * sizeof(float), (void*)(Psizeof * sizeof(float)));//设置属性指针
		glEnableVertexAttribArray(ID);
	}
	// ----- 删除缓冲区 ---------
	void DeleteBuffer() {
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteVertexArrays(1, &VAO);
	}
	// ----- 获取VAO ------------
	GLuint GetVAO() const { return VAO; }
private:
	unsigned int VAO, VBO, EBO;
};