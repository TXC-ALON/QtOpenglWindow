#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1
layout (location = 2) in float aVertexID; // 序号id


uniform mat4 model;
uniform mat4 transform;
uniform mat4 view;
uniform float time;
//uniform mat4 projection;
out vec3 ourColor;
float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}
void main() {
    gl_Position = view*transform*model*vec4(aPos, 1.0);
    // 假设 aVertexID 为 0, 1, 2 分别对应红、绿、蓝三个点

    float redRand = 1;
    float greenRand = 1;
    float blueRand = 1;

    float redOffset = sin(time + aVertexID * 3.14159 / 3.0)*redRand;
    float greenOffset = sin(time + (aVertexID) * 3.14159 / 3.0)*greenRand; // 绿色有一个相位偏移
    float blueOffset = sin(time + (aVertexID) * 3.14159 / 3.0)*greenRand; // 蓝色有不同的相位偏移

    // 对应到RGB三个颜色分量上
    vec3 colorOffset = vec3(redOffset, greenOffset, blueOffset);
    // 应用变化到颜色上，确保颜色值保持在 [0, 1] 范围内
    vec3 finalColor = clamp(aColor + colorOffset, 0.0, 1.0);
    // 将最终颜色赋值给 ourColor
    ourColor = finalColor; // 假设我们的颜色有alpha通道
}