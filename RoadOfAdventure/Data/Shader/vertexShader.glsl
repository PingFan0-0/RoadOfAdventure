#version 330 core

layout (location = 0) in vec3 aPos;//输入
layout (location = 1) in vec2 aUv;//输入

out vec2 uv;


uniform mat4 transform;//输入

uniform mat4 model;//相机透视
uniform mat4 view;//相机透视
uniform mat4 projection;//相机透视

void main(){
    gl_Position =  projection * view * model * transform * vec4(aPos, 1.0);
    uv = aUv;
}