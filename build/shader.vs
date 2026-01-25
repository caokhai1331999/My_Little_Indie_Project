#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;
layout(location = 3) in vec3 VertexColor;

out vec2 TextCoord;
out vec4 FragColorr;
out vec3 ModelPos;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

bool notNUll(mat4 matrix){
 for(int i = 0; i < 4; i++){
 for(int j = 0 ; j < 4; j++){
      if(matrix[i][j] != 0.0){
        return true;
     };     
   }
 }
 return false;
}

void main(){

// Place Model First -> calculate the local space to World/Perspective space
//                  -> Then Calculate latest space to clip space
       gl_Position = projection * view * model * vec4(aPos, 1.0f);

        TextCoord = vec2(0.0f);
	TextCoord = aTextCoord;

// Model Pos for deciding showing texture or mere color
 ModelPos = vec3(0.0f);

     ModelPos.z = model[1][0];
     ModelPos.y = model[2][0];
     ModelPos.x = model[3][0];

     FragColorr = vec4(VertexColor,1.0f);
}