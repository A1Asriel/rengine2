#version 330 core
in vec3 ourColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D textureSampler;
uniform bool useTexture;

void main() {
    if (!gl_FrontFacing) {
        discard;
        return;
    }
    
    if (useTexture) {
        vec4 texColor = texture(textureSampler, TexCoord);
        FragColor = texColor;
    } else {
        FragColor = vec4(ourColor, 1.0);
    }
}
