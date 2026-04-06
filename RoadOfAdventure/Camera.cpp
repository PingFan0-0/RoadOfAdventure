#include "Camera.h"


glm::mat4 model = glm::mat4(1.0f);//模型矩阵
glm::mat4 view = glm::mat4(1.0f);//视图矩阵
glm::mat4 projection = glm::mat4(1.0f);//投影矩阵


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//摄像机位置
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//摄像机朝向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);//摄像机上方向
float camerafov = 45.0f;//摄像机视野