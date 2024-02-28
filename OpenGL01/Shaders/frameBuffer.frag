#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float x_offset = 1.0 / 1920;
const float y_offset = 1.0 / 1080;

void main()
{ 
    // 反色
    // FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);

    // 灰度化
    // FragColor = texture(screenTexture, TexCoords);
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(average, average, average, 1.0);

    // 边缘提取
    vec2 offsets[9] = vec2[](
        vec2(-x_offset,  y_offset), // 左上
        vec2( 0.0f,    y_offset), // 正上
        vec2( x_offset,  y_offset), // 右上
        vec2(-x_offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( x_offset,  0.0f),   // 右
        vec2(-x_offset, -y_offset), // 左下
        vec2( 0.0f,   -y_offset), // 正下
        vec2( x_offset, -y_offset)  // 右下
    );

    // 锐化
    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );

    // 模糊
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    // 边缘提取
    // float kernel[9] = float[](
        // 1, 1, 1,
        // 1, -8, 1,
        // 1, 1, 1
    // );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}