#version 330 core
out vec4 FragColor;

float near = 0.1; 
float far = 5.0;//this should be 100 sampe as outside,but for easy distinguish depth,i set it to 5

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // to NDC
    return (2.0 * near * far) / (far + near - z * (far - near)); // from https://learnopengl.com/Advanced-OpenGL/Depth-testing
}

vec3 ColorFromDepth(float depth)
{
    // 将深度值转换为 [0, 1] 范围内
    float normalizedDepth = depth / far;
    
    // 定义色谱中的颜色
    const vec3 red = vec3(1.0, 0.0, 0.0);
    const vec3 orange = vec3(1.0, 0.647, 0.0);
    const vec3 yellow = vec3(1.0, 1.0, 0.0);
    const vec3 green = vec3(0.0, 1.0, 0.0);
    const vec3 cyan = vec3(0.0, 1.0, 1.0);
    const vec3 blue = vec3(0.0, 0.0, 1.0);
    const vec3 purple = vec3(0.5, 0.0, 0.5);

    // 创建一个颜色渐变表
    vec3[7] colors = vec3[](red, orange, yellow, green, cyan, blue, purple);
    
    // 根据深度值选择颜色，这里简单地将深度范围分成6个等分
    int index = int(clamp(normalizedDepth * 6.0, 0.0, 5.99));
    float blendFactor = fract(normalizedDepth * 6.0);
    return mix(colors[index], colors[index + 1], blendFactor);
}

void main()
{
    float depth = LinearizeDepth(gl_FragCoord.z); // 获取线性深度
    FragColor = vec4(ColorFromDepth(depth), 1.0); // 设置片段颜色，alpha设为1.0
}