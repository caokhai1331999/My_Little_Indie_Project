#version 330 core

in vec4 FragColorr;
out vec4 FragColor;

uniform sampler2D ttexture;
uniform float colorOffset;
uniform bool short_color_change_;

in vec3 ModelPos;
in vec2 TextCoord;

bool ColorNULL(vec4 color){
    for(int i = 0; i < 4; i++){
        if(color[i]!=0.0f){
            return false;
            break;
        }
    };
    return true;
}

vec4 interpolateColor(vec4 color, float colorOffset){
    vec4 outColor;

    if(colorOffset < 1.0f){
        outColor.x = color.x + sin(colorOffset);
        outColor.y = color.y + cos(colorOffset);
        outColor.z = color.z + tan(colorOffset);

        // outColor.x = sin(color.x + colorOffset);
        // outColor.y = cos(color.y + colorOffset);
        // outColor.z = tan(color.z + colorOffset);

        return outColor;
    }
    outColor = color;
    return outColor ;
}

void resetColor(vec4 color){
    if(color.x >= 1.0f){
        color.x -= 1.0f;      
    }

    if(color.y >= 1.0f){
        color.y -= 1.0f;
    }

    if(color.x >= 1.0f){
        color.z -= 1.0f;
    }
}

void main(){
    // if(ModelPos.x >= 0.0f){
    // resetColor(FragColor);
    // FragColor = interpolateColor(FragColorr, colorOffset);
    if(!short_color_change_){
          // FragColor = interpolateColor(FragColorr, colorOffset);
        FragColor = FragColorr;
     }else{
	  FragColor = vec4(0.7, 0.3, 0.1, 1.0f);
     }
    // } else {
    //  FragColor = vec4(0.0f);
    // }
}
