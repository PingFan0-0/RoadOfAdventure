#pragma once



#include"OpenGL.h"




extern glm::mat4 model;//模型矩阵
extern glm::mat4 view;//视图矩阵
extern glm::mat4 projection;//投影矩阵


extern glm::vec3 cameraPos;//摄像机位置
extern glm::vec3 cameraFront;//摄像机朝向
extern glm::vec3 cameraUp;//摄像机上方向
extern float camerafov;//摄像机视野