#version 330 core


in vec2 uv;

out vec4 FragColor;//输出

uniform sampler2D ourTexture;//贴图
uniform float R;//输入
uniform float G;//输入
uniform float B;//输入

uniform float A = 0.5f;//输入

void main(){
//    FragColor = vec4(vertexColor, 1.0f);
//    FragColor = vec4(R, G, B, 1.0f);
	vec3 t1 = texture(ourTexture, uv).xyz;

	vec3 Color1 = vec3(R, G, B);
//	vec3 TT = mix(t1 , Color1, A);
	vec3 TT = t1 * Color1;

	FragColor = vec4(TT, 1);
}