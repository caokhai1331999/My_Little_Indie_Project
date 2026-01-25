#version 330 core
in vec4 FragColorr;
out vec4 FragColor;

uniform sampler2D ttexture;

in vec3 ModelPos;
in vec2 TextCoord;

void main(){
     if(ModelPos.x >= 0.0f){
     FragColor = FragColorr;
    } else {
     FragColor = vec4(0.0f);
    }
}