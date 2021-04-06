#version 330 core
  
in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D uTexture;

void main()
{
    fragColor = texture(uTexture, texCoord);
}