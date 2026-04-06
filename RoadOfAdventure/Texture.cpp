#include "Texture.h"


unsigned int Texture(const char* Way, int ID, bool FZ, unsigned char* data) {

	unsigned int texture;//纹理ID
	int width, height, nrChannels;//图像宽高和通道数
	stbi_set_flip_vertically_on_load(FZ);//翻转图像
	glGenTextures(1, &texture);//生成纹理
	glActiveTexture(GL_TEXTURE0 + ID);//激活纹理单元
	glBindTexture(GL_TEXTURE_2D, texture);//绑定纹理
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//过滤方式
	// 加载并生成纹理

	data = stbi_load(Way, &width, &height, &nrChannels, 0);//加载图像
	if (data)//如果加载成功
	{
		Debug("贴图加载成功" + std::string(Way));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//生成纹理
		glGenerateMipmap(GL_TEXTURE_2D);//生成多级纹理
	}
	else//如果加载失败
	{
		Error("贴图加载失败" + std::string(Way), "R");
	}
	stbi_image_free(data);//释放图像内存
	return texture;
}
