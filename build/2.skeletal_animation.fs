#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffused1;

void main(){
  FragColor = texture(texture_diffused1, TexCoords);
};