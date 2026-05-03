#version 330 core

in vec4 FragColorr;
out vec4 FragColor;

uniform sampler2D ttexture;
uniform float colorOffset;

in vec3 ModelPos;
in vec2 TextCoord;

vec4 interpolateColor(vec4 color, float colorOffset){
 vec4 outColor;

       if(colorOffset < 1.0f){
	  outColor.x = color.x + sin(colorOffset);
          outColor.y = color.y + cos(colorOffset);
          outColor.z = color.z + tan(colorOffset);
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
    vec4 texture_ = texture(ttexture, TextCoord);
    if(ModelPos.x >= 0.0f){
     // FragColor = interpolateColor(FragColorr, colorOffset);
     // resetColor(FragColor);
     FragColor = texture_;
    } else {
     FragColor = vec4(0.0f);
    }
}
